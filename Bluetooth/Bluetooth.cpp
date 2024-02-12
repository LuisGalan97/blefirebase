#include "Bluetooth.h"

int Bluetooth::sizeCont = 0;
int Bluetooth::contData = 0;
String Bluetooth::data = "";
bool Bluetooth::ready = false;
double Bluetooth::arrayData[20];

Bluetooth::Bluetooth(BLEUUID serviceUUID, BLEUUID charUUID, int sizeCont) {
  this->serviceUUID = serviceUUID;
  this->charUUID = charUUID;
  Bluetooth::sizeCont = sizeCont;
  Serial.println("Starting Arduino BLE Client Application...");
  BLEDevice::init("");
  this->pBLEScan = BLEDevice::getScan();
  this->targetDevice = nullptr;
  this->advertCallback = new MyAdvertisedDeviceCallbacks(
                         this->serviceUUID,
                         this->pBLEScan);
  this->pClient = nullptr;
  this->clientCallback = new MyClientCallbacks();
  this->pRemoteServ = nullptr;
  this->pRemoteChar = nullptr;
  this->pBLEScan->setAdvertisedDeviceCallbacks(this->advertCallback);
  this->pBLEScan->setInterval(1349); //Intervalo de escaneo
  this->pBLEScan->setWindow(449);
  this->pBLEScan->setActiveScan(true); //Necesario para un escaneo profundo
}

String Bluetooth::getData() {
  while (!this->setData()) {
    delay(500);
  }
  return Bluetooth::data;
}

bool Bluetooth::setData() {
  if (!this->scan()) {
    delay(500);
    return false;
  }
  delay(500);
  if (!this->connect()) {
    delay(500);
    return false;
  }
  delay(500);
  if (!this->findService()) {
    delay(500);
    return false;
  }
  delay(500);
  if (!this->findCharacteristic()) {
    delay(500);
    return false;
  }
  delay(500);
  if (!this->startNotify()) {
    delay(500);
    return false;
  }
  while (!Bluetooth::ready &&
    this->clientCallback->getConnected()) {
    delay(10);
  }
  this->stopNotify();
  delay(500);
  this->disconnect();
  delay(500);
  if (!Bluetooth::ready) {
    return false;
  } else {
    return true;
  }
}

bool Bluetooth::scan() {
  Serial.println("Scanning BLE devices.");
  this->pBLEScan->start(0);
  if (this->targetDevice != nullptr) {
    delete this->targetDevice;
    this->targetDevice = nullptr;
  }
  if (this->targetDevice == nullptr) {
    this->targetDevice = new BLEAdvertisedDevice(
                         advertCallback->getTargetDevice());
  }
  return true;
}

bool Bluetooth::connect() {
  if (this->targetDevice != nullptr) {
    bool connected = this->clientCallback->getConnected();
    if (!connected) {
      this->clearInstances();
      if (this->pClient == nullptr) {
        Serial.print("Connecting to ");
        Serial.println(this->targetDevice->getAddress().toString().c_str());
        this->pClient  = BLEDevice::createClient();
        Serial.println(" - Created client");
        this->pClient->setClientCallbacks(this->clientCallback);
        bool success = this->pClient->connect(this->targetDevice);
        if (!success) {
          Serial.println("An error happened.");
          return false;
        } else {
          return true;
        }
      } else {
        Serial.println("Client already exist...");
        return false;
      }
    } else {
      Serial.println("Already connected...");
      return false;
    }
  } else {
    Serial.println("Target device doesn't exist...");
    return false;
  }
}

bool Bluetooth::findService() {
  if (this->pClient != nullptr) {
    bool connected = this->clientCallback->getConnected();
    if (connected) {
      if (this->pRemoteServ == nullptr) {
        this->pRemoteServ = pClient->getService(this->serviceUUID);
        if (this->pRemoteServ == nullptr) {
          Serial.print("Failed to find our Service UUID: ");
          Serial.println(this->serviceUUID.toString().c_str());
          return false;
        } else {
          Serial.println(" - Service Found");
          return true;
        }
      } else {
        Serial.println("Service already exist...");
        return false;
      }
    } else {
      Serial.println("Not connected to device...");
      return false;
    }
  } else {
    Serial.println("Client doesn't exist...");
    return false;
  }
}

