#include "FirebaseDB.h"

FirebaseDB::FirebaseDB(const char* wiFiSSID, const char* wiFiPass,
  const char* fireHost, const char* fireAuth,
  String clusterID, String sensorID) {
  Serial.println("Starting Arduino Firebase Application...");
  this->wiFiSSID = wiFiSSID;
  this->wiFiPass = wiFiPass;
  this->fireHost = fireHost;
  this->fireAuth = fireAuth;
  this->node = clusterID + "/" + sensorID + "/";
  this->firebaseConn = false;
  this->boolData = false;
  this->timedelay = 200;
  this->attemps = 0;
  Firebase.reconnectWiFi(false);
}

void FirebaseDB::sendData(String data){
  this->attemps = 0;
  this->customDelay();
  while (!this->setData(data)) {
    if (this->attemps < 5) {
      this->attemps += 1;
      if (this->attemps > 1) {
        Serial.print("Firebase sendData attempt: ");
        Serial.println(this->attemps);
      }
      this->customDelay();
    } else {
      Serial.println("Attemps exceeded, rebooting...");
      this->customDelay();
      ESP.restart();
    }
  }
}

bool FirebaseDB::setData(String data) {
  LEDController::setLED_3(true);
  if (!this->scanWiFi()) {
    this->attemps = 0;
    this->customDelay();
    return false;
  }
  this->customDelay();
  if (!this->connectWiFi()) {
    this->customDelay();
    return false;
  }
  this->customDelay();
  if (!this->connectFirebase()) {
    this->customDelay();
    return false;
  }
  this->customDelay();
  LEDController::setLED_3(true);
  if (!this->readBoolData(this->node + "connection")) {
    this->customDelay();
    return false;
  } else {
    if (!this->boolData) {
      Serial.println("Writing inactive from firebase.");
      this->attemps = 0;
      this->customDelay();
      return false;
    }
  }
  this->customDelay();
  LEDController::setLED_3(true);
  if (!this->sendStringData(this->node + "data", data)) {
    this->customDelay();
    return false;
  }
  this->customDelay();
  if (!this->disconnectWiFi()) {
    this->customDelay();
    return false;
  }
  this->customDelay();
  return true;
}

bool FirebaseDB::scanWiFi(){
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Scannning available networks");
    int value = WiFi.scanNetworks();
    for (int i = 0; i < value; i++) {
      if ((i+1)%4 == 0) {
        LEDController::alternateLED_3();
      }
      delay(50);
      if ((String)this->wiFiSSID == WiFi.SSID(i)) {
        Serial.print("Found network ");
        Serial.println((String)this->wiFiSSID + "!");
        return true;
      }
    }
    Serial.print("Not found network ");
    Serial.println((String)this->wiFiSSID + "...");
    return false;
  } else {
    Serial.println("Unable to scan, WiFi connection already exist...");
    LEDController::setLED_2(true);
    return true;
  }
}

bool FirebaseDB::connectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to network ");
    Serial.println((String)this->wiFiSSID);
    WiFi.begin(this->wiFiSSID, this->wiFiPass);
    int timeout = 0;
    while(WiFi.status() != WL_CONNECTED &&
      timeout < 150){
      LEDController::alternateLED_3();
      delay(100);
      timeout += 1;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Connected to network ");
      Serial.println((String)this->wiFiSSID + "!");
      LEDController::setLED_2(true);
      this->firebaseConn = false;
      return true;
    } else {
      Serial.print("Unable to connect network ");
      Serial.println((String)this->wiFiSSID + "...");
      return false;
    }
  } else {
    Serial.println("WiFi connection already exist...");
    LEDController::setLED_2(true);
    return true;
  }
}

bool FirebaseDB::disconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Disconnecting from WiFi.");
    WiFi.disconnect();
    int timeout = 0;
    while(WiFi.status() == WL_CONNECTED &&
      timeout < 150){
      LEDController::alternateLED_3();
      delay(100);
      timeout += 1;
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected!");
      LEDController::setLED_2(false);
      return true;
    } else {
      Serial.println("Unable to disconnect WiFi...");
      return false;
    }
  } else {
    Serial.println("WiFi connection doesn't exist...");
    LEDController::setLED_2(false);
    return true;
  }
}

bool FirebaseDB::connectFirebase() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!this->firebaseConn) {
      Firebase.begin(this->fireHost, this->fireAuth);
      this->firebaseConn = true;
      return true;
    } else {
      Serial.println("Firebase connection already exist...");
      return true;
    }
  } else {
    Serial.println("WiFi connection doesn't exist...");
    LEDController::setLED_2(false);
    return false;
  }
}

bool FirebaseDB::readBoolData(String target) {
  if (WiFi.status() == WL_CONNECTED) {
    if (this->firebaseConn) {
      Serial.println("Reading bool data from Firebase.");
      if (Firebase.getBool(this->firebaseData, target)) {
        this->boolData = this->firebaseData.boolData();
        Serial.print("Bool data read succesfully! ");
        if (this->boolData) {
          Serial.println("Value: True");
        } else {
          Serial.println("value: False");
        }
        return true;
      } else {
        Serial.println("Unable to read bool data...");
        return false;
      }
    } else {
      Serial.println("Firebase connection doesn't exist...");
      return false;
    }
  } else {
    Serial.println("WiFi connection doesn't exist...");
    LEDController::setLED_2(false);
    return false;
  }
}

bool FirebaseDB::sendStringData(String target, String data) {
  if (WiFi.status() == WL_CONNECTED) {
    if (this->firebaseConn) {
      Serial.println("Sending data to Firebase.");
      if (Firebase.setString(this->firebaseData, target, data)) {
        Serial.print("Data " + data);
        Serial.println(" sent succesfully!");
        return true;
      } else {
        Serial.print("Unable to send data ");
        Serial.println(data + "...");
        return false;
      }
    } else {
      Serial.println("Firebase connection doesn't exist...");
      return false;
    }
  } else {
    Serial.println("WiFi connection doesn't exist...");
    LEDController::setLED_2(false);
    return false;
  }
}

void FirebaseDB::customDelay() {
  LEDController::alternateLED_3();
  delay(this->timedelay);
}