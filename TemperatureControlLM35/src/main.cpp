// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <PID.h>

// sensor pins
#define LM35_PIN A0

// pwm output pins
#define TIP31C_PIN 3

float Kp = 2.0;    // proportional gain
float Ti = 40.0;   // integral time constant in seconds
float Td = 5.0;    // derivative time constant in seconds
float sampleTimeSec = 0.5; // sampling time in seconds
float minOutput = 0.0;   // minimum output
float maxOutput = 255.0; // maximum output
PID tempPID(Kp, Ti, Td, sampleTimeSec, minOutput, maxOutput);

// control vars
float tempRef = 50.00; // setpoint

// timer control
long prevMillis = 0;
long prevMillisTemp = 0;
long controlIntervalTs = (int) (1000 * sampleTimeSec); //500ms interval for control loop

void setup() {
  // initialize serial communication
  Serial.begin(115200);

  // initialize mcu ports
  pinMode(LM35_PIN, INPUT);
  pinMode(TIP31C_PIN, OUTPUT);
 
  // initialize PID controller
  tempPID.resetStates();
  // tempPID.setSampleTime(0.5); // 500ms sample time
  // tempPID.setOutputLimits(0.0, 255.0);
  tempPID.computePIDOut(0.0);
}

void loop() {
  // filter the sensor reading
  int sensorValue = 0;
  // actual temperature in Celsius
  float temperature_c = 0.0;

  // simple moving average filter for sensor reading
  for (int i = 0; i < 10; i++)
  {
    sensorValue = analogRead(LM35_PIN);
    float voltage = sensorValue * (5.0 / 1023.0); // Convert ADC value to voltage
    temperature_c = voltage * 100.0; // LM35 outputs 10mV per degree Celsius
  }

  // timed control loop
  long currentMillis = millis();
  if (currentMillis - prevMillisTemp >= controlIntervalTs) {
    prevMillisTemp = currentMillis;

    // compute PID output
    float error = tempRef - temperature_c;
    float pidOutput = tempPID.computePIDOut(error);

    // apply PID output to heater
    analogWrite(TIP31C_PIN, (int)pidOutput);

    Serial.print(tempRef);
    Serial.print(",");
    Serial.print(temperature_c);
    Serial.print(",");
    Serial.println(pidOutput);

    // Serial for teleplot in VSCode
    Serial.print(">Temp:");
    Serial.println(temperature_c);

    Serial.print(">Ref:");
    Serial.println(tempRef);

    Serial.print(">PIDout:");
    Serial.println(pidOutput);

    Serial.print(">Zero:");
    Serial.println(0);
  }
}