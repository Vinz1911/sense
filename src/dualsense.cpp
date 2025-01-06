#include "sense/dualsense.h"
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <filesystem>
#include <iostream>

namespace sense {
    DualSense::DualSense(const char* path): device_path_(path) {
        buttons = { {BUTTON_CROSS, 0}, {BUTTON_CIRCLE, 0}, {BUTTON_TRIANGLE, 0}, {BUTTON_SQUARE, 0}, {BUTTON_SHOULDER_LEFT, 0}, {BUTTON_SHOULDER_RIGHT, 0}, {BUTTON_TRIGGER_LEFT, 0}, {BUTTON_TRIGGER_RIGHT, 0}, {BUTTON_SHARE, 0}, {BUTTON_OPTIONS, 0}, {BUTTON_PS, 0}, {BUTTON_THUMB_LEFT, 0}, {BUTTON_THUMB_RIGHT, 0} };
        axis = { {AXIS_LEFT_THUMB_X, 0}, {AXIS_LEFT_THUMB_Y, 0}, {AXIS_LEFT_TRIGGER, -32767}, {AXIS_RIGHT_THUMB_X, 0}, {AXIS_RIGHT_THUMB_Y, 0}, {AXIS_RIGHT_TRIGGER, -32767}, {AXIS_D_PAD_LEFT_RIGHT, 0}, {AXIS_D_PAD_UP_DOWN, 0} };
    }

    DualSense::~DualSense() { set_close(); }

    bool DualSense::set_open() {
        connection_ = open(device_path_, O_RDONLY);
        if (connection_ != -1) { get_input(); is_active = true; }
        return connection_ != -1;
    }

    bool DualSense::set_close() {
        is_terminated_ = true; is_active = false;
        if (input_thread_.joinable()) { input_thread_.join(); }
        return close(connection_) != -1;
    }

    void DualSense::set_led_brightness(const uint8_t brightness) {
        const std::string path = "/sys/class/leds/input9:rgb:indicator/brightness";
        set_fstream_value(path, brightness);
    }

    std::string DualSense::get_led_brightness() {
        const std::string path = "/sys/class/leds/input9:rgb:indicator/brightness";
        return get_fstream_value(path);
    }

    void DualSense::set_led_color(const uint8_t red, const uint8_t green, const uint8_t blue) {
        const std::string path = "/sys/class/leds/input9:rgb:indicator/multi_intensity";
        const std::string values = std::to_string(red) + " " + std::to_string(green) + " " + std::to_string(blue);
        set_fstream_value(path, values);
    }

    std::vector<std::string> DualSense::get_device_info() {
        std::vector<std::string> device_info;
        for (const std::string power_supply_path = "/sys/class/power_supply/"; const auto& entry : std::filesystem::directory_iterator(power_supply_path)) {
            if (entry.is_directory() && entry.path().filename().string().rfind("ps-controller-battery-", 0) == 0) {
                const std::string path = entry.path().string();
                device_info.push_back(get_fstream_value(path + "/capacity"));
                device_info.push_back(get_fstream_value(path + "/status")); break;
            }
        } return device_info;
    }

    // MARK: - Private API -
    void DualSense::get_input() {
        input_thread_ = std::thread([this] {
            while (!is_terminated_) {
                if (const ssize_t bytes = read(connection_, &event_, sizeof(event_)); bytes == sizeof(event_)) {
                    std::lock_guard lock(lock_);
                    if (event_.type == JS_EVENT_BUTTON) { buttons[static_cast<SenseButtonConstants>(event_.number)] = event_.value; }
                    if (event_.type == JS_EVENT_AXIS) { axis[static_cast<SenseAxisConstants>(event_.number)] = event_.value; }
                } else { std::printf("[Sense]: Error during read, terminating.\n"); set_close(); break; }
            }
        }); input_thread_.detach();
    }

    std::string DualSense::get_fstream_value(const std::string& path) {
        auto value = std::string();
        stream_.open(path, std::fstream::in);
        stream_ >> value; stream_.close(); return value;
    }

    template <typename T>
    void DualSense::set_fstream_value(const std::string& path, const T& value) {
        stream_.open(path, std::fstream::out);
        stream_ << value; stream_.close();
    }
} // namespace sense