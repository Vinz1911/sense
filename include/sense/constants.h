#ifndef SENSE_CONSTANTS_H
#define SENSE_CONSTANTS_H

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

#endif //SENSE_CONSTANTS_H