#ifndef MyClientCallbacks_h
#define MyClientCallbacks_h

#ifndef BLEDevice_h
#include "BLEDevice.h"
#endif
#ifndef LEDController_h
#include "../LEDController/LEDController.h"
#endif

class MyClientCallbacks: public BLEClientCallbacks {
  private:
    bool connected;
  public:
    MyClientCallbacks();
    void onConnect(BLEClient* pclient);
    void onDisconnect(BLEClient* pclient);
    bool getConnected();
};

#endif