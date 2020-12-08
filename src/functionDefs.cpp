#ifndef FUNCTIONDEFS
#include "functionDefs.h"
#endif

#ifndef MOTORCONTROL
#include "MotorControl.h"
#endif


SnoozeDigital digitalWakeUp;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
SnoozeBlock config(digitalWakeUp);

float p = 3.141562654;
int selectedButton = 0;
volatile boolean changeFlag;
volatile int changeButtonDir = 0;
int activeMenu = 0;
int activeFrame = 0;
int menuHeight = 0;
int numButtDisplayed = 3;
int *ptr = &numButtDisplayed;
boolean selectFlag = false;
boolean bottFlag = false;
boolean topFlag = false;

char menuName[200] = "";

int frameCounts[3] = {1, 2, 2};

char *menuButt[200] = {
    "Run Programs",
    "Edit Programs",
    "Calibrate",
    "SHUT DOWN",
};

//Determines which button was pushed and then calls the appropriate menu regeneration function
void genMenu(int* actMenu, int* selButt, boolean* selFlag, int* actFrame)
{
    //Serial.println("Selecting menu printing stage");
    //Serial.println(selFlag);
    //If the green SELECT button was pushed, determine what screen to load
    if (*selFlag)
    {
        //If the user selected the first button on the MAIN menu, load the RUN menu.
        if (*actMenu == 0)
        {
            if (*selButt == 0)
            {
                *actMenu = 1;
                *selButt = 0;
                tft.fillScreen(ST77XX_BLACK);
                blankMenu(); //Reset the button names
                genRunMenu(*selButt);
                *selFlag = false;
            }
            else if (*selButt == 1)
            {
                *actMenu = 2;
                *selButt = 0;
                tft.fillScreen(ST77XX_BLACK);
                blankMenu(); //Reset the button names
                genEditMenu(selButt, &numButtDisplayed);
                *selFlag = false;
            }
            else if (*selButt == 2)
            {
                *actMenu = 3;
                *selButt = 0;
                tft.fillScreen(ST77XX_BLACK);
                blankMenu(); //Reset the button names
                genCalibrateMenu();
                *selFlag = false;
            }

        }
        else if (*actMenu == 1)
        {
            if (*selButt == 0)
            {
                progNum = 0;
                runMoveProg();
            }
            else if (*selButt == 1)
            {
            }
            else if (*selButt == 2)
            {
            }
            else if (*selButt == 3)
            {
            }
            else if (*selButt == 4)
            {
            }
            else if (*selButt == 5)
            {
                *actMenu = 0;
                *selButt = 0;
                //tft.fillScreen(ST77XX_BLACK);();
                numButtDisplayed = 3; //Reset the number of buttons to be displayed on the main menu
                tft.fillScreen(ST77XX_BLACK);
                blankMenu(); //Reset the button names
                *actFrame = 0;
                genMainMenu(*selButt);
                *selFlag = false;
            }
        }
        else if (*actMenu == 2)
        {
            if (*selButt == 0)
            {
            }
            else if (*selButt == 1)
            {
            }
            else if (*selButt == 2)
            {
            }
            else if (*selButt == 3)
            {
            }
            else if (*selButt == 4)
            {
            }
            else if (*selButt == 5)
            {
                *actMenu = 0;
                *selButt = 0;
                //tft.fillScreen(ST77XX_BLACK);();
                numButtDisplayed = 3; //Reset the number of buttons to be displayed on the main menu
                tft.fillScreen(ST77XX_BLACK);
                blankMenu(); //Reset the button names
                *actFrame = 0;
                genMainMenu(*selButt);
                *selFlag = false;
            }
        }
        else if (*actMenu == 3)
        {
            if (*selButt == 0)
            {
            }
            else if (*selButt == 1)
            {
                *actMenu = 0;
                *selButt = 0;
                //tft.fillScreen(ST77XX_BLACK);();
                numButtDisplayed = 3; //Reset the number of buttons to be displayed on the main menu
                tft.fillScreen(ST77XX_BLACK);
                blankMenu(); //Reset the button names
                *actFrame = 0;
                genMainMenu(*selButt);
                *selFlag = false;
            }
        }
    }
    else
    {
        //Serial.println("Select menu to print");
        //Serial.println(actMenu);
        if (*actMenu == 0)
        {
            //Serial.println("Print main menu");
            genMainMenu(*selButt);
        }
        else if (*actMenu == 1)
        {
            //Serial.println("Print run menu");
            genRunMenu(*selButt);
        }
        else if (*actMenu == 2)
        {
            //Serial.println("Print run menu");
            genEditMenu(selButt, &numButtDisplayed);
        }
        else if (*actMenu == 3)
        {
            //Serial.println("Print run menu");
            genCalibrateMenu();
        }
    }
}

