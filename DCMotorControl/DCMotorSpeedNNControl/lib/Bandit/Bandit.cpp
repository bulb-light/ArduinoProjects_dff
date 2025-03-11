#include "Bandit.h"
#include "Algorithms.h"

Bandit::Bandit(int k_arms, float epsilon, float alpha, float init_estimations, int method)
{
    this->_k_arms = k_arms;
    this->_epsilon = epsilon;
    this->_alpha = alpha;
    this->_init_estimation = init_estimations;
    this->_method = method;

    this->_steps = 0;
}

Bandit::~Bandit()
{
}

int Bandit::actionSelection()
{
    // exploration eps-greedy
    if (((double) random(0, RAND_MAX) / RAND_MAX) < this->_epsilon)
    {
        return random(0, this->_k_arms);
    }
    int action = 0;
    // greedy action
    maxElement(this->_estimations, this->_k_arms, action);
    return action;
}

float Bandit::rewardFromError(float error, int action)
{
    // 12 -> means a 12V DC motor
    // _k_arms = 255 pwm resolution
    // return - ( pow(error, 2) + 0.01 * pow(((float) action * (12.0 / this->_k_arms)), 2) );

    // if ((abs(error) > 10))
    // {
    //     return -100000.0;
    // }

    // NOTE: This is working fine  
    return -pow(error, 2) + 10.0 * exp(- pow(error/10, 2)) ;
}

int Bandit::step(float error, int action)
{
    // int action1 = this->actionSelection();
    float reward = this->rewardFromError(error, action);

    // this->_steps += 1;
    this->_action_count[action] += 1;

    if (this->_method == 0)
    {
        this->_estimations[action] += (reward - this->_estimations[action]) / ((float) this->_action_count[action]);
    } else if (this->_method == 1) {
        // Uncomment the following lines for unbiased constant step size method
        this->_o = this->_o + this->_alpha * (1 - this->_o); 
        float beta = this->_alpha/this->_o;
        this->_estimations[action] += (reward - this->_estimations[action]) * beta + log(this->_action_count[action]);
        action = this->actionSelection();

        // Uncomment the following line for constant step size method
        // this->_estimations[action] += (reward - this->_estimations[action]) * this->_alpha;
    }
    
    return action;
}

void Bandit::reset()
{
    for (int i = 0; i < this->_k_arms; i++)
    {
        this->_estimations[i] = this->_init_estimation;
        // this->_estimations[i] = 0.0;
        this->_action_count[i] = 0;
    }
    this->_steps = 0;   
}
