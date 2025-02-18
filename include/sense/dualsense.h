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

#ifndef SENSE_INPUT_H
#define SENSE_INPUT_H

#include <array>
#include <string>
#include <map>
#include <thread>
#include <linux/joystick.h>

#include "constants.h"
#include "pathfinder.h"

namespace sense {
    class DualSense {
    public:
        /**
         * @brief create instance of `DualSense`.
         * @param path input path, default is "/dev/input/js0".
         * @param timeout the time before connection gets closed because of non responsibility.
         */
        explicit DualSense(const char* path = "/dev/input/js0", uint16_t timeout = 1000);

        /**
         * @brief destroy instance of `DualSense`.
         */
        ~DualSense();

        /**
         * @brief status if the device is active.
         */
        [[nodiscard]] bool is_active() const;

        /**
         * @brief open the connection to a device's path.
         * @return bool indicates success.
         */
        bool set_open();

        /**
         * @brief close the connection to a device's path.
         * @return bool indicates success.
         */
        bool set_close();

        /**
         * @brief get buttons.
         * @return map of buttons and values.
         */
        std::map<SenseButtonConstants, int16_t> get_buttons();

        /**
         * @brief get axis.
         * @return map of axis and values.
         */
        std::map<SenseAxisConstants, int16_t> get_axis();

        /**
         * @brief set the intensity values for Red, Green and Blue.
         * @param red set level 0-255.
         * @param green set level 0-255.
         * @param blue set level 0-255.
         * @param brightness set level 0-255.
         */
        void set_led(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 255);

        /**
         * @brief get status infos from the device.
         * @return a vector containing device info's.
         * [sense::STATUS] or [sense::CAPACITY].
         */
        std::map<SenseStatusConstants, std::string> get_device_info();

    private:
        /**
         * @brief stores the devices path.
         */
        const char* device_path_;

        /**
         * @brief time before timout appears.
         */
        uint16_t grace_period_;

        /**
         * @brief pathfinder to get and set path values.
         */
        Pathfinder pathfinder_ = Pathfinder();

        /**
         * @brief stores the active js event path.
         */
        std::atomic<int> js_event_path_ = {};

        /**
         * @brief stores the active io event path.
         */
        std::atomic<int> io_event_path_ = {};

        /**
         * @brief status if the device is active.
         */
        std::atomic<bool> is_active_ = false;

        /**
         * @brief button values.
         */
        std::map<SenseButtonConstants, int16_t> buttons_;

        /**
         * @brief axis values.
         */
        std::map<SenseAxisConstants, int16_t> axis_;

        /**
         * @brief stores the js event data.
         */
        js_event js_event_ = {};

        /**
         * @brief stores the input event data.
         */
        input_event io_event_ = {};

        /**
         * @brief check if terminated.
         */
        std::atomic<bool> is_terminated_ = {false};

        /**
         * @brief thread pool.
         */
        std::array<std::thread, 3> thread_pool_ = {};

        /**
         *@brief mutex lock for thread safety.
         */
        std::mutex mutex_lock_;

        /**
         * @brief the current time for measuring possible timeout.
         */
        std::atomic<std::chrono::high_resolution_clock::time_point> current_time_;

        /**
         * @brief set the input thread.
         */
        void set_input_thread();

        /**
         * @brief set the timestamp thread.
         */
        void set_timestamp_thread();

        /**
         * @brief set the timeout thread.
         */
        void set_timeout_thread();

        /**
         * @brief get the sensor event path.
         */
        static std::string get_sensor_path();
    };
} // namespace sense

#endif //SENSE_INPUT_H