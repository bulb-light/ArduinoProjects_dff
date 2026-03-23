// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root or https://opensource.org/licenses/MIT

#include <Arduino.h>
#include <util/atomic.h>
#include <DCMotorDriver.h>
#include <SpeedEstimator.h>
#include <DigitalFilter.h>
#include <INA219.h>
#include <PID.h>

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

// globals speed and current measurement and control variables
float speedRefRPM = 150.0; // reference speed in RPM
float speedRPM = 0.0; // velocity
float filteredSpeedRPM = 0.0; // filtered velocity

float currentRef_A = 0.0; // reference current in A
float current_mA = 0.0; // current measurement
float filteredCurrent_mA = 0.0; // filtered current measurement

int pwm = 0; // motor pwm

// timer control
// using unsigned long for millis() compatibility and overflow handling
unsigned long prevMillis = 0;
unsigned long prevMillisSpeed = 0;
unsigned long intervalTs = 10; // Sampling interval 10ms

float sampleTimeSec = float(intervalTs) / 1000.0; // Convert ms to seconds

// Speed control PID parameters (update these as needed)
float KpSpeed = 0.005;
float TiSpeed = 1.5;
float TdSpeed = 0.0; // No derivative action for now
float minCurrentOutput = -1.0;
float maxCurrentOutput = 1.0;
PID motorSpeedPID(KpSpeed, TiSpeed, TdSpeed, sampleTimeSec, minCurrentOutput, maxCurrentOutput);

// Current control PID parameters (update these as needed)
float KpCurrent = 80.0;
float TiCurrent = 0.08;
float TdCurrent = 0.0; // No derivative action for now
float minPWMOutput = 0.0;
float maxPWMOutput = 255.0;
PID motorCurrentPID(KpCurrent, TiCurrent, TdCurrent, sampleTimeSec, minPWMOutput, maxPWMOutput);

// Digital filter for current measurement
// cutoff=8.0Hz, sampling=100Hz
DigitalFilter currentFilter(DigitalFilter::Type::EWMALowPass,
     8.0, 1.0 / ((float) intervalTs / 1000.0));
// cutoff=8Hz, sampling=100Hz
DigitalFilter speedFilter(DigitalFilter::Type::EWMALowPass,
     8.0, 1.0 / ((float) intervalTs / 1000.0));

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

    // Speed PI control loop
    float speedError = speedRefRPM - filteredSpeedRPM;
    float speedPIOutput = motorSpeedPID.computePIDOut(speedError);
    currentRef_A = speedPIOutput;

    // Current PI control loop
    current_mA = INA.getCurrent_mA();
    // filtered current measurement
    filteredCurrent_mA = currentFilter.computeFilterOut(current_mA);
    float currentError_A = currentRef_A - (filteredCurrent_mA / 1000.0); // Current error A
    float currentPIOutput = motorCurrentPID.computePIDOut(currentError_A);
    // update motor pwm
    pwm = (int) currentPIOutput;

    // apply control signal to motor
    myMotor.moveMotor(LOW, HIGH, pwm); // Example: forward direction with PWM control

    // update previous time for sensor readings and control routines
    prevMillisSpeed = currentMillis;
  }

  // Timing control for speedRefRPM changes
  if ((currentMillis - prevMillis) > 1500 && (currentMillis - prevMillis) < 2500)
  {
    speedRefRPM = 500;
  } else if ((currentMillis - prevMillis) > 2500) {
    speedRefRPM = 300;
  }

  // Serial.print(((float)currentMillis) / 1000.0);
  // Serial.print(" ");
  Serial.print(pwm);
  Serial.print(" ");
  Serial.print(filteredSpeedRPM, 2);
  Serial.print(" ");
  Serial.print(speedRefRPM, 2);
  Serial.print(" ");
  Serial.print(filteredCurrent_mA/2, 2);
  Serial.print(" ");
  Serial.println(currentRef_A * 500.0, 2);

}

void readEncoder() {
  // Just a simple counter increment example
  // In a real scenario, you would read the encoder pins and determine direction
  pos_i ++;
}