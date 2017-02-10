#include <AccelStepper.h>

// General definitions
int CurrentFilter = 0;                  // Filter integer
int HallValue;                          // Hall switch variable

// Filter steps definitions
int CalibrationOffset = 70;              // Steps required to move to first filter position after calibration is done
int FilterSteps = 819.2;                  // Steps required to move the filter to next filter location adjust as required to suit motor wheel combination
int FilterSteps0 = 0;                   // Set filter steps for home position from calibration
int FilterSteps1 = FilterSteps;         // Set filter steps for 1st position
int FilterSteps2 = FilterSteps * 2;     // Set filter steps for 2nd position
int FilterSteps3 = FilterSteps * 3;     // Set filter steps for 3rd position
int FilterSteps4 = FilterSteps * 4;     // Set filter steps for 4th position

// Hall pin definition
const int out = 12;                     // PIN 12 = Hall effect switch

// Motor definitions
#define STEPS 8                         // 28BYJ-48 steps 4 or 8

// Motor pin definitions
#define motorPin1  8       // IN1 on UL2003
#define motorPin2  9       // IN2 on UL2003
#define motorPin3  10      // IN3 on UL2003
#define motorPin4  11      // IN4 on UL2003

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(STEPS, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  Serial.flush();
  Serial.begin(9600);  // Baud rate, make sure this is the same as ASCOM driver
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(1000.0); // Acceleration
  stepper1.setSpeed(1000.0);
  stepperHome(); //runs routine to home motor
  Serial.println("1#");
}


void loop() {

  String cmd;
  if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('#');  // Terminator so arduino knows when the message ends
    if (cmd == "GETFILTER") {
      Serial.print(CurrentFilter); Serial.println("#");  // Similarly, so ASCOM knows
    }
    else if (cmd == "FILTER0") MoveFilter(0); // Move Filter Routine
    else if (cmd == "FILTER1") MoveFilter(1); // Move Filter Routine
    else if (cmd == "FILTER2") MoveFilter(2); // Move Filter Routine
    else if (cmd == "FILTER3") MoveFilter(3); // Move Filter Routine
    else if (cmd == "FILTER4") MoveFilter(4); // Move Filter Routine
    else if (cmd == "FILTER5") MoveFilter(5); // Calibration Routine
  }
}


void MoveFilter(int pos) {
  
  CurrentFilter = pos;    // Note that the position is always 0 when powered on due to calibration
  switch (CurrentFilter) {
  case 0:
    digitalWrite(13, HIGH); // Flash LEDs for Move
    stepper1.runToNewPosition(FilterSteps0); // Goto new position
    Serial.print(CurrentFilter); Serial.println("#"); // Notify that we sent the command
    digitalWrite(13, LOW); // Disable LEDs after Move
  break;

  case 1:
    digitalWrite(13, HIGH); // Flash LEDs for Move
    stepper1.runToNewPosition(FilterSteps1); // Goto new position
    Serial.print(CurrentFilter); Serial.println("#"); // Notify that we sent the command
    digitalWrite(13, LOW); // Disable LEDs after Move
  break;

  case 2:
    digitalWrite(13, HIGH); // Flash LEDs for Move
    stepper1.runToNewPosition(FilterSteps2); // Goto new position
    Serial.print(CurrentFilter); Serial.println("#"); // Notify that we sent the command
    digitalWrite(13, LOW); // Disable LEDs after Move
  break;

  case 3:
    digitalWrite(13, HIGH); // Flash LEDs for Move
    stepper1.runToNewPosition(FilterSteps3); // Goto new position
    Serial.print(CurrentFilter); Serial.println("#"); // Notify that we sent the command
    digitalWrite(13, LOW); // Disable LEDs after Move
  break;

  case 4:
    digitalWrite(13, HIGH); // Flash LEDs for Move
    stepper1.runToNewPosition(FilterSteps4); // Goto new position
    Serial.print(CurrentFilter); Serial.println("#"); // Notify that we sent the command
    digitalWrite(13, LOW); // Disable LEDs after Move
  break;

  case 5:
    stepperHome(); // Calibratio Routine
    Serial.print(CurrentFilter); Serial.println("#"); // Notify that we sent the command
  break;
  }

}

void stepperHome()
{ 
  HallValue = digitalRead(out);    // read the hall sensor value
  digitalWrite(13, HIGH); // Flash LEDs for Move

  while (HallValue == HIGH)
  {
    stepper1.setAcceleration(1000.0); // Acceleration
    stepper1.setSpeed(1000.0);
    stepper1.move(20);
    stepper1.run();
    HallValue = digitalRead(out); // read the hall sensor value
  }

  stepper1.setCurrentPosition(0);
  stepper1.runToNewPosition(CalibrationOffset);
  stepper1.setCurrentPosition(0);
  
  digitalWrite(13, LOW); // Disable LEDs after Move
  
}
