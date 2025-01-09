#include <sense/dualsense.h>
#include <sense/constants.h>

int main() {
    auto sense = sense::DualSense();
    if(!sense.set_open()) { printf("failed to open path\n"); return 1; }
    auto info = sense.get_device_info();

    printf("status: %s\n", info[sense::STATUS].c_str());
    printf("capacity: %s\n", info[sense::CAPACITY].c_str());

    sense.set_led_color(128, 0, 255);

    while (sense.is_active()) {
        auto buttons = sense.get_buttons(); auto axis = sense.get_axis();
        const auto trigger = axis[sense::AXIS_RIGHT_TRIGGER];
        const auto button_x = buttons[sense::BUTTON_CROSS];

        printf("trigger value %i, button value: %i\n", trigger, button_x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } return 0;
}