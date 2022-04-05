/*
  Test Script for Plant Sensor

  This code enables a series of tests that we run to confirm that sensors are working as intended. Super useful!

  modified date 4/3/2022
  by Jack Watson
 */

#include <CapacitiveSensor.h>

// Output pins.
int colorPin = 9;
int brightnessPin = A0;

// Sensor input pins.
int moisturePinIn = A2;
int moisturePinOut = A3;
int photoPin = A1;
int capacitivePinOut = 10;
int capacitivePinIn = 8;

CapacitiveSensor cs_10_8 = CapacitiveSensor(capacitivePinOut, capacitivePinIn);

int sensorReadDelay = 20;

void setup() {
  Serial.begin(9600);
  pinMode(colorPin, OUTPUT);  
}

void loop() {

  // --- Green LED Test ---
  Serial.println("--- Green LED Test ---");
  digitalWrite(colorPin, HIGH);
  
  for(int i = 0; i < 255; i++) {
    analogWrite(A0, map(i, 0, 255, 40, 80));
    delay(10);
  }

  delay(2000);

  // --- Blue LED Test . ---
  Serial.println("--- Blue LED Test ---");
  digitalWrite(colorPin, LOW);

  for(int i = 0; i < 255; i++) {
    analogWrite(brightnessPin, map(i, 0, 255, 40, 80));
    delay(10);
  }

  delay(2000);

  // --- Plant Sensor Test ---
  Serial.println("--- Plant Sensor Test ---");
  
  // Run the following test for 10 seconds.
  for(int i = 0; i < 100; i++) {
     Serial.println(getTouchSensor());
     delay(10);
  }

  delay(2000);

  // --- Moisture Sensor Test ---
  Serial.println("--- Moisture Sensor Test ---");
  for(int i = 0; i < 100; i++) {
     Serial.println(getMoistureSensor());
  }

  delay(2000);
  // --- Light Sensor Test ---
  Serial.println("--- Light Sensor Test ---");
  for(int i = 0; i < 100; i++) {
    Serial.println(getLightSensor());
    delay(10);
  }
  
  delay(2000);
}

/*
 * Get the capacitive sensor value.
 */
int getTouchSensor() {

  int tempValue = 0;
  for (int i = 0; i < 5; i++) {
    tempValue += map(cs_10_8.capacitiveSensor(30), 0, 90000, 0, 255);
    delay(sensorReadDelay);
  }
  return tempValue / 5;
}

/*
 * Get the light value from the sensor.
 */
int getLightSensor() {
  int tempValue = 0;
  for (int i = 0; i < 5; i++) {
    tempValue += analogRead(photoPin);
    delay(sensorReadDelay);
  }
  return tempValue / 5;
}

/*
 * Get the moisture value.
 */
int getMoistureSensor() {
  int tempValue = 0; // variable to temporarly store moisture value 
  for (int i = 0; i < 5; i++) { 
      analogWrite(moisturePinOut, 255);
      delay(sensorReadDelay * 0.25);
      tempValue += map(analogRead(moisturePinIn), 900, 1024, 0, 255);
      delay(sensorReadDelay * 0.75);
      analogWrite(moisturePinOut, 0);    
  } 
  return tempValue / 5; 
} 
