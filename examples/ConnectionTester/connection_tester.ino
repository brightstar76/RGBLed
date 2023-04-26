/*
  This sample program will help you test if your RGB LED works properly and is properly connected.

  Redefine PIN_RED, PIN_GREEN and PIN_BLUE to wherever your RGB LED pins are connected.

  Once you upload the sketch and run it, it should do the following in a loop, one second per step
    1) blink in white
    2) emit constant red light
    3) emit constant green light
    4) emit constant blue light

  If this sketch works as described, congratulations! Your diode is connected correctly and functioning!

*/

#include <rgbled.h>

#define PIN_RED 10
#define PIN_GREEN 11
#define PIN_BLUE 12

RGBLed rgb(PIN_RED, PIN_GREEN, PIN_BLUE);

void setup()
{
  rgb.begin();
}



unsigned long lastChange = 0;
int mode = -1;

void loop()
{
    if (millis() > lastChange + 1000 || mode == -1)
    {
      mode = (mode + 1)%4;
      lastChange = millis();
      switch (mode)
      {
      case 0:
        rgb.setColor(RGBColor(1.0, 1.0, 1.0));
        rgb.setBlinking(8);
        break;
      case 1:
        rgb.setColor(RGBColor(1.0, 0.0, 0.0));
        rgb.setOn();
        break;
      case 2:
        rgb.setColor(RGBColor(0.0, 1.0, 0.0));
        rgb.setOn();
        break;
      case 3:
        rgb.setColor(RGBColor(0.0, 0.0, 1.0));
        rgb.setOn();
        break;
      }
    }

    rgb.cycle();
}