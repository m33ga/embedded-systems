#ifndef DD_MOTOR_H
#define DD_MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(uint8_t pinA, uint8_t pinB);
    void begin();
    void setSpeed(int speed);
    void stop();
    int getSpeed();
    int getDirection();
private:
    uint8_t pinA;
    uint8_t pinB;
    int currentSpeed; // -255..+255
};

#endif
