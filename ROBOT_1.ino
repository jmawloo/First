/*
  Memsic2125

  Read the Memsic 2125 two-axis accelerometer. Converts the pulses output by the
  2125 into milli-g's (1/1000 of Earth's gravity) and prints them over the
  serial connection to the computer.

  The circuit:
    - X output of accelerometer to digital pin 2
    - Y output of accelerometer to digital pin 3
    - +V of accelerometer to +5V
    - GND of accelerometer to ground

  created 6 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Memsic2125
*/

// these constants won't change:
const int xPin = 2;     // X output of the accelerometer
const int yPin = 3;     // Y output of the accelerometer
const int led1 = 9;  //Green
const int led2 = 10; //Red
const int led3 = 11; //Yellow
const int led4 = 12; //White
void setup() {
  // initialize serial communications:
  Serial.begin(9600);
  // initialize the pins connected to the accelerometer as inputs:
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
}


void loop() {
  // variables to read the pulse widths:
  int pulseX, pulseY;
  // variables to contain the resulting accelerations
  int accelerationX, accelerationY;

  // read pulse from x- and y-axes:
  pulseX = pulseIn(xPin, HIGH);
  pulseY = pulseIn(yPin, HIGH);

  // convert the pulse width into acceleration
  // accelerationX and accelerationY are in milli-g's:
  // Earth's gravity is 1000 milli-g's, or 1 g.
  accelerationX = ((pulseX / 10) - 500) * 8;
  accelerationY = ((pulseY / 10) - 500) * 8;

  if(accelerationX < -66) {
    digitalWrite(led1, HIGH);
  }
  else {
    digitalWrite(led1, LOW); 
  }
  if(accelerationX > 24) {
    digitalWrite(led2, HIGH);
  }
  else {
    digitalWrite(led2, LOW); 
  }

  if (accelerationY < -80) {
    digitalWrite(led3, HIGH);
  }
  else {
    digitalWrite(led3, LOW);
  }
  if (accelerationY > -40) {
    digitalWrite(led4, HIGH);
  }
  else {
    digitalWrite(led4, LOW);
  }
  
  // print the acceleration
  Serial.print(accelerationX);
  // print a tab character:
  Serial.print("\t");
  Serial.print(accelerationY);
  
  Serial.print("| \t |");
  Serial.print(pulseX);
  Serial.print("\t");
  Serial.print(pulseY);
  Serial.println();

  delay(100);
}
