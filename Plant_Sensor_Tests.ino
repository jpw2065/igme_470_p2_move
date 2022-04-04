#include <CapacitiveSensor.h>

// Output pins.
int colorPin = 9;
int brightnessPin = A0;

// Sensor input pins.
int moisturePin = A2;
int photoPin = A1;
int capacitivePinOut = 10;
int capacitivePinIn = 8;

CapacitiveSensor cs_10_8 = CapacitiveSensor(capacitivePinOut, capacitivePinIn);

void setup() {
  Serial.begin(9600);
  pinMode(colorPin, OUTPUT);  
}

void loop() {

  // --- Blue LED Test ---
  Serial.println("--- Blue LED Test ---");
  digitalWrite(colorPin, HIGH);
  
  for(int i = 0; i < 255; i++) {
    analogWrite(A0, map(i, 0, 255, 40, 80));
    delay(10);
  }

  delay(2000);

  // --- Green LED Test . ---
  Serial.println("--- Green LED Test ---");
  digitalWrite(colorPin, LOW);

  for(int i = 0; i < 255; i++) {
    analogWrite(brightnessPin, map(i, 0, 255, 40, 80));
    delay(10);
  }

  delay(2000);

  // --- Plant Sensor Test ---
  Serial.println("--- Plant Sensor Test ---");
  
  // Run the following test for 10 seconds.
  for(int i = 0; i < 1000; i++) {
     Serial.println(getTouchSensor());
     delay(10);
  }

  delay(2000);

  // --- Moisture Sensor Test ---
  Serial.println("--- Moisture Sensor Test ---");
  for(int i = 0; i < 10; i++) {
     Serial.println(getMoistureSensor());
  }

  delay(2000);
  // --- Light Sensor Test ---
  Serial.println("--- Light Sensor Test ---");
  for(int i = 0; i < 1000; i++) {
    Serial.println(getLightSensor());
  }
  
  delay(2000);
}

/*
 * Get the capacitive sensor value.
 */
int getTouchSensor() {
  return cs_10_8.capacitiveSensor(30);
}

/*
 * Get the light value from the sensor.
 */
int getLightSensor() {
  return analogRead(photoPin);
}

/*
 * Get the moisture value.
 */
int getMoistureSensor() {
   int tempValue = 0; // variable to temporarly store moisture value 
   for (int a = 0; a < 10; a++) { 
     tempValue += analogRead(moisturePin); 
     delay(100); 
   } 
   return tempValue / 10; 
} 
