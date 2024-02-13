#ifndef LEDController_h
#define LEDController_h

class LEDController {
  private:
    static const int ONBOARD_LED_1 = 23;
    static const int ONBOARD_LED_2 = 19;
    static const int ONBOARD_LED_3 = 13;
    static const int ledChannel_1 = 0;
    static const int ledChannel_2 = 1;
    static const int ledChannel_3 = 2;
    static const int freq = 5000;
    static const int resolution = 8;//256 resolution 8bits
    static const int intensity = 1; //0-255
    static bool statusLED_1;
    static bool statusLED_2;
    static bool statusLED_3;
    static void ledsConfig();
  public:
    LEDController();
    static void alternateLED_1();
    static void alternateLED_2();
    static void alternateLED_3();
    static void setLED_1(bool status);
    static void setLED_2(bool status);
    static void setLED_3(bool status);
};

#endif