#include "Bluetooth/Bluetooth.h"
#include "Bluetooth/Bluetooth.cpp"
#include "Bluetooth/MyAdvertisedDeviceCallbacks.cpp"
#include "Bluetooth/MyClientCallbacks.cpp"
#include "FirebaseDB/FirebaseDB.h"
#include "FirebaseDB/FirebaseDB.cpp"
#include "LEDController/LEDController.cpp"
#include "Config/Config.h"

Bluetooth* ble;
FirebaseDB* firebaseDB;
int iteration;

void setup() {
  Serial.begin(115200);
  delay(5000);
  iteration = 0;
  ble = new Bluetooth(SERVICE_UUID, CHAR_UUID);
  firebaseDB = new FirebaseDB(WIFI_SSID, WIFI_PASSWORD,
                              FIREBASE_HOST, FIREBASE_AUTH,
                              CLUSTER_ID, SENSOR_ID);
}

void loop() {
  iteration = (iteration < 1000) ? iteration+1 : 1;
  Serial.println("---------------" + String(iteration) +
                 " -> Acquiring BLE data----------------");
  String data = ble->getData();
  Serial.println("---------------" + String(iteration) +
                 " -> Sending to Firebase---------------");
  firebaseDB->sendData(data);
}
