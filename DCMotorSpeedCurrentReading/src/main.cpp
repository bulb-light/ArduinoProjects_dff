// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

#include <Arduino.h>
#include <util/atomic.h>
#include <DCMotorDriver.h>
#include <SpeedEstimator.h>
#include <INA219.h>

// Motor control pins
// Modify these pin definitions as per your wiring
#define IN1 8
#define IN2 7
#define ENA 10

// NOTE: The following steps are mandatory to use the SpeedEstimator class!
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

// INA219 object for current measurement
// NOTE: This lib depends on Wire.h for I2C communication
// The Wire library is included in INA219.h and is expected exist
// in your Arduino environment by default (e.g., Arduino IDE, PlatformIO, etc.)
// Include the Wire library in your main project if needed.
INA219 INA(0x40); // default address

// globals speed measurement and control variables
float speedRPM = 0.0; // current velocity
int pwm = 0; // motor pwm

// timer control
// using unsigned long for millis() compatibility and overflow handling
unsigned long prevMillis = 0;
unsigned long prevMillisSpeed = 0;
unsigned long interval = 10; // 10ms interval

// NOTE: These steps are mandatory to use the SpeedEstimator class!
// Implement your own method to read encoder pulses. This is just a simplified example.

// globals speed measurement method 1
volatile int pos_i = 0;

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

  // setting pwm
  pwm = 100;

  Wire.begin();
  if (!INA.begin())
  {
    Serial.println("Could not connect. Fix and Reboot");
  }
  // else {
  //   Serial.println("INA219 connected");
  // }
  INA.setMaxCurrentShunt(3.2, 0.100); // 3.6A max current, 100mOhm shunt
  //Serial.println(INA.getBusVoltageRange()); // should print 16 or 32
  delay(2000);

  // initialize timers
  prevMillis = millis();
  prevMillisSpeed = millis();
}

void loop() {
  // get elapsed time
  unsigned long currentMillis = millis();

  myMotor.moveMotor(0, 1, pwm);
  // read the position in an atomic block
  // to avoid potential misreads
  int pos = 0;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = pos_i;
  } 

  // speed calculation at defined interval 10ms
  if ((currentMillis - prevMillisSpeed) > interval){
    // getting speed
    speedRPM = speedEstimator.estimateSpeed(pos);
    prevMillisSpeed = currentMillis;
  }

  Serial.print((float) currentMillis / 1000.0);
  Serial.print(" ");
  Serial.print(pwm);
  Serial.print(" ");
  Serial.print(speedRPM);
  Serial.print(" ");
  // Serial.print(INA.getBusVoltage_mV(), 2);
  // Serial.print(" ");
  // Serial.println(INA.getFilteredtCurrent_mA(), 2);
  Serial.println(INA.getCurrent_mA(), 2);

  // delay(10);

  // Timing control for pwm changes
  if ((currentMillis - prevMillis) > 1000 && (currentMillis - prevMillis) < 2000)
  {
    pwm = 255;
  } else if ((currentMillis - prevMillis) > 2000) {
    pwm = 150;
  }
}

void readEncoder() {
  // Just a simple counter increment example
  // In a real scenario, you would read the encoder pins and determine direction
  pos_i ++;
}