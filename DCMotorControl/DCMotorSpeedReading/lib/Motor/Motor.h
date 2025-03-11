#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <Arduino.h>

class Motor {
    private:
        uint8_t _pin1;
        uint8_t _pin2;
        uint8_t _pwmPin;

    public:
        Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmPin);
        ~Motor();
        // motor initialization
        void motorInit();

        // motor movements
        void forward(uint8_t pwm);
        void backward(uint8_t pwm);
        void stop();
};
#endif