bool Bluetooth::findCharacteristic() {
  if (this->pRemoteServ != nullptr) {
    bool connected = this->clientCallback->getConnected();
    if (connected) {
      if (this->pRemoteChar == nullptr) {
        this->pRemoteChar = this->pRemoteServ->getCharacteristic(
                            this->charUUID);
        if (this->pRemoteChar == nullptr) {
          Serial.print("Failed to find our characteristic UUID: ");
          Serial.println(this->charUUID.toString().c_str());
          return false;
        } else {
          Serial.println(" - Characteristic Found");
          return true;
        }
      } else {
         Serial.println("Characteristic already exist...");
         return false;
      }
    } else {
      Serial.println("Not connected to device...");
      return false;
    }
  } else {
    Serial.println("Service doesn't exist...");
    return false;
  }
}

bool Bluetooth::startNotify() {
  if (this->pRemoteChar != nullptr) {
    bool connected = this->clientCallback->getConnected();
    if (connected) {
      if (this->pRemoteChar->canNotify()) {
        Serial.println("Notifying characteristic!");
        Bluetooth::contData = 0;
        Bluetooth::data = "";
        Bluetooth::ready = false;
        this->pRemoteChar->registerForNotify(notifyCallback, this);
        return true;
      } else {
        Serial.println("Unable to notify...");
        return false;
      }
    } else {
      Serial.println("Not connected to device...");
      return false;
    }
  } else {
    Serial.println("Characteristic doesn't exist...");
    return false;
  }
}

bool Bluetooth::stopNotify() {
  if (this->pRemoteChar != nullptr) {
    bool connected = this->clientCallback->getConnected();
    if (connected) {
      if (this->pRemoteChar->canNotify()) {
        Serial.println("Denotifying characteristic!");
        this->pRemoteChar->registerForNotify(nullptr);
        return true;
      } else {
        Serial.println("Unable to denotify...");
        return false;
      }
    } else {
      Serial.println("Not connected to device...");
      return false;
    }
  } else {
    Serial.println("Characteristic doesn't exist...");
    return false;
  }
}

bool Bluetooth::disconnect() {
  if (this->targetDevice != nullptr) {
    bool connected = this->clientCallback->getConnected();
    if (connected) {
      Serial.print("Disconnecting from ");
      Serial.println(this->targetDevice->getAddress().toString().c_str());
      this->pClient->disconnect();
      while(this->clientCallback->getConnected()){
        delay(10);
      }
      return true;
    } else {
      Serial.println("Already disconnected...");
      return false;
    }
  } else {
    Serial.println("Target device doesn't exist, please find it...");
    return false;
  }
}

void Bluetooth::clearInstances() {
  if (this->pClient != nullptr) {
    delete this->pClient;
    this->pClient = nullptr;
  }
  if (this->pRemoteServ != nullptr) {
    this->pRemoteServ = nullptr;
  }
  if (this->pRemoteChar !=nullptr) {
    this->pRemoteChar = nullptr;
  }
  delay(500);
}

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  if (isNotify) {
    if (!Bluetooth::ready) {
      if (Bluetooth::contData < 80) {
        for (size_t i = 0; i < length; i++) {
          if (Bluetooth::contData == 0) {
            Bluetooth::arrayData[i] = pData[i];
          } else {
            Bluetooth::arrayData[i] += pData[i];
          }
        }
        Bluetooth::contData++;
      } else {
        for (int i = 0; i < length; i++) {
          Bluetooth::arrayData[i] = Bluetooth::arrayData[i] /
                                    Bluetooth::contData;
          String tempData = String((int)round(Bluetooth::arrayData[i]), HEX);
          Bluetooth::arrayData[i] = 0;
          if (tempData.length() < 2) {
            Bluetooth::data += "0" + tempData;
          } else {
            Bluetooth::data += tempData;
          }
        }
        Serial.println("Completando adquisicion de datos...");
        Bluetooth::ready = true;
      }
    }
  }
}