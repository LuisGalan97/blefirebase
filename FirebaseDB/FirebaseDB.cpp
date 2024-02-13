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
  this->timedelay = 250;
  Firebase.reconnectWiFi(false);
}

void FirebaseDB::sendData(String data){
  delay(this->timedelay);
  while (!this->setData(data)) {
    delay(this->timedelay);
  }
}

bool FirebaseDB::setData(String data) {
  if (!this->connectWiFi()) {
    delay(this->timedelay);
    return false;
  }
  delay(this->timedelay);
  if (!this->connectFirebase()) {
    delay(this->timedelay);
    return false;
  }
  delay(this->timedelay);
  if (!this->readBoolData(this->node + "connection")) {
    delay(this->timedelay);
    return false;
  } else {
    if (!this->boolData) {
      Serial.println("Writing inactive from firebase.");
      delay(this->timedelay);
      return false;
    }
  }
  delay(this->timedelay);
  if (!this->sendStringData(this->node + "data", data)) {
    delay(this->timedelay);
    return false;
  }
  delay(this->timedelay);
  if (!this->disconnectWiFi()) {
    delay(this->timedelay);
    return false;
  }
  delay(this->timedelay);
  return true;
}

bool FirebaseDB::connectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to network ");
    Serial.println((String)this->wiFiSSID);
    WiFi.begin(this->wiFiSSID, this->wiFiPass);
    int timeout = 0;
    while(WiFi.status() != WL_CONNECTED &&
      timeout < 3000){
      delay(10);
      timeout += 1;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Connected to network ");
      Serial.println((String)this->wiFiSSID + "!");
      this->firebaseConn = false;
      return true;
    } else {
      Serial.print("Unable to connect network ");
      Serial.println((String)this->wiFiSSID + "...");
      return false;
    }
  } else {
    Serial.println("WiFi connection already exist...");
    return true;
  }
}

bool FirebaseDB::disconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Disconnecting from WiFi.");
    WiFi.disconnect();
    int timeout = 0;
    while(WiFi.status() == WL_CONNECTED &&
      timeout < 3000){
      delay(10);
      timeout += 1;
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected!");
      return true;
    } else {
      Serial.println("Unable to disconnect WiFi...");
      return false;
    }
  } else {
    Serial.println("WiFi connection doesn't exist...");
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
    return false;
  }
}