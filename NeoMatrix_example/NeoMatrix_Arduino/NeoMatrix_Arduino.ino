
///setup for Neopixel
#include <FastLED.h> // declares to use the FastLEB library
#define LED_PIN  10
#define COLOR_ORDER GRB
#define CHIPSET     WS2811  // type of Neopixel
#define BRIGHTNESS 64

/*declare width and height of the matrix*/
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

/* for different pixel layouts */
const bool    kMatrixSerpentineLayout = true;

///setup for Serial Communication from Processing
#define NUM_OF_VALUES_FROM_PROCESSING 3    /** YOU MUST CHANGE THIS ACCORDING TO YOUR PROJECT **/

/** DO NOT REMOVE THESE **/
int tempValue = 0;
int valueIndex = 0;

/* This is the array of values storing the data from Processing. */
int processing_values[NUM_OF_VALUES_FROM_PROCESSING];


// This function will return the right 'led index number' for
// a given set of X and Y coordinates on your matrix.
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  //if layed out in one direction
  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  //if layed out in reverse direction repeated (S shaped)
  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i; 
}


#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);


void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  getSerialData();
  //start writing code here
  
}


//receive serial data from Processing
void getSerialData() {
  while (Serial.available()) {
    char c = Serial.read();
    //switch - case checks the value of the variable in the switch function
    //in this case, the char c, then runs one of the cases that fit the value of the variable
    //for more information, visit the reference page: https://www.arduino.cc/en/Reference/SwitchCase
    switch (c) {
      //if the char c from Processing is a number between 0 and 9
      case '0'...'9':
        //save the value of char c to tempValue
        //but simultaneously rearrange the existing values saved in tempValue
        //for the digits received through char c to remain coherent
        //if this does not make sense and would like to know more, send an email to me!
        tempValue = tempValue * 10 + c - '0';
        break;
      //if the char c from Processing is a comma
      //indicating that the following values of char c is for the next element in the values array
      case ',':
        processing_values[valueIndex] = tempValue;
        //reset tempValue value
        tempValue = 0;
        //increment valuesIndex by 1
        valueIndex++;
        break;
      //if the char c from Processing is character 'n'
      //which signals that it is the end of data
      case '\n':
        //save the tempValue
        //this will b the last element in the values array
        processing_values[valueIndex] = tempValue;
        //reset tempValue and valueIndex values
        //to clear out the values array for the next round of readings from Processing
        tempValue = 0;
        valueIndex = 0;
        break;
    }
  }
}
