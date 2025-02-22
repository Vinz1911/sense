# Sense
**Sense** is a lightweight and efficient solution for using the DualSense controller on Linux.
It provides a fast and reliable way to process controller input by utilizing the `linux/joystick.h` interface and reading data directly from `/dev/input/js0` or a specified path.
Designed for simplicity, Sense ensures smooth and responsive interaction without unnecessary complexity. 
For different kind of applications, it offers a seamless experience with minimal overhead. 

## License:
[![License](https://img.shields.io/badge/License-MIT-blue.svg?longCache=true&style=flat)](https://github.com/Vinz1911/sense/blob/master/LICENSE)

## C++ Version:
[![C++23](https://img.shields.io/badge/C++-23-blue.svg?logo=c%2B%2B&style=flat)](https://isocpp.org)

## Import:
```cpp
// include the header file from the project
#include <sense/dualsense.h>
#include <sense/constants.h>

auto sense = sense::DualSense();
if(!sense.set_open()) { printf("Failed to open path\n"); return -1; }
```

## Build and Install:
```sh
# in root directory
mkdir build
cd build/
cmake ..
make && make install
```

## Access LED's:
```shell
# in "/etc/udev/rules.d/" for gpio group in rasberry pi
sudo touch 99-leds.rules
SUBSYSTEM=="leds", ACTION=="add", PROGRAM="/bin/sh -c 'chgrp -R gpio /sys%p && chmod -R g=u /sys%p'"
```

## Usage:
### Read input data:
```cpp
auto sense = sense::DualSense();
if(!sense.set_open()) { printf("Failed to open path\n"); return -1; }

while (sense.is_active) {
    const auto trigger = sense.axis[sense::AXIS_RIGHT_TRIGGER];
    const auto button_x = sense.buttons[sense::BUTTON_CROSS];

    printf("trigger value %i, button value: %i\n", trigger, button_x);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
```

### Get status infos and set LED:
```cpp
auto sense = sense::DualSense();
if(!sense.set_open()) { printf("failed to open path\n"); return 1; }

auto info = sense.get_device_info();
printf("status: %s\n", info[sense::STATUS].c_str());
printf("capacity: %s\n", info[sense::CAPACITY].c_str());

sense.set_led(128, 0, 255);
```

## Author:
👨🏼‍💻 [Vinzenz Weist](https://github.com/Vinz1911)
