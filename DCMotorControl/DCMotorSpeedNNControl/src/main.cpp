#include <Arduino.h>
#include <util/atomic.h>
#include <Motor.h>
#include <MagneticEncoder.h>
// #include <Bandit.h>
#include <MatrixMath.h>
#include <PID.h>

// This is the first test of the NN control

// motor pins
#define IN1 8
#define IN2 7
#define ENA 10
// encoder pins
#define ENCA 3
#define ENCB 2

// pwm and rpm-speed max values
#define MAX_RPM_VALUE 573.0
#define MAX_PWM 255.0

// globals speed measurement method 1
volatile int pos_i = 0;

// instantiate motor object
Motor myMotor(IN1, IN2, ENA);

// instantiate PID object (kp, ki, kd, Ts, min_lim, max_lim)
PID speedPI(0.01, 0.001, 0.0, 0.010, 0.0, 255.0);

// control vars
float w_ref_rpm = 350.0; // setpoint
float velocity_rpm = 0.0; // current velocity
int pwm = 0;

// timer control
long prev_millis = 0;
long prev_millis_speed = 0;
long interval_speed = 20; //10ms interval for speed reading
long interval = 10; // 100ms interval

/////////////////////////////
// Neural Netowork matrices//
/////////////////////////////
// last element of the input vector represents the input bias
mtx_type input[2] = {0, 1}; // layer 0

mtx_type hidden_weights1[3][2] = {{-3.0345596, 2.90394588},
                                  {-1.0976389 , -0.70886087},
                                  {-0.7685747 , -0.42865463}
                                };

mtx_type out1[3];
// ---------------
mtx_type input2[4] = {0, 0, 0, 1};

mtx_type hidden_weights2[4][4] = {{-0.68284827, -0.21032326, -0.1178421 , -0.02764689},
                                  {-0.3283163 ,  0.18387903, -0.80329341,  0.94183746},
                                  {-4.03018258,  0.55826217, -0.10950553, -0.24412867},
                                  {-0.57183908, -0.56724367,  0.50227013,  1.2603674 }};

mtx_type out2[4];

// -------------------
mtx_type input3[5] = {0, 0, 0, 0, 1};

mtx_type output_weights[5] = {0.05829049, 1.36496838, 2.20687278, 1.62508009, 0.29592965};

mtx_type out[1];

// inputs for test
float intputs[] = {0.0,
0.0, 
0.0,
0.0,
0.0,
0.0,
0.373472949,
0.472949389,
0.547993019,
0.621291449,
0.668411867,
0.705061082,
0.741710297,
0.778359511,
0.802792321,
0.830715532,
0.851657941,
0.872600349,
0.890052356,
0.917975567,
0.919720768,
0.919720768,
0.919720768,
0.945898778,
0.959860384,
0.972076789,
1.0};

// methods declaration
void readEncoder();

// feedForward funciton
void feedForward(float x);

// -- MAIN -- //
void setup() {
  Serial.begin(115200);
  
  myMotor.motorInit();
  myMotor.backward(0);

  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB), readEncoder, RISING);
  
}

void loop() {
  // get elapsed time
  long current_millis = millis();

  // setpoint from serial port
  if (Serial.available() > 0)
  {
    String w_ref_serial = Serial.readString();
    w_ref_rpm = w_ref_serial.toFloat();
  }

  // read the position in an atomic block
  // to avoid potential misreads
  int pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = pos_i;
  }
  
  // getting speed
  if ((current_millis - prev_millis_speed) > interval_speed){
    velocity_rpm = readSpeed(pos);
    prev_millis_speed = current_millis;
  }

  // calculate pwm from the neural network
  feedForward(w_ref_rpm / MAX_RPM_VALUE);
  pwm = (int) (out[0] * MAX_PWM);

  myMotor.backward(pwm);
  // speed error
  float w_error_rpm = w_ref_rpm - velocity_rpm;
  // pwm = pwm + (int) speedPI.pid_out(w_error_rpm);
  if ((current_millis - prev_millis) > interval)
  {
    pwm = pwm + (int) speedPI.pid_out(w_error_rpm);
  }

  // if ((current_millis - prev_millis) > interval)
  // {
    
  //   prev_millis = current_millis;
  // }
  // Serial.print((float) current_millis);
  // Serial.print(" ");
  Serial.print(pwm);
  Serial.print(" ");
  Serial.print(w_ref_rpm);
  Serial.print(" ");  
  Serial.println(velocity_rpm);

  // delay(10);
}

void readEncoder() {
  pos_i ++;
}

void feedForward(float x)
{
  input[0] = x;

  // hidden layer (1)
  Matrix.Multiply((mtx_type*)hidden_weights1, (mtx_type*) input, 3, 2, 1, (mtx_type*) out1);

  for (int i = 0; i < 3; i++)
  {
    out1[i] = tanh(out1[i]);
    input2[i] = out1[i];
  }

  // hidden layer (2)
  Matrix.Multiply((mtx_type*)hidden_weights2, (mtx_type*) input2, 4, 4, 1, (mtx_type*) out2);

  for (int i = 0; i < 4; i++)
  {
    out2[i] = tanh(out2[i]);
    input3[i] = out2[i];
  }
  
  Matrix.Multiply((mtx_type*)output_weights, (mtx_type*) input3, 1, 5, 1, (mtx_type*) out);
  out[0] = tanh(out[0]);
}
