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

#pragma once

namespace sense {
    enum SenseAxisConstants: uint8_t {
        AXIS_LEFT_THUMB_X = 0x00,
        AXIS_LEFT_THUMB_Y = 0x01,
        AXIS_LEFT_TRIGGER = 0x02,
        AXIS_RIGHT_THUMB_X = 0x03,
        AXIS_RIGHT_THUMB_Y = 0x04,
        AXIS_RIGHT_TRIGGER = 0x05,
        AXIS_D_PAD_LEFT_RIGHT = 0x06,
        AXIS_D_PAD_UP_DOWN = 0x07
    };

    enum SenseButtonConstants: uint8_t {
        BUTTON_CROSS = 0x00,
        BUTTON_CIRCLE = 0x01,
        BUTTON_TRIANGLE = 0x02,
        BUTTON_SQUARE = 0x03,
        BUTTON_SHOULDER_LEFT = 0x04,
        BUTTON_SHOULDER_RIGHT = 0x05,
        BUTTON_TRIGGER_LEFT = 0x06,
        BUTTON_TRIGGER_RIGHT = 0x07,
        BUTTON_SHARE = 0x08,
        BUTTON_OPTIONS = 0x09,
        BUTTON_PS = 0x0a,
        BUTTON_THUMB_LEFT = 0x0b,
        BUTTON_THUMB_RIGHT = 0x0c
    };

    enum SenseStatusConstants: uint8_t {
        STATUS = 0x00,
        CAPACITY = 0x01,
    };
} // namespace sense