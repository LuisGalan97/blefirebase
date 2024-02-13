#ifndef FirebaseDB_h
#define FirebaseDB_h

#ifndef WiFi_h
#include <WiFi.h>
#endif
#ifndef FirebaseESP32_h
#include "FirebaseESP32.h"
#endif
#ifndef LEDController_h
#include "../LEDController/LEDController.h"
#endif

class FirebaseDB {
  private:
    const char* wiFiSSID;
    const char* wiFiPass;
    const char* fireHost;
    const char* fireAuth;
    String node;
    bool firebaseConn;
    bool boolData;
    FirebaseData firebaseData;
    bool setData(String data);
    bool scanWiFi();
    bool connectWiFi();
    bool disconnectWiFi();
    bool connectFirebase();
    bool readBoolData(String target);
    bool sendStringData(String target, String data);
    int timedelay;
    void customDelay();
    int attemps;
  public:
    FirebaseDB(const char* wiFiSSID, const char* wiFiPass,
               const char* fireHost, const char* fireAuth,
               String clusterID, String sensorID);
    void sendData(String data);
};

#endif