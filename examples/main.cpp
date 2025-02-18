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

#include <sense/dualsense.h>
#include <sense/constants.h>

int main() {
    auto sense = sense::DualSense();
    if(!sense.set_open()) { printf("failed to open path\n"); return 1; }

    // get device information's like charging status or capacity.
    auto info = sense.get_device_info();
    printf("status: %s\n", info[sense::STATUS].c_str());
    printf("capacity: %s\n", info[sense::CAPACITY].c_str());

    // update the rgb color of the device.
    sense.set_led(64, 0, 255);

    // get input while the device is reachable.
    while (sense.is_active()) {
        auto buttons = sense.get_buttons(); auto axis = sense.get_axis();
        const auto trigger = axis[sense::AXIS_RIGHT_TRIGGER];
        const auto button = buttons[sense::BUTTON_CROSS];

        std::printf("trigger value %i, button value: %i\n", trigger, button);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } return 0;
}