#pragma once

#include <aidl/bcm/hardware/led/BnLed.h>
namespace aidl {
namespace bcm {
namespace hardware {
namespace led {

class Led: public BnLed{
    public:
        ndk::ScopedAStatus setValue(int8_t in_value) ;
        ndk::ScopedAStatus getValue(int8_t* _aidl_return) ;
};

}  // namespace hello
}  // namespace hardware
}  // namespace android
}  // namespace aidl