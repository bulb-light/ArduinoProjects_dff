#ifndef __PID_H__
#define __PID_H__

#include <Arduino.h>

class PID {
    private:
        // pid errors
        float _error_cumulative = 0.0f;
        float _error_old = 0.0f;

        // limits
        float _max_pid = INFINITY;
        float _min_pid = -INFINITY;

    public:
        PID(float kp, float ki, float kd, float min_limit, float max_limit);
        ~PID();

        // pid params
        float _kp = 0.0f;
        float _ki = 0.0f;
        float _kd = 0.0f;

        // methods
        float pid_out(float error);
        void reset();
};

#endif