/*
  Plant Sensing

  Code that enables three different types of sensors for a plant. A capacitive sensor, a moisture sensor, and a photo sensor. In addition this script also supports
  different outputs to both P5 and a physical LED.

  modified date 4/3/2022
  by Jack Watson
 */

#include <CapacitiveSensor.h>
#include <arduino-timer.h>

// Output pins
int colorPin = 9;
int brightnessPin = A0;

// Sensor input pins
int moisturePinIn = A2;
int moisturePinOut = A3;
int photoPin = A1;
int capacitivePinOut = 10;
int capacitivePinIn = 8;

// Create the touch sensor object, and the timer object
CapacitiveSensor touchSensor = CapacitiveSensor(capacitivePinOut, capacitivePinIn);
auto timer = timer_create_default(); // create a timer with default settings

// Memory for the global plants state. [0] is watered, [1] is light, [2] is touched
bool currentState[3] = {false, false, false};

// Sensor configuration variables.
int calibratedTouchValue = 14;
int sensorReadDelay = 20;

/**
 * Initialize the arduino
 */
void setup() {
  Serial.begin(9600);
  pinMode(colorPin, OUTPUT);
  timer.every(120000, statusIndicator); // Every 2 minutes.
  determineState(currentState);
  
  // On setup we want to calibrate the touch sensor so that we can confirm that we are going to be going from a value that can resist the changes from water.
  calibratedTouchValue = getTouchSense(100);
}

/**
 * Run the main program loop. Here our goal is to determine what the current state of the applicaiton is.
 */
void loop() {
  
  timer.tick();

  // Determine the state changes based on sensor values.
  bool* oldState = copyCurrentState();
  determineState(currentState);
  
  // Here we both update and render based on the current game state. Not the best way to do it but it works for us now and prevents us from having to duplicated if statement logic.
  // I could clean this up if I could pass functions to next functions - or use object oriented.
  // Here we have a state machine that tracks the state of three seperate processies all in one. 

  // If the touch state changes
  if(currentState[2] != oldState[2]){
    changeTouchState();
  // If the water state changes
  } else if(currentState[0] != oldState[0]) {
    changeWaterState();
  // If the light state changes
  } else if (currentState[1] != oldState[1]){
    changeLightState();
  }
}

// === Output on P5 ===
/**
 * Change the state of the output to be that of either thirsty (2) or back to normal (4)
 */
void changeWaterState() {
  if(currentState[0]) {
    Serial.println(2);
  } else {
    Serial.println(4);
  }
}

/**
 * Change the state of the output to be that of either dark (3) or back to normal (4)
 */
void changeLightState() {
  if(currentState[1]) {
    Serial.println(4);
  } else {
    Serial.println(3);
  }
}

/**
 * Change the state of the touch output to be either touched (1) or back to normal (4)
 */
void changeTouchState() {
  if(currentState[2]) {
    Serial.println(1);
  } else {
    Serial.println(4);
    calibratedTouchValue = getTouchSense(50); // Recalculate the touch metric when this finishes. We have to do this becuase we were getting werid values on the capacitive
    // sensor where it would change it's default voltage after an initial touch.
  }
}

// === Output On LED ===

/**
 * Status indicator for both brighness, and also moisture content.
 */
bool statusIndicator(void *) {
  int waterValue = getDryness();
  int lightValue = getLight();

  digitalWrite(colorPin, HIGH);  
  analogWrite(brightnessPin, map(lightValue, 0, 50, 45, 80));
  delay(5000);
  digitalWrite(colorPin, LOW);
  analogWrite(brightnessPin, map(waterValue, 0, 255, 45, 80));
  delay(5000);

  analogWrite(brightnessPin, 0);

  return true;
}

// === Input ===
/**
 * Helpe method to copy the internal state so we can save it to memory somwhere.
 */
bool* copyCurrentState() {
  return new bool[3] {
    currentState[0], 
    currentState[1], 
    currentState[2]
  };
}

/*
 * Determine the overall state of the plant. Load this into our custom datastructure. 
 */
bool* determineState(bool* currentState) {
   currentState[0] = determineStateChange(currentState[0], getDryness(), 100, 200);
   currentState[1] = determineStateChange(currentState[1], getLight(), 30, 60);
   currentState[2] = determineStateChange(currentState[2], getTouchSense(20), calibratedTouchValue + 5, calibratedTouchValue + 10);
}


/*
 * Help function that allows us to determine the internal state of the current sensor.
 */
bool determineStateChange(bool originalState, int newValue, int minValue, int maxValue) {
  if(newValue >= maxValue && !originalState) {
    return true;
  } else if(newValue <= minValue && originalState) {
    return false;
  }
  return originalState;
}


/*
 * Get the light data from the potted plant. Take the average for more accurate results.
 */
int getLight() {
  int tempValue = 0;
  for (int i = 0; i < 5; i++) {
    tempValue += analogRead(photoPin);
    delay(sensorReadDelay);
  }
  return tempValue / 5;
}

/**
 * Get the dryness of the soil that the plant is currently in. Right now we implement this as an average.
 */
int getDryness() {
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


/**
 * Get the plants sense of touch. Take the average for more accurate results.
 */
int getTouchSense(int average) {
  int tempValue = 0;
  for (int i = 0; i < average; i++) {
    tempValue += map(touchSensor.capacitiveSensor(50), 0, 90000, 0, 255);
    delay(sensorReadDelay);
  }
  return tempValue / average;
}
