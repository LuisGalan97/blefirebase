#ifndef MyAdvertisedDeviceCallbacks_h
#define MyAdvertisedDeviceCallbacks_h

#ifndef BLEDevice_h
#include "BLEDevice.h"
#endif
#ifndef LEDController_h
#include "../LEDController/LEDController.h"
#endif

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  private:
    BLEUUID serviceUUID;
    BLEScan* pBLEScan;
    BLEAdvertisedDevice* targetDevice;
  public:
    MyAdvertisedDeviceCallbacks(BLEUUID serviceUUID, BLEScan* pBLEScan);
    void onResult(BLEAdvertisedDevice advertisedDevice);
    BLEAdvertisedDevice getTargetDevice();
};

#endif