#include "rgbled.h"

RGBColor::RGBColor(float valR, float valG, float valB)
{
    r = valR;
    g = valG;
    b = valB;
}

RGBColor::RGBColor()
{
    r = 0.0;
    g = 0.0;
    b = 0.0;
}

unsigned long millisFromMicros(void)
{
    return micros()/1000;
}

RGBColor RGBColor::blend(RGBColor otherColor, float factor)
{
    return RGBColor(r * factor + otherColor.r * (1.0 - factor),
                    g * factor + otherColor.g * (1.0 - factor),
                    b * factor + otherColor.b * (1.0 - factor));
}

RGBColor RGBColor::applyBrigthness(float brightness)
{
    return RGBColor(r * brightness, g * brightness, b * brightness);
}

RGBLed::RGBLed(int pinR, int pinG, int pinB)
{
    _pinR = pinR;
    _pinG = pinG;
    _pinB = pinB;
    _currentColor = RGBColor(1.0, 1.0, 1.0);
    _brightness = 1.0;
    
}

void RGBLed::setColor(RGBColor color, bool isSecondary)
{
    if (!isSecondary)
        _currentColor = color;
    else
        _secondaryColor = color;
}

void RGBLed::setBlinking(int freq)
{
    setColor(RGBColor(0.0, 0.0, 0.0), true);
    _mode = 2;
    _freq = freq;
    _lastChange = millisFromMicros();
}

void RGBLed::setGradientAnimation(unsigned long cycleTime)
{
    _mode = 5;
    _animationCycleTime = cycleTime;
    _millisStart = millisFromMicros();
    _microsStart = micros();
}

void RGBLed::displayColorBeforeBlockingOperation(bool r, bool g, bool b)
{
    digitalWrite(_pinR, r ? HIGH : LOW);
    digitalWrite(_pinG, g ? HIGH : LOW);
    digitalWrite(_pinB, b ? HIGH : LOW);
}

int RGBLed::begin(void)
{
    RGBLed *self = this;

    pinMode(self->_pinR, OUTPUT);
    pinMode(self->_pinG, OUTPUT);
    pinMode(self->_pinB, OUTPUT);

    

    return 0;
}

int RGBLed::end(void)
{
    return 0;
}

void RGBLed::writeColor(RGBColor color)
{
    unsigned long microsSync = micros();
    outputBrightnessToPin(_pinR, color.r, microsSync);
    outputBrightnessToPin(_pinG, color.g, microsSync);
    outputBrightnessToPin(_pinB, color.b, microsSync);
}



void RGBLed::outputBrightnessToPin(int pin, float brightness, unsigned long microsSync)
{
    bool shouldBeHigh;

    unsigned long cycleSize = 20 * 1000;
    unsigned long phase = (microsSync) % cycleSize;
    shouldBeHigh = (brightness * cycleSize > phase);

    digitalWrite(pin, shouldBeHigh ? HIGH : LOW);
}

void RGBLed::setAlternatingColors(int freq)
{
    _freq = freq;
    _mode = 2;
    _lastChange = millisFromMicros();
}

void RGBLed::setBrightness(float brightness)
{
    _brightness = brightness;
}

void RGBLed::setOn(bool isOn)
{
    _mode = isOn ? 1 : 0;
}

void RGBLed::setAnimateBrightness(unsigned long cycleLengthMs)
{
    setColor(RGBColor(0.0, 0.0, 0.0), true);
    _animationCycleTime = cycleLengthMs;
    _mode = 5;
}


int RGBLed::cycle(void)
{
    RGBLed *self = this;

    switch (self->_mode)
    {
    case 0:
        self->writeColor(RGBColor(0.0, 0.0, 0.0));
        break;
    case 1:
        self->writeColor(self->_currentColor.applyBrigthness(_brightness));
        break;
    case 2:
        if (self->_lastChange > millisFromMicros())  // the micros counter has overflown
            self->_lastChange = millisFromMicros();  // so reset the last change in milliseconds

        if (self->_lastChange + (1000 / self->_freq) < millisFromMicros())
        {
            self->_state = !self->_state;
            self->_lastChange = millisFromMicros();
        }
        self->writeColor(self->_state ? self->_currentColor : self->_secondaryColor);
        break;
    case 4:
        if (self->_lastChange > millisFromMicros())  // the micros counter has overflown
            self->_lastChange = millisFromMicros();  // so reset the last change in milliseconds
            
        if (self->_lastChange + (1000 / self->_freq) < millisFromMicros())
        {
            self->_state = !self->_state;
            self->_lastChange = millisFromMicros();
        }
        self->writeColor(self->_state ? self->_currentColor : self->_secondaryColor);
        break;

    case 5:
        // blend between colors running a gradient there and back
        float angle = (1 / (float)self->_animationCycleTime) * PI * (micros() / 1000);
        float factor = sinf(angle) * 0.5 + 0.5;
        //Serial.println(factor);
        self->writeColor(self->_currentColor.blend(self->_secondaryColor, factor));
        break;
    }
    return 0;
}