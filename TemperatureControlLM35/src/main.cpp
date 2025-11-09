#include <Arduino.h>
#include <PID.h>

// sensor pins
#define LM35_PIN A0

// pwm output pins
#define TIP31C_PIN 3

// instantiate PID object (kp, ki, kd, Ts, min_lim, max_lim)
PID tempPID(0.01, 0.001, 0.0, 0.010, 0.0, 255.0);

// control vars
float temp_ref = 350.0; // setpoint

// timer control
long prev_millis = 0;
long prev_millis_temp = 0;
long control_interval = 20; //10ms interval for speed reading

void setup() {
  // initialize serial communication
  Serial.begin(115200);

  // initialize mcu ports
  pinMode(LM35_PIN, INPUT);
  pinMode(TIP31C_PIN, OUTPUT);
}

void loop() {
  // pwm output to control the heater
  analogWrite(TIP31C_PIN, 128);

  // filter the sensor reading
  int sensor_value = 0;
  float temperature_c = 0.0;

  for (int i = 0; i < 10; i++)
  {
    sensor_value = analogRead(LM35_PIN);
    float voltage = sensor_value * (5.0 / 1023.0); // Convert ADC value to voltage
    temperature_c = voltage * 100.0; // LM35 outputs 10mV per degree Celsius
  }

  // Serial.print("Temperature: ");
  Serial.print(10);
  Serial.print(" ");
  Serial.print(30);
  Serial.print(" ");
  Serial.println(temperature_c);
  // Serial.println(" °C");

  // Serial for teleplot
  Serial.print(">Temp:");
  Serial.println(temperature_c);

  Serial.print(">Set:");
  Serial.println(0);
  
  // int setpointInt = (int) ((30.0 /100) * (1023.0/5.0)); // 30 degrees C to 10 bits ADC value
  

  // downscale sensor reading sensorvalue to fit in a byte
  delay(200); // Wait for a second before the next reading
}