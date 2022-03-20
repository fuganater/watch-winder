// Watch Winder
#include <Stepper.h>

#define LEDPIN 5
#define MAX_TURNS 5

// 28BYJ-48 settings
// When the 28BYJ-48 motor runs in full step mode, each step corresponds to a rotation of 11.25°.
// That means there are 32 steps per revolution (360°/11.25° = 32).
// In addition, the motor has a 1/64 reduction gear set.
// (Actually its 1/63.68395 but for most purposes 1/64 is a good enough approximation)
// What this means is that there are actually 32*63.68395 steps per revolution = 2037.8864 ~ 2038 steps!

// Defines the number of steps per rotation/revolution
const int stepsPerRevolution = 2038;

// The motor (wires 1 2 3 4) is connected to the outputs 8 9 10 11 of the Arduino (and to GND, + V)
Stepper small_stepper (stepsPerRevolution, 8, 9, 10, 11); // Clockwise

long duration = 0;        // Duration of rotation for one revolution
long totalturntime = 0;   // Duration of turning

int turn=1;

void setup ()
{
  Serial.begin (115200);
  Serial.println ("Watch Winder");
  pinMode (LEDPIN, OUTPUT); // Declare Pin 5 as output for the LED

  // Set motor speed
  small_stepper.setSpeed(12); // Speed of 12 rpm => 0.2 revolution per second => 5 seconds for a revolution
  totalturntime = millis();
}

// Turns per day calculation TPD
//1 turn L 5 seconds
//1 second delay
//1 turn R 5 seconds
//1 second delay
//= 60 /12 => 5 turns per minute
//Add 15 minutes idle
// 1 cycle = 16 minutes
// 1 day = (24 hrs X 60 mins) / 16 mins = 90 cycles
// 1 mins X 5 turns X 90 cycles = 450 turns per day

void loop ()
{
  digitalWrite (LEDPIN, HIGH);
  delay (100);
 
  if (turn <= MAX_TURNS) {
    Serial.println ("Motor running");
    Serial.print ("Turn:");Serial.println(turn);

    duration = millis ();
    small_stepper.step (-stepsPerRevolution); //It turns CCW
    duration = millis () - duration;
    Serial.println (duration); // Displays the time (in ms) for a complete revolution
    delay (1000); //pause

    duration = millis ();
    small_stepper.step (stepsPerRevolution); //It turns CW
    duration = millis () - duration;
    Serial.println (duration); // Displays the time (in ms) for a complete revolution
    delay (1000); //pause

    turn++; // Add 1 to the turns Counter
  }
  else
  {
    totalturntime = millis() - totalturntime;
    Serial.print("Duration of turn in minutes: ");Serial.println(totalturntime/1000/60);
  // The above is 5 turns per minute
    for (int i=1;i<=150;i++)
    {
      digitalWrite(LEDPIN, LOW);Serial.print('.');
      delay(1000);
      digitalWrite(LEDPIN, HIGH);Serial.print('.');
      delay(1000);
    }
    // 5 Minutes Elapsed with the LED flashing

    // Wait 10 minutes before rotation again
    Serial.println ("Idle for 10 minutes");
    delay(60000); //(60000) = 10 minutes

    turn = 0; // Reset the turns Counter
    totalturntime = millis();
  }
}