void readPushedButt()
{
    Serial.println("Pushed");

    if (!changeFlag)
    {
        //If the user pushed the up button, move the menu selection up and flag that a change has occurred
        if (digitalRead(upArrButt))
        {
            selectedButton--;
            changeFlag = true;
            changeButtonDir = -1;
        }
        //If the user pushed the down button, move the menu selection down and flag that a change has occurred
        else if (digitalRead(downArrButt))
        {
            Serial.print("frameCounts[1]: ");
            Serial.println(frameCounts[1]);
            selectedButton++;
            //Serial.print("selectedButton: ");
            //Serial.println(selectedButton);
            changeFlag = true;
            changeButtonDir = 1;
            //Serial.println("Down button");
        }
        //Flag that the user wants to select an on-screen button and prevent the menu from moving up or down
        else if (digitalRead(greenButt))
        {
            changeFlag = true;
            selectFlag = true;
            changeButtonDir = 0;
        }
    }
}

char **getMenuText(int currMenu)
{
    //menuButt[0] = "TEST 1";
    if (currMenu == 0)
    {
        menuButt[0] = "Run Programs";
        menuButt[1] = "Edit Programs";
        menuButt[2] = "Calibrate";
        //menuButt[3] = "SHUT DOWN";
    }
    else if (currMenu == 1)
    {
        menuButt[0] = "Program 1";
        menuButt[1] = "Program 2";
        menuButt[2] = "Program 3";
        menuButt[3] = "Program 4";
    }
    return menuButt;
}

void blankMenuButtons(int menuLen, int xCorner, int yCorner)
{
    //Serial.println("Blanking");
    int i;
    for (i = 0; i < 4; i++)
    {
        tft.setCursor(xCorner, yCorner + (55 * i));
        tft.fillRoundRect(xCorner - 5, yCorner + (55 * i) - 30 + (menuHeight * 20), 220, 35, 5, ST77XX_BLACK);
    }
}

void blankMenu()
{
    for (int i = 0; i < 200; i++)
    {
        menuButt[i] = "";
    }
}

