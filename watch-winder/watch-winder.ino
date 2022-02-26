//**************************************************
// Created by Shane Fuga
//**************************************************

#include <Stepper.h>

#define STEPS 100
#define LEDPIN 5
#define MAX_CYCLES 30

// The motor (wires 1 2 3 4) is connected to the outputs 8 9 10 11 of the Arduino (and to GND, + V)
Stepper small_stepper (STEPS, 8, 10, 9, 11); // Clockwise
// Stepper small_stepper (STEPS, 11, 10, 9, 8); // Counterclockwise
int Steps2Take = 0;   // Number of rotation steps requested from the motor
long time = 0;        // Duration of rotation for one revolution

// For a motor of this type: https://amzn.to/3d9IVh9
// 64 steps per revolution, 4 phases, 5.625 ° angle according to the motor specifications
// 1:64 reduction
// 360 ° / 5.625 ° * 64 = 4096 angles with the gear reduction
// 360 ° / 5.625 ° * 64 * 4 coils / 2 bipolar = 2048 step / turn

int cycle=1;

void setup ()
{
  Serial.begin (9600);
  Serial.println ("Watch Winder");
  pinMode (LEDPIN, OUTPUT); // Declare Pin 5 as output for the LED

  // Run the motor
  small_stepper.setSpeed(300); // Speed of 300 (max) reduce this figure for a slower movement
  // 100 allows to have a high torque> 300 the motor vibrates without turning
}

void loop ()
{
  digitalWrite (LEDPIN, HIGH);
  delay (100);

  if (cycle <= MAX_CYCLES) {
    Serial.println ("Motor running");
    Serial.print ("Cycle:");Serial.println(cycle);

    Steps2Take = -4096; // One complete rotation with 2048 steps (1 turn around 4.5sec)
    // To turn backwards 6 times 1 / 30th of a turn, simply multiply Steps2Take by 6/30 and put a minus to reverse the direction
    // Example Steps2Take = -6 * 2048/30;
    time = millis ();
    small_stepper.step (Steps2Take); //It turns
    time = millis () - time;
    Serial.println (time); // Displays the time (in ms) for a complete revolution
    delay (2000); //pause

    Steps2Take = 4096; // One complete rotation with 2048 steps
    // To turn backwards 6 times 1 / 30th of a turn, simply multiply Steps2Take by 6/30 and put a minus to reverse the direction
    // Example Steps2Take = -6 * 2048/30;
    time = millis ();
    small_stepper.step (Steps2Take); //It turns
    time = millis () - time;
    Serial.println (time); // Displays the time (in ms) for a complete revolution
    delay (2000); //pause

    cycle++; // Add 1 to the cycles Counter
  }
  else
  {
  // 30 rotations/cycles takes about 10 minutes
    for (int i=1;i<=150;i++)
    {
      digitalWrite(LEDPIN, LOW);Serial.print('.');
      delay(1000);
      digitalWrite(LEDPIN, HIGH);Serial.print('.');
      delay(1000);
    }
    // 5 Minutes Elapsed with the LED flashing

    // Wait 30 minutes before rotation again
    Serial.println ("Idle for 30 minutes");
    delay(1800000); //(1800000) = 30 minutes

    cycle = 0; // Reset the cycles Counter
  }
}
