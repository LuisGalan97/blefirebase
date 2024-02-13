#include "Bluetooth/Bluetooth.h"
#include "Bluetooth/Bluetooth.cpp"
#include "Bluetooth/MyAdvertisedDeviceCallbacks.cpp"
#include "Bluetooth/MyClientCallbacks.cpp"
#include "FirebaseDB/FirebaseDB.h"
#include "FirebaseDB/FirebaseDB.cpp"
#include "Config/Config.h"

Bluetooth* ble;
FirebaseDB* firebaseDB;

void setup() {
  Serial.begin(115200);
  delay(5000);
  ble = new Bluetooth(SERVICE_UUID, CHAR_UUID);
  firebaseDB = new FirebaseDB(WIFI_SSID, WIFI_PASSWORD,
                              FIREBASE_HOST, FIREBASE_AUTH,
                              CLUSTER_ID, SENSOR_ID);
}

void loop() {
  String data = ble->getData();
  firebaseDB->sendData(data);
}
