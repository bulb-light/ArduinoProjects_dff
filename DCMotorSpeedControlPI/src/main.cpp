// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

#include <Arduino.h>
#include <util/atomic.h>
#include <DCMotorDriver.h>
#include <SpeedEstimator.h>
#include <DigitalFilter.h>
#include <PID.h>

// Motor control pins
// Modify these pin definitions as per your wiring
#define IN1 8
#define IN2 7
#define ENA 10

// Encoder pins (change as needed)
#define ENCA 3
#define ENCB 2

// instantiate motor object
DCMotorDriver myMotor(IN1, IN2, ENA);

// instantiate speed estimator object
// Encoder parameters
float ppr = 22.0; // Pulses per revolution
float gearRatio = 9.3; // Gear ratio
SpeedEstimator speedEstimator(ppr, gearRatio); // ppr, gear

// globals speed and current measurement and control variables
float speedRefRPM = 150.0; // reference velocity
float speedRPM = 0.0; // velocity
float filteredSpeedRPM = 0.0; // filtered velocity

int pwm = 0; // motor pwm

// timer control
// using unsigned long for millis() compatibility and overflow handling
unsigned long prevMillis = 0;
unsigned long prevMillisSpeed = 0;
unsigned long intervalTs = 10; // Sampling interval 10ms

// PID parameters (update these as needed)
float Kp = 0.18;
float Ti = 0.05;
float Td = 0.0; // No derivative action for now
float sampleTimeSec = float(intervalTs) / 1000.0; // Convert ms to seconds
float minOutput = 0.0;
float maxOutput = 255.0;
PID motorSpeedPID(Kp, Ti, Td, sampleTimeSec, minOutput, maxOutput);

// speed estimator filter
// cutoff=10Hz, sampling=100Hz
DigitalFilter speedFilter(DigitalFilter::Type::EWMALowPass,
     10.0, 1.0 / ((float) intervalTs / 1000.0));

// NOTE: These steps are mandatory to use the SpeedEstimator class!
// Implement your own method to read encoder pulses. This is just a simplified example.

// globals speed measurement method 1
volatile long pos_i = 0;

// method to read encoder
void readEncoder();

// -- MAIN -- //
void setup() {
  Serial.begin(115200);
  
  myMotor.motorInit();
  myMotor.stop();

  // setting up encoder interrupts
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB), readEncoder, RISING);

  delay(2000);

  // initialize timers
  prevMillis = millis();
  prevMillisSpeed = millis();
}

void loop() {
  // get elapsed time
  unsigned long currentMillis = millis();

  // read the position in an atomic block
  // to avoid potential misreads
  long pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = pos_i;
  } 

  // speed calculation at defined interval 10ms
  if ((currentMillis - prevMillisSpeed) > intervalTs){
    // getting speed
    speedRPM = speedEstimator.estimateSpeed(pos);
    // filtering speed
    filteredSpeedRPM = speedFilter.computeFilterOut(speedRPM);

    // PID control
    float speedError = speedRefRPM - filteredSpeedRPM;
    float controlOutput = motorSpeedPID.computePIDOut(speedError);
    // update motor pwm
    pwm = (int) controlOutput;
    // apply control signal to motor
    myMotor.moveMotor(HIGH, LOW, pwm); // Example: forward direction with PWM control

    prevMillisSpeed = currentMillis;
  }

  // Timing control for speedRefRPM changes
  if ((currentMillis - prevMillis) > 1500 && (currentMillis - prevMillis) < 2500)
  {
    speedRefRPM = 500;
  } else if ((currentMillis - prevMillis) > 2500) {
    speedRefRPM = 300;
  }

  Serial.print(((float)currentMillis) / 1000.0);
  Serial.print(" ");
  Serial.print(pwm);
  Serial.print(" ");
  Serial.print(filteredSpeedRPM, 2);
  Serial.print(" ");
  Serial.println(speedRefRPM, 2);
}

void readEncoder() {
  // Just a simple counter increment example
  // In a real scenario, you would read the encoder pins and determine direction
  pos_i ++;
}