void printMenu(int choseButt, int menuLength, int menuNum)
{
    //Serial.print("numButtDisplayed: ");
    //Serial.println(numButtDisplayed);
    int i;
    //Define the starting point of any menu
    int xCorner = 15;
    int yCorner = 50;

    //numButtDisplayed = 3;

    if (selectedButton == numButtDisplayed + activeFrame)
    {
        if (numButtDisplayed + activeFrame < menuLength)
        {
            activeFrame++;
            selectFlag = true;
        }
    }
    else if (selectedButton < activeFrame)
    {
        if (activeFrame > 0)
        {
            //Serial.println("Frame changed");
            activeFrame--;
            selectFlag = true;
        }
    }

    if (selectFlag)
    {
        blankMenuButtons(menuLength, xCorner, yCorner);
        selectFlag = false;
    }

    tft.setCursor(xCorner, 10);
    writeText(menuName, ST77XX_BLUE, 2);

    for (i = 0; i < numButtDisplayed; i += 1)
    {

        if (bottFlag & changeButtonDir == 1)
        {
            //Serial.println("Beyond lower bound");
            //Serial.print("selectedButton: ");
            //Serial.println(selectedButton);
            //Serial.print("menuLength - 1: ");
            //Serial.println(menuLength - 1);
            if (selectedButton > menuLength - 1)
            {
                selectedButton = menuLength - 1;
            }
            //Serial.print("i + activeFrame: ");
            //Serial.println(i + activeFrame);
            //Serial.print("selectedButton: ");
            //Serial.println(selectedButton);
        }
        else if ((selectedButton < activeFrame & changeButtonDir == -1) & i == 0)
        {
            Serial.println("Beyond upper bound");
            if (selectedButton < 0)
            {
                selectedButton = 0;
            }
        }
        else if (i + activeFrame == selectedButton)
        {
            //Serial.print("menuHeight: ");
            //Serial.println(menuHeight);
            tft.fillRoundRect(xCorner - 5, yCorner - 20 + (55 * i) - 10 + (menuHeight * 20), 220, 35, 5, 0x96C6);

            //Serial.print("i + activeFrame: ");
            //Serial.println(i + activeFrame);

            //Serial.print("menuLength - 1: ");
            //Serial.println(menuLength - 1);

            if (i + activeFrame == menuLength - 1)
            {
                bottFlag = true;
                topFlag = false;
                //Serial.println(bottFlag);
            }
            else if (i == 0)
            {
                topFlag = true;
                bottFlag = false;
                //Serial.print("topFlag: ");
                //Serial.println(topFlag);
            }
            else
            {
                bottFlag = false;
                topFlag = false;
            }
        }
        else if (i + activeFrame == selectedButton - 1 & changeButtonDir == 1)
        {
            tft.fillRoundRect(xCorner - 5, yCorner + (55 * i) - 30 + (menuHeight * 20), 220, 35, 5, ST77XX_BLACK);
            //Serial.println("Filling button above with black");
        }
        else if (i + activeFrame == selectedButton + 1 & changeButtonDir == -1)
        {
            tft.fillRoundRect(xCorner - 5, yCorner + (55 * i) - 30 + (menuHeight * 20), 220, 35, 5, ST77XX_BLACK);
            //Serial.println("Filling button below with black");
        }

        if (i + 1 + activeFrame <= frameCounts[menuNum] + 4)
        {
            Serial.println("printMenu Stage 3ab");
            tft.setCursor(xCorner, yCorner + (55 * i) + (menuHeight * 20) - 20);
            Serial.println(menuButt[i + activeFrame]);
            testDrawText(menuButt[i + activeFrame], ST77XX_BLUE, 2);
            testroundrects(xCorner - 10, yCorner + (55 * i) - 35 + (menuHeight * 20));
        }

        Serial.print("activeFrame: ");
        Serial.println(activeFrame);
        Serial.print("menuLength: ");
        Serial.println(menuLength);
        Serial.print("numButtDisplayed: ");
        Serial.println(numButtDisplayed);
        if (menuLength <= numButtDisplayed)
        {
            //tft.fillTriangle(220, 255 - (menuHeight * 20), 205, 240 - (menuHeight * 20), 235, 240 - (menuHeight * 20), ST77XX_BLACK);
            tft.fillTriangle(220, 10, 205, 25, 235, 25, ST77XX_BLACK);
        }
        else if (activeFrame == 0 & topFlag)
        {
            tft.fillTriangle(220, 235, 205, 220, 235, 220, ST77XX_WHITE);
            tft.fillTriangle(220, 10, 205, 25, 235, 25, ST77XX_BLACK);
        }
        else if (activeFrame > 0 & activeFrame < menuLength - numButtDisplayed)
        {
            tft.fillTriangle(220, 10, 205, 25, 235, 25, ST77XX_WHITE);
            tft.fillTriangle(220, 235, 205, 220, 235, 220, ST77XX_WHITE);
        }
        else if (activeFrame == menuLength - numButtDisplayed)
        {
            tft.fillTriangle(220, 10, 205, 25, 235, 25, ST77XX_WHITE);
            tft.fillTriangle(220, 235, 205, 220, 235, 220, ST77XX_BLACK);
        }

        Serial.println("\r");
    }
}

