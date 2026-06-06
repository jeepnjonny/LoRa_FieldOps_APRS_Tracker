#ifndef DEVICE_ROLE_H_
#define DEVICE_ROLE_H_

#include "configuration.h"

namespace DeviceRoleUtils {

    const char* getRoleString(DeviceRole role);

    void initializeRole(DeviceRole role);

    void initializeTracker();
    void initializeDigipeater();

    #ifdef HAS_WIFI
    void initializeWiFiSTA();
    void initializeIGate();
    void handleWiFiTasks();
    void handleIGateTasks();
    #endif

    void handleRoleSpecificTasks();

    DeviceRole getCurrentRole();
    bool isRoleInitialized();

    bool validateRoleForPlatform(DeviceRole role);

}

#endif
