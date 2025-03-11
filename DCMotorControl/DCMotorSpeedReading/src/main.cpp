#include <Arduino.h>
#include <util/atomic.h>
#include <Motor.h>
#include <MagneticEncoder.h>

// This is speed reading of the DC motor

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

float velocity_rpm = 0.0; // current velocity
int pwm = 0; // motor pwm

// timer control
long prev_millis = 0;
long prev_millis_speed = 0;
long interval = 20; // 10ms interval

// methods declaration
void readEncoder();

// -- MAIN -- //
void setup() {
  Serial.begin(115200);
  
  myMotor.motorInit();
  myMotor.backward(0);

  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB), readEncoder, RISING);

  // setting pwm
  pwm = 255;
}

void loop() {
  // get elapsed time
  long current_millis = millis();

  myMotor.backward(pwm);
  // read the position in an atomic block
  // to avoid potential misreads
  int pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = pos_i;
  } 
  // velocity_rpm = readSpeed(pos);

  if ((current_millis - prev_millis_speed) > interval){
    // getting speed
    velocity_rpm = readSpeed(pos);
    prev_millis_speed = current_millis;
  }

  Serial.print((float) current_millis / 1000.0);
  Serial.print(" ");
  Serial.print(pwm);
  Serial.print(" ");
  Serial.println(velocity_rpm);

  prev_millis = current_millis;

  // delay(10);
}

void readEncoder() {
  pos_i ++;
}