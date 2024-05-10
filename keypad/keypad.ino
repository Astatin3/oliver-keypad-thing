#include <Arduino.h>

// Define pin mappings for the switch matrix
const int switchRowPins[] = {50, 48, 46, 44}; // Switch Rows
const int switchColPins[] = {40, 38, 36, 34, 32, 30, 28, 26, 24, 22}; // Switch Columns

// Define the pin arrays for the LED matrix
const int LEDRowPins[] = {51, 49, 47, 45}; // LED Rows (Anodes)
const int LEDColPins[] = {41, 39, 37, 35, 33, 31, 29, 27, 25, 23}; // LED Columns (Cathodes)

const int NUM_SLIDERS = 10;
const int analogInputs[NUM_SLIDERS] = {A9, A8, A7, A6, A5, A4, A3, A2, A1, A0};

const int keyLightTimeout = 100;
const int sliderUpdateTolerance = 8;

#define ROWS 4
#define COLS 10

bool keyState[ROWS][COLS] = {false}; // Holds the state of each key
int analogSliderValues[NUM_SLIDERS];


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

}

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

int newVal = 0;

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    newVal = analogRead(analogInputs[i]);
    if(abs(analogSliderValues[i]-newVal) >= sliderUpdateTolerance){
      analogSliderValues[i] = newVal;
      Serial.print("1|");
      Serial.print(i+1);
      Serial.print("|");
      Serial.println(newVal);
    }
  }
}

unsigned long lastupdated = 0;

void loop() {

  updateLights();
  updateSliderValues();


  if(millis()-lastupdated > keyLightTimeout){
    lastupdated = millis();
    lightsOff();
    // Serial.println("Lights reset!");
  }


    for (int row = 0; row < ROWS; row++) {
        digitalWrite(switchRowPins[row], LOW);  // Activate row
        for (int col = 0; col < COLS; col++) {
          if(digitalRead(switchColPins[col]) == LOW){

            if(keyState[row][col] == false){
              Serial.print("0|");
              Serial.print(row);
              Serial.print("|");
              Serial.println(col);
            }

            keyState[row][col] = true;


            lastupdated = millis();
          }

          
        }
        digitalWrite(switchRowPins[row], HIGH);  // Deactivate row
    }
}
