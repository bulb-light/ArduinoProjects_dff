#ifndef __BANDIT_H__
#define __BANDIT_H__

#include <Arduino.h>

class Bandit {
    public:
        Bandit(int k_arms, float epsilon, float alpha, float init_estimations, int method);
        ~Bandit();

        // array estimations and action count array (internal)
        float _estimations[100];
        int _action_count[100];

        float _o = 0.0; // initial value for unbiased constant step size

        // user defined parameters
        int _k_arms; // number of arms
        float _epsilon;
        float _alpha;
        float _init_estimation;
        int _method; // 0: stationary method, 1: nonstationary method

        // internal parameters
        int _steps;

        // methods
        int actionSelection();
        float rewardFromError(float error,int action);
        int step(float error, int action);
        void reset();

};

#endif