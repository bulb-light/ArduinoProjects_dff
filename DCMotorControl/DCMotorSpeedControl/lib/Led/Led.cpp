#include "Led.h"

// constructor
Led::Led(uint8_t pin){
    this->pin = pin;
}

// deconstructor
Led::~Led(){
}

void Led::init(bool state){
    pinMode(pin, OUTPUT);
    if (state == HIGH)
        on();
    else
        off();
}

void Led::on(){
    digitalWrite(pin, HIGH);
}
void Led::off(){
    digitalWrite(pin, LOW);
}