void genMainMenu(int choseButt)
{
    int mainMenuLength = 3;
    menuHeight = 1;
    strcpy(menuName, "Main Menu");

    menuButt[0] = "Run Programs";
    menuButt[1] = "Edit Programs";
    menuButt[2] = "Calibrate";
    //menuButt[3] = "SHUT DOWN";

    printMenu(choseButt, mainMenuLength, 0);
}

//Generates the menu for running programs.
//Takes as input the GUI button that is currently highlighted, the frame that is displayed,
//and the direction that the user moved the GUI button selection.
void genRunMenu(int choseButt)
{
    int runMenuLength = 6;
    menuHeight = 1;
    numButtDisplayed = 3;
    //Serial.print("numButtDisplayed: ");
    //Serial.println(numButtDisplayed);
    strcpy(menuName, "Edit Programs");

    menuButt[0] = "Program 1";
    menuButt[1] = "Program 2";
    menuButt[2] = "Program 3";
    menuButt[3] = "Program 4";
    menuButt[4] = "Program 5";
    //Serial.print("numButtDisplayed: ");
    //Serial.println(numButtDisplayed);
    menuButt[5] = "BACK";

    printMenu(choseButt, runMenuLength, 1);
}

void genEditMenu(int *choseButt, int *numButtDisplayed)
{
    int runMenuLength = 6;
    *numButtDisplayed = 3;
    menuHeight = 1;

    strcpy(menuName, "Edit Programs");

    menuButt[0] = "Program 1";
    menuButt[1] = "Program 2";
    menuButt[2] = "Program 3";
    menuButt[3] = "Program 4";
    menuButt[4] = "Program 5";
    menuButt[5] = "BACK";

    printMenu(*choseButt, runMenuLength, 2);
}

void genCalibrateMenu()
{
    int xCorner = 5;
    int yCorner = 50;
    int calibrateMenuLength = 2;
    numButtDisplayed = 2;
    menuHeight = 4;
    strcpy(menuName, "Use the left and right arrow buttons to move gantry to starting position:");

    menuButt[0] = "Accept calibration";
    menuButt[1] = "BACK";
    menuButt[2] = "";
    menuButt[3] = "";
    menuButt[4] = "";
    menuButt[5] = "";

    printMenu(0, calibrateMenuLength, 3);
}

//This function takes as input: the physical button the user pushed, the menu currently displayed, and the button highlighted on the menu.
//It evaluates whether to open a new menu, allow for program entry, run a program, etc.
void selectButton(int pushButt, int menu, int selButt)
{

    if (changeButtonDir == 0)
    {
    }
    else
    {
    }

    //Main menu
    if (menu == 0)
    {
        //If the green select button was pushed, execute the button highlighted on the menu
        //Otherwise, call a function to move move the selected menu button
        //Red button does nothing on this menu
        if (pushButt == 0)
        {
            genMainMenu(selButt);
        }
        else if (pushButt == 1)
        {
            genMainMenu(selButt);
        }
        else if (pushButt == 3)
        {
            menu += 1 + 1 * selButt;
            getMenuText(menu);
            //genMenu(menuButt, 4, selButt);
            tft.fillScreen(ST77XX_BLACK);
            genRunMenu(selButt);
        }
        else
        {
            genMainMenu(selButt);
        }
    }
    else if (menu == 0) //Run Program menu
    {
        //If the green select button was pushed, execute the button highlighted on the menu
        //Otherwise, call a function to move move the selected button
        //Red button does nothing on this menu
        if (pushButt == 0)
        {
            genRunMenu(selButt);
        }
        else if (pushButt == 1)
        {
            genRunMenu(selButt);
        }
        else if (pushButt == 3)
        {
            menu += 1 + 1 * selButt;
            getMenuText(menu);
            //genMenu(menuButt, 4, selButt);
            tft.fillScreen(ST77XX_BLACK);
            genRunMenu(selButt);
        }
        else
        {
            genMainMenu(selButt);
        }
    }
}

