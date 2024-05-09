#include <Arduino.h>

// Define pin mappings for the switch matrix
const int switchRowPins[] = {50, 48, 46, 44}; // Switch Rows
const int switchColPins[] = {40, 38, 36, 34, 32, 30, 28, 26, 24, 22}; // Switch Columns

// Define the pin arrays for the LED matrix
const int LEDRowPins[] = {51, 49, 47, 45}; // LED Rows (Anodes)
const int LEDColPins[] = {41, 39, 37, 35, 33, 31, 29, 27, 25, 23}; // LED Columns (Cathodes)

const int NUM_SLIDERS = 6;
const int analogInputs[NUM_SLIDERS] = {A2, A3, A4, A5, A7, A9};

const int keyLightTimeout = 100;

#define ROWS 4
#define COLS 10

bool keyState[ROWS][COLS] = {false}; // Holds the state of each key


// State definitions for key states
#define NONE 0
#define PRESSED 1
#define HELD 2


void setup() {
    Serial.begin(9600);
    for (int pin : switchRowPins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
    for (int pin : switchColPins) {
        pinMode(pin, INPUT_PULLUP);
    }
    for (int pin : LEDRowPins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    for (int pin : LEDColPins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }
    for (int i = 0; i < NUM_SLIDERS; i++) {
      pinMode(analogInputs[i], INPUT);
    }


    // for(int row = 0; row < ROWS; row++){
    //   for(int col = 0; col < COLS; col++){
    //     if((row+col) % 2 == 0){
    //       keyState[row][col] = true;
    //     }
    //   }
    // }

    // for(int y = 0; y < litButtons.length; y++){
    //   for(int x = 0; x < litButtons[y].length; x++){
    //     litButtons
    //   }
    // }
}

// bool arrayIsFalse(bool arr[]){
//   for(bool b : arr){
//     if(!b){return false;}
//   }
//   return true;
// }

// bool array2dIsFalse(bool arr[][]){
//   for(bool b[] : arr){
//     if(!arrayIsFalse(b)){return false;}
//   }
//   return true;
// }

void updateLights(){
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      if (keyState[row][col]) {
        // Turn on the LED

        // Serial.println(time);

        digitalWrite(LEDRowPins[row], HIGH);
        digitalWrite(LEDColPins[col], LOW);

        delay(1); // Short delay to keep the LED visible

        // Turn off the LED
        digitalWrite(LEDRowPins[row], LOW);
        digitalWrite(LEDColPins[col], HIGH);
      }
    }
  }
}

void lightsOff(){
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      keyState[row][col] = false;
    }
  }
}

unsigned long lastupdated = 0;

void loop() {

  updateLights();


  if(millis()-lastupdated > keyLightTimeout){
    lastupdated = millis();
    lightsOff();
    // Serial.println("Lights reset!");
  }


    for (int row = 0; row < ROWS; row++) {
        digitalWrite(switchRowPins[row], LOW);  // Activate row
        for (int col = 0; col < COLS; col++) {
          if(digitalRead(switchColPins[col]) == LOW){
            keyState[row][col] = true;

            // Serial.print(row);
            // Serial.print(", ");
            // Serial.print(col);
            // Serial.print(" ");
            // Serial.println("Pressed!");

            lastupdated = millis();
          }

          
        }
        digitalWrite(switchRowPins[row], HIGH);  // Deactivate row
    }
}
