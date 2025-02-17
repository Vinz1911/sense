#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
#include <iostream>
#include <mutex>

#include "sense/dualsense.h"

namespace sense {
    DualSense::DualSense(const char* path, const uint16_t timeout): device_path_(path), timeout_(timeout) {
        buttons_ = { { BUTTON_CROSS, 0 }, { BUTTON_CIRCLE, 0 }, { BUTTON_TRIANGLE, 0 }, { BUTTON_SQUARE, 0 }, { BUTTON_SHOULDER_LEFT, 0 }, { BUTTON_SHOULDER_RIGHT, 0 }, { BUTTON_TRIGGER_LEFT, 0 }, { BUTTON_TRIGGER_RIGHT, 0 }, { BUTTON_SHARE, 0 }, { BUTTON_OPTIONS, 0 }, { BUTTON_PS, 0 }, { BUTTON_THUMB_LEFT, 0 }, { BUTTON_THUMB_RIGHT, 0 } };
        axis_ = { { AXIS_LEFT_THUMB_X, 0 }, { AXIS_LEFT_THUMB_Y, 0 }, { AXIS_LEFT_TRIGGER, -32767 }, { AXIS_RIGHT_THUMB_X, 0 }, { AXIS_RIGHT_THUMB_Y, 0 }, { AXIS_RIGHT_TRIGGER, -32767 }, { AXIS_D_PAD_LEFT_RIGHT, 0 }, { AXIS_D_PAD_UP_DOWN, 0 } };
    }

    DualSense::~DualSense() { set_close(); }

    bool DualSense::set_open() {
        js_event_path_.store(open(device_path_, O_RDONLY), std::memory_order::relaxed);
        if (timeout_ != 0) { io_event_path_.store(open(get_motion_sensor_path().c_str(), O_RDONLY), std::memory_order::relaxed); }
        if (js_event_path_.load(std::memory_order::relaxed) != -1) { this->get_input_thread(); is_active_.store(true, std::memory_order::relaxed); }
        if (io_event_path_.load(std::memory_order::relaxed) != -1 and timeout_ != 0) { this->get_motion_thread(); this->get_timeout_thread(); }
        return js_event_path_.load(std::memory_order::relaxed) != -1 and io_event_path_.load(std::memory_order::relaxed) != -1;
    }

    bool DualSense::set_close() {
        is_terminated_.store(true, std::memory_order::relaxed);
        for (auto &thread : thread_pool_) { if (thread.joinable()) { thread.join(); } }
        is_active_.store(false, std::memory_order::relaxed);
        return close(js_event_path_.load(std::memory_order::relaxed)) != -1 and close(io_event_path_.load(std::memory_order::relaxed)) != -1;
    }

    bool DualSense::is_active() const {
        return is_active_.load(std::memory_order::relaxed);
    }

    std::map<SenseButtonConstants, int16_t> DualSense::get_buttons() {
        std::lock_guard lock(lock_); return buttons_;
    }

    std::map<SenseAxisConstants, int16_t> DualSense::get_axis() {
        std::lock_guard lock(lock_); return axis_;
    }

    void DualSense::set_led(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t brightness) {
        const std::string rgb_path = "/sys/class/leds/input9:rgb:indicator/multi_intensity";
        const std::string brightness_path = "/sys/class/leds/input9:rgb:indicator/brightness";
        const std::string values = std::to_string(red) + " " + std::to_string(green) + " " + std::to_string(blue);
        pathfinder_.set_value(rgb_path, values); pathfinder_.set_value(brightness_path, brightness);
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

    std::string DualSense::get_motion_sensor_path() {
        const std::string target = "Motion Sensors";
        for (const auto &entry : std::filesystem::directory_iterator("/dev/input/")) {
            if (entry.path().string().find("event") == std::string::npos) { continue; }
            int fd = open(entry.path().c_str(), O_RDONLY); if (fd < 0) { continue; }
            if (char name[256] = {}; ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
                if (std::string(name).find(target) != std::string::npos) { close(fd); return entry.path().string();  }
            } close(fd);
        } return {};
    }

    void DualSense::get_input_thread() {
        thread_pool_[0] = std::thread([this] {
            while (!is_terminated_.load(std::memory_order::relaxed)) {
                if (const ssize_t bytes = read(js_event_path_.load(std::memory_order::relaxed), &js_event_, sizeof(js_event_)); bytes == sizeof(js_event_)) {
                    std::lock_guard lock(lock_);
                    if (js_event_.type == JS_EVENT_BUTTON) { buttons_[static_cast<SenseButtonConstants>(js_event_.number)] = js_event_.value; }
                    if (js_event_.type == JS_EVENT_AXIS) { axis_[static_cast<SenseAxisConstants>(js_event_.number)] = js_event_.value; }
                } else { std::printf("[Sense]: error during read, terminating.\n"); set_close(); break; }
            }
        }); thread_pool_[0].detach();
    }

    void DualSense::get_motion_thread() {
        current_time_.store(std::chrono::high_resolution_clock::now(), std::memory_order::relaxed);
        thread_pool_[1] = std::thread([this] {
            while (!is_terminated_.load(std::memory_order::relaxed)) {
                if (const ssize_t bytes = read(io_event_path_.load(std::memory_order::relaxed), &io_event_, sizeof(io_event_)); bytes == sizeof(io_event_)) {
                    if (io_event_.type != EV_MSC && io_event_.code != MSC_TIMESTAMP) { continue; }
                    current_time_.store(std::chrono::high_resolution_clock::now(), std::memory_order::relaxed);
                }
            }
        }); thread_pool_[1].detach();
    }

    void DualSense::get_timeout_thread() {
        thread_pool_[2] = std::thread([this] {
            const auto period = std::max(static_cast<uint16_t>(100), timeout_);
            while (!is_terminated_.load(std::memory_order::relaxed)) {
                if (std::chrono::high_resolution_clock::now() >= current_time_.load(std::memory_order::relaxed) + std::chrono::milliseconds(period)) {
                    std::printf("[Sense]: timeout error.\n"); this->set_close();
                } std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }); thread_pool_[2].detach();
    }
} // namespace sense