// Watch Winder
#include <Stepper.h>

#define LEDPIN 5
#define MAX_TURNS 5
#define IDLE 22 // idle time in minutes

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

long duration = 0;                  // Duration of rotation for one revolution
float totalturntime = 0;            // Duration of turning
float totalcycletime = 0;           // Duration of cycle
float totalturntimeinminutes = 0;   // Total Turn time in minutes
float totalcycletimeinminutes = 0;  // Total Cycle time in minutes
float turnsperminute = 0;           // Turns per minute

int turn=1;

void setup ()
{
  Serial.begin (115200);
  Serial.println ("Watch Winder");
  pinMode (LEDPIN, OUTPUT); // Declare Pin 5 as output for the LED

  // Set motor speed
  small_stepper.setSpeed(12); // Speed of 12 rpm => 0.2 revolution per second => 5 seconds for a revolution
  totalturntime = millis();
  totalcycletime = millis();
}

void loop ()
{
  digitalWrite (LEDPIN, HIGH);
 
  if (turn <= MAX_TURNS) {
    Serial.println ("Motor running");
    Serial.print ("Turn:");Serial.println(turn);

    turnCCW();
    turnCCW();
    delay (1000); //pause

    turnCW();
    turnCW();
    delay (1000); //pause

    turn++; // Add 1 to the turns Counter
  }
  else
  {
    totalturntime = millis() - totalturntime - (2000*MAX_TURNS); //Adjust for the 2 second delay
    totalturntimeinminutes = totalturntime/1000/60;
    turnsperminute = (MAX_TURNS*4)/totalturntimeinminutes;
    Serial.print("Duration of turning in minutes: ");Serial.println(totalturntimeinminutes,2);
    Serial.print("Turns per minute: ");Serial.println(turnsperminute,2);

    // Print some caclulations instead of having to wait for the IDLE time
    float tempcycletimeinminutes=totalturntimeinminutes+IDLE;
    Serial.print("Calculated cycle time in minutes: ");Serial.println(tempcycletimeinminutes,2);
    Serial.print("Caclulated cycles per day: ");Serial.println((24*60)/tempcycletimeinminutes,2);
    Serial.print("Calculated TPD: ");Serial.println(totalturntimeinminutes*turnsperminute*(24*60/(tempcycletimeinminutes)),2);

    Serial.print("Idle for ");Serial.print(IDLE);Serial.println(" minutes");
  // While idle flash the led every second
    for (int i=1;i<=IDLE*60;i++)
    {
      flashled_1sec();
    }

    totalcycletime = millis()-totalcycletime;
    totalcycletimeinminutes=totalcycletime/1000/60;
    Serial.println();
    Serial.print("Total cycle time in minutes: ");Serial.println(totalcycletimeinminutes,2);
    Serial.print("Cycles per day based on the above: ");Serial.println((24*60)/(totalcycletimeinminutes),2);
    Serial.print("Calculated TPD based on the above: ");Serial.println(totalturntimeinminutes*turnsperminute*(24*60/(totalcycletimeinminutes)),2);
    turn = 1; // Reset the turns Counter
    totalcycletime = millis();
    totalturntime = millis();

    // Example TPD
    // IDLE 22 minutes
    // MAX_TURNS 5=>20 turns
    // Duration of turning 1.67 minutes
    // Turns per minute 11.98
    // Total cycle time in minutes 23.67 minutes
    // 1 cycle = 23.67 minutes
    // 1 day = (24 hrs X 60 mins) / 23.67 mins = 60.84 cycles
    // 1.67 minutes turns X 11.98 turns per minute X 60.84 cycles = 1217 turns per day
    // With IDLE 30 minutes => 909 turns per day
    // source: https://www.sgrolexclub.com/forum/other-watches-watch-winder-discussion/watch-winder-discussion/69844-tpd-calculation
  }
}


void turnCCW()
{
  duration = millis ();
  small_stepper.step (-stepsPerRevolution); //It turns CCW
  duration = millis () - duration;
  Serial.print("L");
  Serial.println (duration); // Displays the time (in ms) for a complete revolution
}

void turnCW()
{
  duration = millis ();
  small_stepper.step (stepsPerRevolution); //It turns CW
  duration = millis () - duration;
  Serial.print("R");
  Serial.println (duration); // Displays the time (in ms) for a complete revolution
}

void flashled_1sec()
{
  digitalWrite(LEDPIN, LOW);
  delay(500);
  digitalWrite(LEDPIN, HIGH);
  delay(500);
  Serial.print('.');
}
