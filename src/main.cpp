#include <Servo.h>
#include <pins_arduino.h>
#include <Arduino.h>

// Function declarations
int getJoystickPosition(int pin);
void setMotorPosition(Servo motor, int position);

// Setup joysticks
const int JOY_R_VERT = A4;
const int JOY_R_HORIZ = A5;
const int JOY_L_VERT = A0;
const int JOY_L_HORIZ = A1;

// Setup motors
const int CLAW = 3;
const int SHOULDER = 9;
const int ELBOW = 5;
const int BASE = 6;

// Setup servo objects
Servo clawMotor;
Servo shoulderMotor;
Servo elbowMotor;
Servo baseMotor;

void setup() {
  pinMode(JOY_R_VERT, INPUT);
  pinMode(JOY_R_HORIZ, INPUT);
  pinMode(JOY_L_VERT, INPUT);
  pinMode(JOY_L_HORIZ, INPUT);  
  Serial.begin(9600);

  // Connect the servos to the correct motor pins
  clawMotor.attach(CLAW);
  shoulderMotor.attach(SHOULDER);
  elbowMotor.attach(ELBOW);
  baseMotor.attach(BASE);

  // Reset the arm to its default position
  setMotorPosition(clawMotor, 0);
  setMotorPosition(shoulderMotor, 0);
  setMotorPosition(elbowMotor, 0);
  setMotorPosition(baseMotor, 90);
}

void loop() {
  int motorSpeed = 5;

  // CLAW     L/R    LEFT
  int clawAmt = getJoystickPosition(JOY_L_HORIZ); 
  int clawPos = clawMotor.read() + motorSpeed * clawAmt;
  setMotorPosition(clawMotor, clawPos);

  // BASE     L/R    RIGHT  
  int baseAmt = getJoystickPosition(JOY_R_HORIZ);
  int basePos = baseMotor.read() + motorSpeed * baseAmt;
  setMotorPosition(baseMotor, basePos);

  // SHOULDER U/D    LEFT
  int shoulderAmt = getJoystickPosition(JOY_R_VERT);
  int shoulderPos = shoulderMotor.read() + motorSpeed * shoulderAmt;
  setMotorPosition(shoulderMotor, shoulderPos);

  // ELBOW    U/D    RIGHT
  int elbowAmt = getJoystickPosition(JOY_L_VERT);
  int elbowPos = elbowMotor.read() + motorSpeed * elbowAmt;
  setMotorPosition(elbowMotor, elbowPos);
}

int getJoystickPosition(int pin) {
  int amount = analogRead(pin);
  int result = 0;
  
  if (amount > 600) {
    result = -1;  // Joystick pushed in one direction
  } else if (amount < 450) {
    result = 1;   // Joystick pushed in the opposite direction
  }
  
  return result;
}

void setMotorPosition(Servo motor, int position) {
  // Make sure the position is within the range of the servo motor (0-180 degrees)
  if (position < 0) {
    position = 0;
  } else if (position > 180) {
    position = 180;
  }
  motor.write(position);
}
