#include <CapacitiveSensor.h>

// Sensor input pins.
int moisturePin = A2;
int photoPin = A1;
int capacitivePinOut = 10;
int capactiviePinIn = 8;

// Sensor ouptut pins
int transistorPin = 8;

CapacitiveSensor cs_10_8 = CapacitiveSensor(capacitivePinOut, capacitivePinInyy);

// Memory for the global plants state. [0] is watered, [1] is light, [2] is touched
bool[] currentState = [false, false, false];

/**
 * Initialize the arduino
 */
void setup() {
  Serial.begin(9600);
  while(!Serial); // Wait for serial to start.
  setState();
  delay(2000);
}

void loop() {
  
  bool[] oldState = copyCurrentState();
  determineState();

  // Here we both update and render basedd on the current game state. Not the best way to do it but it works for us now and prevents us from having to duplicated if statement logic.
  // I could clean this up if I could pass functions to next functions - or use object oriented.
  // Here we have a state machine that tracks the state of three seperate processies all in one. 
  if(currentState[0] != oldState[0]){
    changeWaterState();
  }

  // If we have chnaged the state of the light sensor trigger that method here.
  if(currentState[1] != oldState[1]) {
    changeLightState()
  }

  // If we have changed the state of the touch sensor trigger that here.
  if(currentState[2] != oldState[2]) {
    changeTouchState();
  }
  
}

// === Output ===

void changeWaterState() {
  if(currentState[0]) {
    // Play watered sound
    // Display watered plant.
  } else {
     // Play un-watered sound.
     // Display un-watered plant.
  }
}

void changeLightState() {
  if(currentState[1]) {
    // Play light on sound
    // Display bright  plant.
  } else {
     // Play light off sound.
     // Display dark plant.
  }
}

void changeTouchState() {
  if(currentState[2]) {
    // Play a touching sound,
    // Display to angry plant.
  } else {
    //Display normal plant.
  }
}

// === Input ===

// Help method to copy the internal state so we can save it to memory somewhere.
bool[] copyCurrentState() {
  return [currentState[0], currentState[1], currentState[2]];
}

/*
 * Determine the overall state of the plant. Load this into our custom datastructure. 
 */
boolean[] determineState() {
  return [
    determineStateChange(currentState[0], getDryness(), 200, 900), 
    determineStateChange(currentState[1], getLight(), 20, 400), 
    determineStateChange(currentState[2], getTouchSense(), 1500, 1500)
  ];
}


/*
 * Help function that allows us to determine the internal state of the current sensor.
 */
bool determineStateChange(originalState, newValue, minValue, maxValue) {
  if(newValue >= maxValue && !originalState) {
    return true;
  } else if(newValue <= minValue && originalState) {
    return false;
  }
  return originalState.
}


/*
 * Get the light data from the potted plant.
 */
int getLight() {
  return analogRead(photoPin);
}

/**
 * Get the dryness of the soil that the plant is currently in. Right now we implement this as an average.
 */
int getDryness() {
  int tempValue = 0; // variable to temporarly store moisture value 
  for (int a = 0; a < 10; a++) { 
    tempValue += analogRead(moisturePin); 
    delay(100); 
  } 
  return tempValue / 10; 
}


/**
 * Get the plants sense of touch.
 */
long getTouchSense() {
    return cs_10_8.capacitiveSensor(30);
//  int scaled = map(total1, 0, 90000, 0, 255); Keep maping just in case we need to output to the transistor pin.
}

// Sensor code.
//int moisturePin = A2; 
//// Set this threeshold accordingly to the resistance you used 
//// The easiest way to calibrate this value is to test the sensor in both dry and wet soil 
//int threeshold = 800; 
//void setup() { 
//   Serial.begin(9600); 
//   while (!Serial); 
//   delay(2000); 
//} 
//void loop() { 
//   Serial.println(get_average_moisture()); 
//   delay(50); 
//} 
//int get_average_moisture() { // make an average of 10 values to be more accurate 
//   int tempValue = 0; // variable to temporarly store moisture value 
//   for (int a = 0; a < 10; a++) { 
//     tempValue += analogRead(moisturePin); 
//     delay(100); 
//   } 
//   return tempValue / 10; 
//} 
//
//
//int ledPin = 3;
//// 1M (fast/touch only) - 40M (within range) resistor (last stripe green blue)
//// between pins 10 & 8 (8 is sensor pin), add a wire or foil or fruit ;)
//CapacitiveSensor cs_10_8 = CapacitiveSensor(10,8);
//
//// Setup serial, and pins.
//void setup() {
//  Serial.begin(9600);
//  pinMode(ledPin, OUTPUT);
//}
//
//// Loop over the sensor reading
//void loop() {
//  long start = millis();
//  long total1 = cs_10_8.capacitiveSensor(30);
//  Serial.print(millis() - start); // check on performance in milliseconds
//  Serial.print("\t"); // tab for debug window spacing
//  Serial.println(total1); // print sensor output 1
//  delay(10); // arbitrary delay to limit data to serial port
//  int scaled = map(total1, 0, 90000, 0, 255);
//  analogWrite(ledPin, scaled);
//}
////https://create.arduino.cc/editor/wmharris/845a2b21-cb1d-4c4a-afa7-74d812d017c5/preview8
