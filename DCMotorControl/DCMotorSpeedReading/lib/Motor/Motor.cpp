#include "Motor.h"

Motor::Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmPin)
{
    this->_pin1 = pin1;
    this->_pin2 = pin2;
    this->_pwmPin = pwmPin;
}

Motor::~Motor()
{
}

void Motor::motorInit()
{
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_pwmPin, OUTPUT);
}

void Motor::forward(uint8_t pwm)
{
    analogWrite(_pwmPin, pwm);
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, LOW);
}

void Motor::backward(uint8_t pwm)
{
    analogWrite(_pwmPin, pwm);
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, HIGH);
}

void Motor::stop()
{
    analogWrite(_pwmPin, 0);
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
}