void testDrawText(char *text, uint16_t color, int textSize)
{

    tft.setTextSize(textSize);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.println(text);
}

void writeText(char *text, uint16_t color, int textSize)
{
    int i = 0;
    int j = 0;
    int charCounter = 0;
    int wordLen = 0;
    int lineCharCounter = 0;
    int menuLen = strlen(text);

    tft.setTextSize(textSize);
    tft.setTextColor(color);
    //tft.setTextWrap(true);

    Serial.println("Beginning print loop");
    //Serial.print("wordLen: ");
    //Serial.println(wordLen);

    for (i = 0; i < menuLen; i++)
    {
        Serial.print("i: ");
        Serial.println(i);
        //If the space does not occur at the end of a line, print it
        if (text[i] == ' ')
        {
            if (i % 19 == 1)
            {
                //Skip over this character, as a space should not appear at the start of a line.
                charCounter++;
            }
            else if (i % 19 != 0)
            {
                tft.print(text[i]);
                charCounter++;
                lineCharCounter++;
            }
            else
            {
                tft.print('\n');
                lineCharCounter = 0;
            }
        }
        else if (text[i] == '\0')
        {
            tft.println(text[charCounter]); //Print the final character
            i = menuLen;                    //Exit the loop
        }
        else
        {
            while (text[i + j] != ' ' & text[i + j] != '\0')
            {
                wordLen++;
                j++;
                Serial.print("wordLen: ");
                Serial.println(wordLen);
                Serial.println(text[i + j]);
            }

            Serial.print("wordLen: ");
            Serial.println(wordLen);
            Serial.print(" 19 - (lineCharCounter % 19): ");
            Serial.println(19 - (lineCharCounter % 19));
            if (wordLen < (19 - (lineCharCounter % 19)) & (lineCharCounter % 19) != 0)
            {
                Serial.print("lineCharCounter % 19: ");
                Serial.println(lineCharCounter % 19);
                tft.print(text[i]);
                Serial.println(text[i]);
                lineCharCounter++;
            }
            else if (lineCharCounter == 0)
            {
                tft.print(text[i]);
                Serial.println(text[i]);
                lineCharCounter++;
            }
            else
            {
                tft.print('\n');
                //i = i + (i % 19);
                Serial.print("i: ");
                Serial.println(i);
                tft.print(text[i]);
                lineCharCounter = 0;
            }
            Serial.print("i: ");
            Serial.println(i);
            wordLen = 0;
            j = 0;
            //lineCharCounter = 0;
        }
    }
    //tft.println(text);
}

void testroundrects(int x, int y)
{
    //tft.fillScreen(ST77XX_BLACK);
    uint16_t color = 100;
    int i;
    int t;
    //for(t = 0 ; t <= 4; t+=1) {
    //int x = 5;
    //int y = 5;
    int w = 230;
    int h = 45;
    for (i = 0; i <= 4; i += 1)
    {
        tft.drawRoundRect(x, y, w, h, 5, ST77XX_BLUE);
        x += 1;
        y += 1;
        w -= 2;
        h -= 2;
        //color+=1100;
    }
    //color+=100;
    //}
    //Serial.println("end of testroundrects()");
}

void drawUnderline(int xCorn, int yCorn, int strLength, uint16_t col)
{
    int i;
    //A character in size 2 font is ~11.67 pixels wide, on average. Multiply number of characters by
    //12 to generate underline
    for (i = 0; i < 4; i++)
    {
        tft.drawLine(xCorn - 3, yCorn + 20 + 1 * i, xCorn + 12 * strLength, yCorn + 20 + 1 * i, ST77XX_RED);
    }
}
