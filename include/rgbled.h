#pragma once

#include <Arduino.h>

class RGBColor {
    public:
    float r, b, g;
    RGBColor(float R, float G, float B);
    RGBColor();
    RGBColor blend(RGBColor otherColor, float factor);
    RGBColor applyBrigthness(float brightness);
};

class RGBLed
{
private:
    int _pinR, _pinG, _pinB;
    volatile int _freq;
    unsigned long _lastChange;
    bool _state;
    int _mode;
    // modes explained:
        // 0 = off
        // 1 = on
        // 2 = blinking (alternating between current color and off state)
        // 3 = unproportional blinking
        // 4 = alternation beween current and secondary color, blinking
        // 5 = gradient alternation animation (you can set secondary color to black/zero)
    float _brightness;
    unsigned long _millisStart;
    unsigned long _microsStart;
    unsigned int _animationCycleTime;
    

public:
    RGBLed(int pinR, int pinG, int pinB);
    RGBColor _currentColor;
    RGBColor _secondaryColor;
    void setBlinking(int freq=1);
    void setColor(RGBColor color, bool isSeconndary = false);
    void outputBrightnessToPin(int pin, float brightness, unsigned long microsSync);
    void writeColor(RGBColor color);
    void displayColorBeforeBlockingOperation(bool r, bool g, bool b);
    void setOn(bool on=true);
    void setBrightness(float brightness);
    void setAnimateBrightness(unsigned long cycleTime = 1000);
    void setAlternatingColors(int freq=1);
    void setGradientAnimation(unsigned long cycleTime = 1000);
    //void attachToTimer(void);
    //void detachFromTimer(void);
    int begin(void);
    int cycle(void);
    int end(void);
    
 };