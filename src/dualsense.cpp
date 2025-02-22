/*
 * MIT License
 *
 * Copyright (c) 2025 Vinzenz Weist
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <mutex>

#include "sense/dualsense.h"

namespace sense {
    static constexpr auto MEMORY_ORDER = std::memory_order::relaxed;

    DualSense::DualSense(const char* path, const uint16_t timeout): device_path_(path), timeout_(timeout), is_log_(false) {
        reset_input();
    }

    DualSense::~DualSense() { set_close(); }

    void DualSense::reset_input() {
        buttons_ = { { BUTTON_CROSS, 0 }, { BUTTON_CIRCLE, 0 }, { BUTTON_TRIANGLE, 0 }, { BUTTON_SQUARE, 0 }, { BUTTON_SHOULDER_LEFT, 0 }, { BUTTON_SHOULDER_RIGHT, 0 }, { BUTTON_TRIGGER_LEFT, 0 }, { BUTTON_TRIGGER_RIGHT, 0 }, { BUTTON_SHARE, 0 }, { BUTTON_OPTIONS, 0 }, { BUTTON_PS, 0 }, { BUTTON_THUMB_LEFT, 0 }, { BUTTON_THUMB_RIGHT, 0 } };
        axis_ = { { AXIS_LEFT_THUMB_X, 0 }, { AXIS_LEFT_THUMB_Y, 0 }, { AXIS_LEFT_TRIGGER, -32767 }, { AXIS_RIGHT_THUMB_X, 0 }, { AXIS_RIGHT_THUMB_Y, 0 }, { AXIS_RIGHT_TRIGGER, -32767 }, { AXIS_D_PAD_LEFT_RIGHT, 0 }, { AXIS_D_PAD_UP_DOWN, 0 } };
    }

    bool DualSense::set_open() {
        is_terminated_.store(false, MEMORY_ORDER); js_event_path_.store(open(device_path_, O_RDONLY), MEMORY_ORDER);
        if (timeout_ != 0) { io_event_path_.store(open(get_sensor_path().c_str(), O_RDONLY), MEMORY_ORDER); }
        if (js_event_path_.load(MEMORY_ORDER) != -1) { this->set_input_thread(); is_active_.store(true, MEMORY_ORDER); }
        if (io_event_path_.load(MEMORY_ORDER) != -1 and timeout_ != 0) { this->set_timestamp_thread(); this->set_timeout_thread(); }
        return js_event_path_.load(MEMORY_ORDER) != -1 and io_event_path_.load(MEMORY_ORDER) != -1;
    }

    bool DualSense::set_close() {
        is_terminated_.store(true, MEMORY_ORDER);
        for (auto &thread : thread_pool_) { if (thread.joinable()) { thread.join(); } }
        is_active_.store(false, MEMORY_ORDER); thread_pool_ = {}; reset_input();
        return close(js_event_path_.load(MEMORY_ORDER)) != -1 and close(io_event_path_.load(MEMORY_ORDER)) != -1;
    }

    bool DualSense::is_active() const {
        return is_active_.load(MEMORY_ORDER);
    }

    void DualSense::set_logging(const bool enable) {
        is_log_ = enable;
    }

    std::map<SenseButtonConstants, int16_t> DualSense::get_buttons() {
        std::lock_guard lock(mutex_lock_); return buttons_;
    }

    std::map<SenseAxisConstants, int16_t> DualSense::get_axis() {
        std::lock_guard lock(mutex_lock_); return axis_;
    }

    void DualSense::set_led(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t brightness) {
        std::string rgb_path; std::string brightness_path; bool device_found = false;
        for (const auto& entry : std::filesystem::directory_iterator("/sys/class/leds/")) {
            if (entry.is_directory() && entry.path().string().find(":rgb:indicator") != std::string::npos) {
                rgb_path = entry.path().string() + "/multi_intensity";
                brightness_path = entry.path().string() + "/brightness";
                device_found = true; break;
            }
        }
        if (!device_found) { if (is_log_) { std::printf("[Sense]: error, no valid rgb device found.\n"); } return; }
        const std::string values = std::to_string(red) + " " + std::to_string(green) + " " + std::to_string(blue);
        pathfinder_.set_value(rgb_path, values); pathfinder_.set_value(brightness_path, std::to_string(brightness));
    }

    std::map<SenseStatusConstants, std::string> DualSense::get_device_info() {
        std::map<SenseStatusConstants, std::string> device_info = { { STATUS, "" }, { CAPACITY, "" } };
        for (const std::string power_supply_path = "/sys/class/power_supply/"; const auto& entry : std::filesystem::directory_iterator(power_supply_path)) {
            if (entry.is_directory() && entry.path().filename().string().rfind("ps-controller-battery-", 0) == 0) {
                const std::string path = entry.path().string();
                device_info[STATUS] = pathfinder_.get_value(path + "/status");
                device_info[CAPACITY] = pathfinder_.get_value(path + "/capacity"); break;
            }
        } return device_info;
    }

    std::string DualSense::get_sensor_path() {
        const std::string target = "Motion Sensors";
        for (const auto &entry : std::filesystem::directory_iterator("/dev/input/")) {
            if (entry.path().string().find("event") == std::string::npos) { continue; }
            int fd = open(entry.path().c_str(), O_RDONLY); if (fd < 0) { continue; }
            if (char name[256] = {}; ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
                if (std::string(name).find(target) != std::string::npos) { close(fd); return entry.path().string(); }
            } close(fd);
        } return {};
    }

    void DualSense::set_input_thread() {
        thread_pool_[0] = std::thread([this] {
            while (!is_terminated_.load(MEMORY_ORDER)) {
                if (const ssize_t bytes = read(js_event_path_.load(MEMORY_ORDER), &js_event_, sizeof(js_event_)); bytes == sizeof(js_event_)) {
                    std::lock_guard lock(mutex_lock_);
                    if (js_event_.type == JS_EVENT_BUTTON) { buttons_[static_cast<SenseButtonConstants>(js_event_.number)] = js_event_.value; }
                    if (js_event_.type == JS_EVENT_AXIS) { axis_[static_cast<SenseAxisConstants>(js_event_.number)] = js_event_.value; }
                } else { if (is_log_) { std::printf("[Sense]: error during read, terminating.\n"); } set_close(); break; }
            }
        }); thread_pool_[0].detach();
    }

    void DualSense::set_timestamp_thread() {
        current_time_.store(std::chrono::high_resolution_clock::now(), MEMORY_ORDER);
        thread_pool_[1] = std::thread([this] {
            while (!is_terminated_.load(MEMORY_ORDER)) {
                if (const ssize_t bytes = read(io_event_path_.load(MEMORY_ORDER), &io_event_, sizeof(io_event_)); bytes == sizeof(io_event_)) {
                    if (io_event_.type != EV_MSC or io_event_.code != MSC_TIMESTAMP) { continue; }
                    current_time_.store(std::chrono::high_resolution_clock::now(), MEMORY_ORDER);
                }
            }
        }); thread_pool_[1].detach();
    }

    void DualSense::set_timeout_thread() {
        thread_pool_[2] = std::thread([this] {
            const auto timeout = std::max(static_cast<uint16_t>(100), timeout_);
            while (!is_terminated_.load(MEMORY_ORDER)) {
                if (std::chrono::high_resolution_clock::now() >= current_time_.load(MEMORY_ORDER) + std::chrono::milliseconds(timeout)) {
                    if (is_log_) { std::printf("[Sense]: error, run into timeout.\n"); this->set_close(); }
                } std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }); thread_pool_[2].detach();
    }
} // namespace sense