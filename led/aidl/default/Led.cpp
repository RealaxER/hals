#define LOG_TAG "LED_RPI"

#include <utils/Log.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Led.h"
#include <gpiod.h>

namespace aidl {
namespace bcm {
namespace hardware {
namespace led {

constexpr char GPIO_CHIP[] = "/dev/gpiochip0"; 
constexpr unsigned int GPIO_LINE = 21;        

ndk::ScopedAStatus Led::getValue(int8_t* _aidl_return) {
    struct gpiod_chip* chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        ALOGE("Failed to open GPIO chip: %s", GPIO_CHIP);
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }

    struct gpiod_line* line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (!line) {
        ALOGE("Failed to get GPIO line: %d", GPIO_LINE);
        gpiod_chip_close(chip);
        return ndk::ScopedAStatus::fromServiceSpecificError(-2);
    }

    if (gpiod_line_request_input(line, "led_rpi_service") < 0) {
        ALOGE("Failed to set GPIO line as input");
        gpiod_chip_close(chip);
        return ndk::ScopedAStatus::fromServiceSpecificError(-3);
    }

    int value = gpiod_line_get_value(line);
    if (value < 0) {
        ALOGE("Failed to read GPIO value");
        gpiod_chip_close(chip);
        return ndk::ScopedAStatus::fromServiceSpecificError(-4);
    }

    *_aidl_return = static_cast<int8_t>(value);
    ALOGD("Led service: Value read = %d", *_aidl_return);

    gpiod_chip_close(chip);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Led::setValue(int8_t in_value) {
    if (in_value < 0 || in_value > 1) {
        ALOGE("Invalid input: value must be 0 or 1");
        return ndk::ScopedAStatus::fromServiceSpecificError(-3);
    }

    struct gpiod_chip* chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        ALOGE("Failed to open GPIO chip: %s", GPIO_CHIP);
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }

    struct gpiod_line* line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (!line) {
        ALOGE("Failed to get GPIO line: %d", GPIO_LINE);
        gpiod_chip_close(chip);
        return ndk::ScopedAStatus::fromServiceSpecificError(-2);
    }

    if (gpiod_line_request_output(line, "led_rpi_service", 0) < 0) {
        ALOGE("Failed to set GPIO line as output");
        gpiod_chip_close(chip);
        return ndk::ScopedAStatus::fromServiceSpecificError(-3);
    }

    if (gpiod_line_set_value(line, in_value) < 0) {
        ALOGE("Failed to set GPIO value");
        gpiod_chip_close(chip);
        return ndk::ScopedAStatus::fromServiceSpecificError(-4);
    }

    ALOGD("Value successfully written to GPIO line: %d", in_value);

    gpiod_chip_close(chip);
    return ndk::ScopedAStatus::ok();
}

}  // namespace aidl
}  // namespace bcm
}  // namespace hardware
}  // namespace led