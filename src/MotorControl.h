#include <Arduino.h>
#include <AccelStepper.h>


#define inAPin 8
#define inBPin 6
#define pwmPin 7

#define inputButton 16


extern AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
//setEnablePin(9);
extern float maxSpd;
extern float targetSpeed;
extern long targetDist;
extern long heatingDist; //12502 to get to position for 900C holding temp. 13239.5 to get to position for 1000C holding temp
extern float targetAccel;
extern int moveCount;
extern int moveStage;
extern int moveState;
extern int progNum;

void moveGantry();
void actuateJaws(int jawPos);
void runMoveProg();