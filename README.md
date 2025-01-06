<div align="center">
    <h1>
        <br>
            Sense
        <br>
    </h1>
</div>

`Sense` is a simple and easy way to deal with the Dual Sense Controller in Linux. It's fast and based on the `linux/joystick.h`. Reads the input from `/dev/input/js0`. 

## License:
[![License](https://img.shields.io/badge/License-MIT-blue.svg?longCache=true&style=flat)](https://github.com/Vinz1911/Sense/blob/master/LICENSE)

## C++ Version:
[![C++23](https://img.shields.io/badge/C++-23-blue.svg?logo=c%2B%2B&style=flat)](https://isocpp.org)

## Import:
```cpp
// include the header file from the project
#include <sense/dualsense.h>
#include <sense/constants.h>

auto dual_sense = sense::DualSense();
if(!dual_sense.set_open()) { printf("Failed to open path\n"); return -1; }
```

## Build and Install:
Compile the projects using `cmake`.
```sh
# in root directory
mkdir build
cd build/
cmake ..
make && make install
```

## Usage:
### Read input data:
```cpp
auto dual_sense = sense::DualSense();
if(!dual_sense.set_open()) { printf("Failed to open path\n"); return -1; }

while (dual_sense.is_active) {
    const auto trigger = dual_sense.axis[AXIS_RIGHT_TRIGGER];
    const auto button_x = dual_sense.buttons[BUTTON_CROSS];

    printf("trigger value %i, button value: %i\n", trigger, button_x);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

### Get status infos and set LED:
```cpp
auto dual_sense = sense::DualSense();
if(!dual_sense.set_open()) { printf("failed to open path\n"); return 1; }

printf("Controller Capacity: %s\n", dual_sense.get_device_info()[0].c_str());
printf("Controller Status: %s\n", dual_sense.get_device_info()[1].c_str());

dual_sense.set_led_color(128, 0, 255);
```

## Author:
👨🏼‍💻 [Vinzenz Weist](https://github.com/Vinz1911)
