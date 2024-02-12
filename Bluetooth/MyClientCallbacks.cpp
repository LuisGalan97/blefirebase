#include "MyClientCallbacks.h"

MyClientCallbacks::MyClientCallbacks() {
  this->connected = false;
}

void MyClientCallbacks::onConnect(BLEClient* pclient) {
  Serial.println("Connected to BLE Device!");
  this->connected = true;
}

void MyClientCallbacks::onDisconnect(BLEClient* pclient) {
  Serial.println("Disconnected from BLE Device...");
  this->connected = false;
}

bool MyClientCallbacks::getConnected() {
  return this->connected;
}