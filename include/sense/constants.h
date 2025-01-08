#ifndef SENSE_CONSTANTS_H
#define SENSE_CONSTANTS_H

namespace sense {
    enum SenseAxisConstants: uint8_t {
        AXIS_LEFT_THUMB_X = 0,
        AXIS_LEFT_THUMB_Y = 1,
        AXIS_LEFT_TRIGGER = 2,
        AXIS_RIGHT_THUMB_X = 3,
        AXIS_RIGHT_THUMB_Y = 4,
        AXIS_RIGHT_TRIGGER = 5,
        AXIS_D_PAD_LEFT_RIGHT = 6,
        AXIS_D_PAD_UP_DOWN = 7
    };

    enum SenseButtonConstants: uint8_t {
        BUTTON_CROSS = 0,
        BUTTON_CIRCLE = 1,
        BUTTON_TRIANGLE = 2,
        BUTTON_SQUARE = 3,
        BUTTON_SHOULDER_LEFT = 4,
        BUTTON_SHOULDER_RIGHT = 5,
        BUTTON_TRIGGER_LEFT = 6,
        BUTTON_TRIGGER_RIGHT = 7,
        BUTTON_SHARE = 8,
        BUTTON_OPTIONS = 9,
        BUTTON_PS = 10,
        BUTTON_THUMB_LEFT = 11,
        BUTTON_THUMB_RIGHT = 12
    };

    enum SenseStatusConstants: uint8_t {
        STATUS = 0,
        CAPACITY = 1,
    };
} // namespace sense

#endif //SENSE_CONSTANTS_H