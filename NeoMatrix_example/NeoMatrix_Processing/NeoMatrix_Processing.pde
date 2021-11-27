// IMA NYU Shanghai
// Interaction Lab

//setup for grid
int rows = 8; // number of rows in the NeoMatrix
int columns = 8;  // number of columns in the NeoMatrix
int x; // width of each cell
int y; // height of each cell
int lineWidth = 2;

//setup for serial communication
import processing.serial.*;

int NUM_OF_VALUES_FROM_PROCESSING = 2;  /** YOU MUST CHANGE THIS ACCORDING TO YOUR PROJECT **/
int processing_values[] = new int[NUM_OF_VALUES_FROM_PROCESSING]; /** this array stores values you might want to send to Arduino **/

Serial myPort;
String myString;


void setup() {
  size(600, 600);
  
  setupSerial();
}

void draw() {
  background(0);
  stroke(255);
  strokeWeight(lineWidth);

  //draw the lines to make a grid
  x = width / rows;
  for (int i = 1; i < rows; i ++ ) {
    line (i*x, 0, i*x, height);
  }

  y = height / columns;
  for (int i = 1; i < columns; i ++ ) {
    line (0, i*y, width, i*y);
  }


  // send the values to Arduino.
  sendSerialData();
}

void setupSerial() {
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[1], 9600);
  // WARNING!
  // You will definitely get an error here.
  // Change the PORT_INDEX to 0 and try running it again.
  // And then, check the list of the ports,
  // find the port "/dev/cu.usbmodem----" or "/dev/tty.usbmodem----"
  // and replace PORT_INDEX above with the index number of the port.

  myPort.clear();
  // Throw out the first reading,
  // in case we started reading in the middle of a string from the sender.
  myString = myPort.readStringUntil( 10 );  // 10 = '\n'  Linefeed in ASCII
  myString = null;
}

void sendSerialData() {
  String data = "";
  for (int i=0; i<processing_values.length; i++) {
    data += processing_values[i];
    //if i is less than the index number of the last element in the values array
    if (i < processing_values.length-1) {
      data += ","; // add splitter character "," between each values element
    }
    //if it is the last element in the values array
    else {
      data += "\n"; // add the end of data character linefeed "\n"
    }
  }
  //write to Arduino
  myPort.write(data);
  print(data); // this prints to the console the values going to arduino
}
