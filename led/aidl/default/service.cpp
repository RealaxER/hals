#define LOG_TAG "LED_SERVICE"
/*Các thư viện của binder_manager*/
#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include "Led.h"

using aidl::bcm::hardware::led::Led;
using std::string_literals::operator""s;

void log_line(std::string msg) {
    std::cout << msg << std::endl;
    ALOGD("%s", msg.c_str());
}

void log_err(std::string msg) {
    std::cout << msg << std::endl;
    ALOGE("%s", msg.c_str());
}

int main() {
    // Cho phép vendor gọi vendor bằng vndbinder call 
    android::ProcessState::initWithDriver("/dev/vndbinder");

    /*Cấu hình thread*/
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    ABinderProcess_startThreadPool();

    /*Truy cập vào default của aidl led*/
    std::shared_ptr<Led> led = ndk::SharedRefBase::make<Led>();
    const std::string name = Led::descriptor + "/default"s;

    /*Check xem có trỏ đến được không*/
    if (led != nullptr) {
        if(AServiceManager_addService(led->asBinder().get(), name.c_str()) != STATUS_OK) {
            log_err("Failed to register ILed service");
            return -1;
        }
    } else {
        log_err("Failed to get ILed instance");
        return -1;
    }

    log_line("ILed service starts to join service pool");
    ABinderProcess_joinThreadPool();

    return EXIT_FAILURE; 
}