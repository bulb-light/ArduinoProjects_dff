#ifndef __LED_H__
#define __LED_H__

#include <Arduino.h>

class Led {
    private:
        uint8_t pin;
    public:
        Led(uint8_t pin);
        ~Led(void);

        void init(bool state);
        void on();
        void off();
};

#endif