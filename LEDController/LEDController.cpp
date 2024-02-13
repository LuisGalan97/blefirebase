#include "LEDController.h"

static LEDController ledCtrl;

bool LEDController::statusLED_1 = false;
bool LEDController::statusLED_2 = false;
bool LEDController::statusLED_3 = false;

LEDController::LEDController() {
  ledsConfig();
}

void LEDController::ledsConfig() {
  ledcSetup(ledChannel_1, freq, resolution);
  ledcSetup(ledChannel_2, freq, resolution);
  ledcSetup(ledChannel_3, freq, resolution);
  ledcAttachPin(ONBOARD_LED_1, ledChannel_1);
  ledcAttachPin(ONBOARD_LED_2, ledChannel_2);
  ledcAttachPin(ONBOARD_LED_3, ledChannel_3);
}

void LEDController::alternateLED_1() {
  if (!statusLED_1) {
    ledcWrite(ledChannel_1, intensity);
    statusLED_1 = true;
  } else {
    ledcWrite(ledChannel_1, 0);
    statusLED_1 = false;
  }
}
void LEDController::alternateLED_2() {
  if (!statusLED_2) {
    ledcWrite(ledChannel_2, intensity);
    statusLED_2 = true;
  } else {
    ledcWrite(ledChannel_2, 0);
    statusLED_2 = false;
  }
}

void LEDController::alternateLED_3() {
  if (!statusLED_3) {
    ledcWrite(ledChannel_3, intensity);
    statusLED_3 = true;
  } else {
    ledcWrite(ledChannel_3, 0);
    statusLED_3 = false;
  }
}

void LEDController::setLED_1(bool status) {
  if (status) {
    if (!statusLED_1) {
      alternateLED_1();
    }
  } else {
    if (statusLED_1) {
      alternateLED_1();
    }
  }
}

void LEDController::setLED_2(bool status) {
  if (status) {
    if (!statusLED_2) {
      alternateLED_2();
    }
  } else {
    if (statusLED_2) {
      alternateLED_2();
    }
  }
}

void LEDController::setLED_3(bool status) {
  if (status) {
    if (!statusLED_3) {
      alternateLED_3();
    }
  } else {
    if (statusLED_3) {
      alternateLED_3();
    }
  }
}