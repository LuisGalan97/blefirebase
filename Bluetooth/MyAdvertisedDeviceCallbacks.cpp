#include "MyAdvertisedDeviceCallbacks.h"

MyAdvertisedDeviceCallbacks::MyAdvertisedDeviceCallbacks(
  BLEUUID serviceUUID, BLEScan* pBLEScan) {
  this->serviceUUID = serviceUUID;
  this->pBLEScan = pBLEScan;
  this->targetDevice = nullptr;
}

void MyAdvertisedDeviceCallbacks::onResult(
  BLEAdvertisedDevice advertisedDevice) {
  //Serial.print("BLE Advertised Device found: ");
  //Serial.println(advertisedDevice.toString().c_str());
  if (advertisedDevice.haveServiceUUID() &&
    advertisedDevice.isAdvertisingService(serviceUUID)) {
    Serial.println("Device found, preparing connection...");
    if (this->targetDevice != nullptr){
      delete this->targetDevice;
      this->targetDevice = nullptr;
    }
    if (this->targetDevice == nullptr) {
      this->targetDevice = new BLEAdvertisedDevice(advertisedDevice);
      pBLEScan->stop();
    }
  }
}

BLEAdvertisedDevice MyAdvertisedDeviceCallbacks::getTargetDevice() {
  return *this->targetDevice;
}