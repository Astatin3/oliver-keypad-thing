#include <Arduino.h>

// Define pin mappings for the switch matrix
const int switchRowPins[] = {50, 48, 46, 44}; // Switch Rows
const int switchColPins[] = {40, 38, 36, 34, 32, 30, 28, 26, 24, 22}; // Switch Columns

// Define the pin arrays for the LED matrix
const int LEDRowPins[] = {51, 49, 47, 45}; // LED Rows (Anodes)
const int LEDColPins[] = {41, 39, 37, 35, 33, 31, 29, 27, 25, 23}; // LED Columns (Cathodes)

#define ROWS 4
#define COLS 10
int keyState[ROWS][COLS] = {0}; // Holds the state of each key

// State definitions for key states
#define NONE 0
#define PRESSED 1
#define HELD 2

bool litButtons[][] = {
  {false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false}
}

int curLitRow = -1;
int curLitCol = -1;

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
}

bool arrayIsFalse(bool arr[]){
  for(bool b : arr){
    if(!b){return false;}
  }
  return true;
}

bool array2dIsFalse(bool arr[][]){
  for(bool b[] : arr){
    if(!arrayIsFalse(b)){return false;}
  }
  return true;
}

void updateLights(){

  if(array2dIsFalse(litButtons)){
    curLitRow = -1;
    curLitCol = -1;
    return;
  }

  if(curLitRow == -1){
    curLitRow = 0;
  }else{
    curLitRow = (curLitRow+1) % ROWS;
  }

  if(curLitCol == -1){
    curLitCol = 0;
  }else{
    curLitCol = (curLitCol+1) % COLS;
  }

  // Find next enabled light
  while(true){
    if(arrayIsFalse(curLitRow)){
      curLitRow = (curLitRow+1) % ROWS;
      continue;
    }

    while(true){
      if(!litButtons[curLitRow][curLitCol]){
        curLitCol = (curLitCol+1) % COLS;
        continue;
      }

      break;
    }
  }

}

void loop() {
    // for (int row = 0; row < ROWS; row++) {
    //     digitalWrite(switchRowPins[row], LOW);  // Activate row
    //     for (int col = 0; col < COLS; col++) {
    //         if (digitalRead(switchColPins[col]) == LOW) {
    //             digitalWrite(LEDRowPins[row], HIGH);
    //             digitalWrite(LEDColPins[col], LOW);
    //             Serial.print("Button pressed at row ");
    //             Serial.print(row);
    //             Serial.print(", col ");
    //             Serial.println(col);
    //             delay(1000);  // Keep the LED on for a second
    //             digitalWrite(LEDRowPins[row], LOW);
    //             digitalWrite(LEDColPins[col], HIGH);
    //         }
    //     }
    //     digitalWrite(switchRowPins[row], HIGH);  // Deactivate row
    // }
}

// #include <Arduino.h>


// void setup() {
//   //Test serial out
//   Serial.begin(115200);
//   delay(10000);
//   Serial.print("Test");
// }

// void loop() {
// }