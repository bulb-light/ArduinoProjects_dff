#include "PID.h"

PID::PID(float kp, float ki, float kd, float min_limit, float max_limit)
{
    this->_kp = kp;
    this->_ki = ki;
    this->_kd = kd;

    this->_min_pid = min_limit;
    this->_max_pid = max_limit;
}

PID::~PID()
{
}

float PID::pid_out(float error)
{
    float pid_out = (this->_kp * error) + (this->_ki * this->_error_cumulative)
                    + (this->_kd * (error - this->_error_old));
    this->_error_cumulative += error;
    this->_error_old = error;

    // saturation
    if (pid_out > this->_max_pid)
    {
        pid_out = this->_max_pid;
    } else if (pid_out < this->_min_pid)
    {
        pid_out = this->_min_pid;
    }

    return pid_out;
}

void PID::reset()
{
    this->_error_old = 0;
    this->_error_cumulative = 0;
}
