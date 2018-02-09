/** 
 *  Three useful functions are defined:
    setupArdumoto() -- Setup the Ardumoto Shield pins
    driveArdumoto([motor], [direction], [speed]) -- Drive [motor] 
      (0 for A, 1 for B) in [direction] (0 or 1) at a [speed]
      between 0 and 255. It will spin until told to stop.
    stopArdumoto([motor]) -- Stop driving [motor] (0 or 1).

  setupArdumoto() is called in the setup().
  The loop() demonstrates use of the motor driving functions.
**/

// Clockwise and counter-clockwise definitions.
// Depending on how you wired your motors, you may need to swap.
#define CW  0
#define CCW 1

// Motor definitions to make life easier:
#define MOTOR_R 0
#define MOTOR_L 1

// Pin Assignments //
// Don't change these! These pins are statically defined by shield layout
const byte PWMA = 3;  // PWM control (speed) for motor R
const byte PWMB = 11; // PWM control (speed) for motor L
const byte DIRA = 12; // Direction control for motor R
const byte DIRB = 13; // Direction control for motor L

const int xPin = 2;     // X output of the accelerometer
const int yPin = 3;     // Y output of the accelerometer
const int led1 = 9;  //Green
const int led2 = 10; //Red
const int led3 = 11; //Yellow
const int led4 = 12; //White

int rX = 0, rY = 0; //Calibration reference variables.

void setup()
{
  setupArdumoto(); // Set all pins as outputs
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


void loop()
{
  // variables to read the pulse widths:
  int pulseX, pulseY;
  // variables to contain the resulting accelerations
  int accelerationX, accelerationY;

  // read pulse from x- and y-axes:
  pulseX = pulseIn(xPin, HIGH);
  pulseY = pulseIn(yPin, HIGH);
  
  //For calibration purposes only:
  if (rX == 0) {//This should only run once in the code.
    rX = ((pulseX / 10) - 500) * 8;
    rY = ((pulseY / 10) - 500) * 8;
  }
  
  // convert the pulse width into acceleration
  // accelerationX and accelerationY are in milli-g's:
  // Earth's gravity is 1000 milli-g's, or 1 g.
  accelerationX = ((pulseX / 10) - 500) * 8;
  accelerationY = ((pulseY / 10) - 500) * 8;


/*
 * Optimize this portion of code. Some ideas:
 * 1. Get acceleration values for X and Y. Compare them to 
 */
  if(accelerationX < rX-40) {
    digitalWrite(led1, HIGH);
    goForward();
  }
  else if(accelerationX > rX+40) {
    digitalWrite(led2, HIGH);
    goBack();
  }
  else {
    digitalWrite(led1, LOW); 
    digitalWrite(led2, LOW);
    stopArdumoto(MOTOR_R);
    stopArdumoto(MOTOR_L);
  }

  if (accelerationY < rY-20) {
    digitalWrite(led3, HIGH);
    goLeft();
  }
  else if (accelerationY > rY+20) {
    digitalWrite(led4, HIGH);
    goRight();
  }
  else {
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    stopArdumoto(MOTOR_R);
    stopArdumoto(MOTOR_L);
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
  Serial.print("\t");
  Serial.print(rX);
  Serial.print("\t");
  Serial.print(rY);
  Serial.println();

  delay(100);
  stopArdumoto(MOTOR_R);
  stopArdumoto(MOTOR_L);
  
}

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == MOTOR_R)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == MOTOR_L)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

// stopArdumoto makes a motor stop
void stopArdumoto(byte motor)
{
  driveArdumoto(motor, 0, 0);
}

// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}


//These are the functions that determine the 4 basic movements of the cart. Input = accelerometer readings.

void goForward() {
  driveArdumoto(MOTOR_R, CW, 255);
  driveArdumoto(MOTOR_L, CCW, 255);
}

void goBack() {
  driveArdumoto(MOTOR_R, CCW, 255);
  driveArdumoto(MOTOR_L, CW, 255);
}

void goLeft() {
  driveArdumoto(MOTOR_R, CW, 255);
  driveArdumoto(MOTOR_L, CW, 255);
}

void goRight() {
  driveArdumoto(MOTOR_R, CCW, 255);
  driveArdumoto(MOTOR_L, CCW, 255);
}
