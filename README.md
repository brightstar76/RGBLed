# RGBLed

A library for Arduino to control RGB Led.

Uses PWM (pulse width modulation) on any digital output pin to control brigthness of each one of the 3 colors.

Features modes such as being constanctly on in any color of the RGB palette a a given brigthness, blinking and alternating between two colors at specified frequency.

Also features a sine wave gradient transition, blending between two colors. Example:

```C++
#include <rgbled.h>

#define PIN_RED 10
#define PIN_GREEN 11
#define PIN_BLUE 12

RGBLed rgb(PIN_RED, PIN_GREEN, PIN_BLUE);

void setup()
{
 rgb.begin();
 rgb.setColor(RGBColor(1.0, 0.0, 0.0));
 rgb.setColor(RGBColor(0.0, 1.0, 0.0), true);
 rgb.setGradientAnimation(3000);
}

void loop()
{
  rgb.cycle();
}

```
