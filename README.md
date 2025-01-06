<div align="center">
    <h1>
        <br>
            PSGamepad
        <br>
    </h1>
</div>

`PSGamepad` is a simple and easy way to deal with the PS4 Controller under Linux. It's fast and based on the `linux/joystick.h`, that means the input trigger is event based. So the `PSGamepad` is made async and non-blocking and runs on his own detached thread. 

## Features:
- [X] fast event based input reading
- [X] dependency free
- [X] read all buttons + axises
- [X] set controller led (needs root rights)
- [X] get controller capacity and status

## License:
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg?longCache=true&style=flat)](https://github.com/Vinz1911/PSGamepad/blob/master/LICENSE)

## C++ Version:
[![C++17](https://img.shields.io/badge/C++-17-blue.svg?logo=c%2B%2B&style=flat)](https://isocpp.org)

## Import:
```cpp
// include the header file from the project
#include <ps_gamepad.h>

// create instance of the gamepad 
PSGamepad gamepad = PSGamepad();
if(!gamepad.set_open()) {
    // handle error
}
```

## Building:
Compile the projects using `make`.
- GamepadDemo: `make GamepadDemo`
- KeyMap: `make KeyMap`

This places an executable in the working directory.
Installation targets are currently not available, just copy the executable to `/usr/local/bin`.

## Usage:
### Read input data:
```cpp
// read controller input, this is non blocking.
gamepad.get_input([&](std::vector<int> input){
    if (input.empty()) {
        // handle error
    }
    // EVENT_TYPE = 0
    if (input[EVENT_TYPE] == JS_EVENT_BUTTON) {
        // EVENT_NUMBER = 1
        if (input[EVENT_NUMBER] == BUTTON_CROSS) {
            // EVENT_VALUE = 2
            printf("BUTTON CROSS: %i\n", input[EVENT_VALUE]);
        }
    }
});
```

### Get status infos and set LED:
```cpp
// this is a example for the mac, you need the mac from your controller...
#define GAMEPAD_MAC "a4:ae:11:c6:13:c2"

// ...same for the LED's
#define GAMEPAD_RED_LED "0005:054C:09CC.0005:red"
#define GAMEPAD_GREEN_LED "0005:054C:09CC.0005:green"
#define GAMEPAD_BLUE_LED "0005:054C:09CC.0005:blue"

#define GAMEPAD_CAPACITY 0
#define GAMEPAD_STATUS 1

printf("Controller Capacity: %s\n", gamepad.get_device_info(GAMEPAD_MAC)[GAMEPAD_CAPACITY].c_str());
printf("Controller Status: %s\n", gamepad.get_device_info(GAMEPAD_MAC)[GAMEPAD_STATUS].c_str());

printf("RED LED Brightness: %i\n", gamepad.get_led_brightness(GAMEPAD_RED_LED));
printf("GREEN LED Brightness: %i\n", gamepad.get_led_brightness(GAMEPAD_GREEN_LED));
printf("BLUE LED Brightness: %i\n", gamepad.get_led_brightness(GAMEPAD_BLUE_LED));

gamepad.set_led_brightness(GAMEPAD_RED_LED, 0);
gamepad.set_led_brightness(GAMEPAD_GREEN_LED, 255);
gamepad.set_led_brightness(GAMEPAD_BLUE_LED, 255);
```

## Author:
👨🏼‍💻 [Vinzenz Weist](https://github.com/Vinz1911)
