#ifndef SENSE_INPUT_H
#define SENSE_INPUT_H

#include <string>
#include <map>
#include <thread>
#include <fstream>
#include <linux/joystick.h>

#include "constants.h"

namespace sense {
    class DualSense {
    public:
        /**
         * initialize HID Device
         * @param path -> input path, default is "/dev/input/js0"
         */
        explicit DualSense(const char* path = "/dev/input/js0");

        /**
         * deconstruct object
         */
        ~DualSense();

        /**
         * status if the HID is active
         */
        bool is_active() const;

        /**
         * open the connection to a HID Device's path
         * @return bool indicates success
         */
        bool set_open();

        /**
         * close the connection to a HID Device's path
         * @return bool indicates success
         */
        bool set_close();

        /**
         * @brief get buttons
         * @return map of buttons and values
         */
        std::map<SenseButtonConstants, int16_t> get_buttons();

        /**
         * @brief get axis
         * @return map of axis and values
         */
        std::map<SenseAxisConstants, int16_t> get_axis();

        /**
         * set the LED's of the HID
         * @param brightness the brightness level 0-255
         */
        void set_led_brightness(uint8_t brightness);

        /**
         * get the brightness level of a controller LED
         * @return the brightness level 0-255
         */
        std::string get_led_brightness();

        /**
         * set the intensity values for Red, Green and Blue
         * @param red set level 0-255
         * @param green set level 0-255
         * @param blue set level 0-255
         */
        void set_led_color(uint8_t red, uint8_t green, uint8_t blue);

        /**
         * get status infos from the HID Device
         * @return a vector containing device info's
         * [0]: CAPACITY
         * [1]: STATUS
         */
        std::map<SenseStatusConstants, std::string> get_device_info();

    private:
        /**
         * stores the devices path
         */
        const char* device_path_;

        /**
         * stores the connection
         */
        std::atomic<int> connection_ = {};

        /**
         * status if the HID is active
         */
        std::atomic<bool> is_active_ = false;

        /**
         * button values
         */
        std::map<SenseButtonConstants, int16_t> buttons_;

        /**
         * axis values
         */
        std::map<SenseAxisConstants, int16_t> axis_;

        /**
         * stores the input data
         */
        js_event event_ = {};

        /**
         * check if terminated
         */
        std::atomic<bool> is_terminated_ = {false};

        /**
         * input thread
         */
        std::thread input_thread_ = {};

        /**
         * mutex lock for thread safety
         */
        std::mutex lock_;

        /**
         * file stream
         */
        std::fstream stream_;

        /**
         * read the gamepad input
         */
        void get_input();

        /**
         * get data from a fstream
         * @param path the path to read from
         * @return the value
         */
        std::string get_value(const std::string& path);

        /**
         * set data to a fstream
         * @param path the path to write to
         * @param value the value to write
         */
        template <typename T>
        void set_value(const std::string& path, const T& value);
    };
} // namespace sense

#endif //SENSE_INPUT_H