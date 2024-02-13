#ifndef Bluetooth_h
#define Bluetooth_h

#ifndef BLEDevice_h
#include "BLEDevice.h"
#endif
#ifndef MyAdvertisedDeviceCallbacks_h
#include "MyAdvertisedDeviceCallbacks.h"
#endif
#ifndef MyClientCallbacks_h
#include "MyClientCallbacks.h"
#endif

class Bluetooth {
  private:
    BLEUUID serviceUUID;
    BLEUUID charUUID;
    BLEScan* pBLEScan;
    BLEAdvertisedDevice* targetDevice;
    MyAdvertisedDeviceCallbacks* advertCallback;
    BLEClient*  pClient;
    MyClientCallbacks* clientCallback;
    BLERemoteService* pRemoteServ;
    BLERemoteCharacteristic* pRemoteChar;
    int timedelay;
    bool setData();
    bool scan();
    bool connect();
    bool findService();
    bool findCharacteristic();
    bool startNotify();
    bool stopNotify();
    bool disconnect();
    void clearInstances();
  public:
    Bluetooth(const char* serviceUUID, const char* charUUID);
    String getData();
    static int sizeCont;
    static int contData;
    static double arrayData[20];
    static String data;
    static bool ready;
};

static void notifyCallback(
BLERemoteCharacteristic* pBLERemoteCharacteristic,
uint8_t* pData,
size_t length,
bool isNotify);

#endif