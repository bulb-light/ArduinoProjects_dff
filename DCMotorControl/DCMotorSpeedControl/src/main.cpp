#include <Arduino.h>
#include <util/atomic.h>
#include <Motor.h>
#include <MagneticEncoder.h>
#include <Bandit.h>
#include <PID.h>

// This is the first test of the PID and k arms bandit algorithm

// motor pins
#define IN1 8
#define IN2 7
#define ENA 10
// encoder pins
#define ENCA 3
#define ENCB 2

// globals speed measurement method 1
volatile int pos_i = 0;

// instantiate motor object
Motor myMotor(IN1, IN2, ENA);

// instantiate bandit object
int k_arms = 100;
float eps = 0.0;
float alpha = 0.01;
float inital_est = -100.0;
Bandit myBandit(k_arms, eps, alpha, inital_est, 1);

// instantiate PID object (kp, ki, kd, min_lim, max_lim)
PID speedPI(0.1, 0.00, 0.0, 0.0, 255.0);

// pi - ki options
float ki[100];
int pi_pwm = 0;

// control vars
float w_ref_rpm = 350.0; // setpoint
float velocity_rpm = 0.0; // current velocity
int action = 0;

// timer control
long prev_millis = 0;
long interval = 500; // 500ms interval

// methods declaration
void readEncoder();

// -- MAIN -- //
void setup() {
  Serial.begin(9600);
  
  myMotor.motorInit();
  myMotor.backward(0);

  // reset bandit
  myBandit.reset();

  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB), readEncoder, RISING);

  // pid - ki options
  for (int i = 0; i < k_arms; i++)
  {
    ki[i] = 0.001 * (i);
  }

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  pinMode(A0, INPUT);
  randomSeed(analogRead(A0));
}

void loop() {

  // read the position in an atomic block
  // to avoid potential misreads
  int pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = pos_i;
  }

  // setpoint from serial port
  if (Serial.available() > 0)
  {
    String w_ref_serial = Serial.readString();
    w_ref_rpm = w_ref_serial.toFloat();
  }
  // getting speed
  velocity_rpm = readSpeed(pos);
  float w_error_rpm = w_ref_rpm - velocity_rpm;

  // timing for motor stabilization
  long current_millis = millis();

  if ((current_millis - prev_millis) > interval){
    action = myBandit.step(w_error_rpm, action);
    // speedPI._ki = ki[action];
    prev_millis = current_millis;
  }

  myBandit.step(w_error_rpm, action);
  speedPI._ki = ki[action];
  pi_pwm = (int) speedPI.pid_out(w_error_rpm);

  myMotor.backward(pi_pwm);

  Serial.print(ki[action]);
  Serial.print(" ");

  Serial.print(velocity_rpm);
  Serial.print(" ");
  Serial.println(w_ref_rpm); 

  delay(10);
}

void readEncoder() {
  pos_i ++;
}