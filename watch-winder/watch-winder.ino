//Watch Winder with Arduino @ Wouter Pieper

//Include the Arduino Stepper Library
#include <Stepper.h>


// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 

// Number of Steps Required
int StepsRequired;


// The pins used are 8,9,10,11 
// Connected to ULN2003 Motor Driver In1, In2, In3, In4 
// Pins entered in sequence 1-3-2-4 for proper step sequencing

Stepper steppermotor(STEPS_PER_REV, 8, 10, 9, 11);

void setup()
{
// Nothing  (Stepper Library sets pins as outputs)
}

void loop()
{
 \
  // About 15 rotations Clockwise (1 full rotation is 2048 steps)
  steppermotor.setSpeed(500);    
  StepsRequired  =  30720;
  steppermotor.step(StepsRequired);
  delay(2000);
  
  // About 15 rotations Counterclockwise
  steppermotor.setSpeed(500);    
  StepsRequired  =  - 30720;
  steppermotor.step(StepsRequired);
  
  // Wait 1 hour before rotation again
  delay(3600000);

}
