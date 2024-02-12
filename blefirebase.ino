#include "Bluetooth/Bluetooth.h"
#include "Bluetooth/Bluetooth.cpp"
#include "Bluetooth/MyAdvertisedDeviceCallbacks.cpp"
#include "Bluetooth/MyClientCallbacks.cpp"


static BLEUUID serviceUUID("0000FFE5-0000-1000-8000-00805F9A34FB");
static BLEUUID charUUID("0000FFE4-0000-1000-8000-00805F9A34FB");
static int sizeCont = 80;
Bluetooth* ble;

void setup() {
  Serial.begin(115200);
  ble = new Bluetooth(serviceUUID, charUUID, sizeCont);
  delay(5000);
  Serial.println(ble->getData());
}

void loop() {

}
