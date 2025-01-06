#include <unistd.h>
#include <sense/dualsense.h>
#include <sense/constants.h>

int main() {
    auto dual_sense = sense::DualSense();
    if(!dual_sense.set_open()) { printf("failed to open path\n"); return 1; }

    printf("Controller Capacity: %s\n", dual_sense.get_device_info()[0].c_str());
    printf("Controller Status: %s\n", dual_sense.get_device_info()[1].c_str());

    dual_sense.set_led_color(128, 0, 255);

    while (dual_sense.is_active) {
        const auto trigger = dual_sense.axis[AXIS_RIGHT_TRIGGER];
        const auto button_x = dual_sense.buttons[BUTTON_CROSS];

        printf("trigger value %i, button value: %i\n", trigger, button_x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}