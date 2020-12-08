#include "functionDefs.h"
#include "MotorControl.h"

int loopCount = 0;

void setup(void)
{
    Serial.begin(9600);

    pinMode(upArrButt, INPUT);
    pinMode(downArrButt, INPUT);

    digitalWakeUp.pinMode(greenButt, INPUT, RISING);

    tft.init(240, 240);

    uint16_t time = millis();
    tft.fillScreen(ST77XX_BLACK);
    time = millis() - time;

    Serial.println(time, DEC);
    delay(500);

    int activeMenuButton = 0; //The button currently highlighted on the GUI
    int pushedButton = 2;     //The button pushed on the control panel by the user

    //selectButton(pushedButton, activeMenu, activeMenuButton);
    genMenu(&activeMenu, &selectedButton, &selectFlag, &activeFrame);
    attachInterrupt(upArrButt, readPushedButt, HIGH);
    attachInterrupt(downArrButt, readPushedButt, HIGH);
    attachInterrupt(greenButt, readPushedButt, HIGH);


    //Motor control block:
    stepper.setEnablePin(0);
	stepper.enableOutputs(); //These stepper drivers (TB6600) are enabled with EN+ low. enableOutputs() drives the signal high,
	//so you must invert the function to achieve the desired results. Here we want to disable the stepper motors.
	Serial.begin(9600);
	stepper.setMaxSpeed(maxSpd);
	stepper.setSpeed(targetSpeed);
	//stepper.move(targetDist);
	stepper.setAcceleration(targetAccel);

	pinMode(inAPin, OUTPUT);
	pinMode(inBPin, OUTPUT);

	pinMode(inputButton, INPUT);

	//attachInterrupt(inputButton, readPushedButton, HIGH);
}

void loop()
{
    int j;
    
    if (loopCount == 1000)
    {
        loopCount = 0;
        Serial.print("moveState: ");
        Serial.println(moveState);
        Serial.print("changeFlag: ");
        Serial.println(changeFlag);
    }
    else if (loopCount % 100 == 1)
    {
        Serial.println(loopCount);
        
    }
    loopCount++;
    //Serial.print("moveState: ");
    //Serial.println(moveState);
    //Serial.print("changeFlag: ");
    //Serial.println(changeFlag);

    //If a change in menu state has been flagged, call the genMenu() function to determine and execute what changed
    if (moveState)
	{
		moveGantry();
	}
    else if (changeFlag)
    {

        //Serial.println(changeFlag);
        genMenu(&activeMenu, &selectedButton, &selectFlag, &activeFrame);
        changeFlag = false;
        Serial.println("Success");
        delay(250);
    }
}







void runProgram(int progNum)
{
}

void editProgram(int progNum)
{
}







