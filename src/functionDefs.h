#ifndef FUNCTIONDEFS_H
#define FUNCTIONDEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <array>
#include <iostream>
#include <Snooze.h>

#define TFT_CS 10
#define TFT_RST 14
#define TFT_DC 8

//Assign the pins for the control panel buttons
#define upArrButt 21
#define rightArrButt 18
#define downArrButt 20
#define leftArrButt 19
#define greenButt 17
#define redButt 16


extern SnoozeDigital digitalWakeUp;

extern Adafruit_ST7789 tft;
extern SnoozeBlock config;

extern float p;
extern int selectedButton;
extern volatile boolean changeFlag;
extern volatile int changeButtonDir;
extern int activeMenu;
extern int activeFrame;
extern int menuHeight;
extern int numButtDisplayed;
extern int *ptr;
extern boolean selectFlag;
extern boolean bottFlag;
extern boolean topFlag;

extern int frameCounts[];
extern char menuName[];
extern char *menuButt[];


void testDrawText(char *text, uint16_t color, int textSize);
void testroundrects(int x, int y);
void genMenu(int* actMenu, int* selButt, boolean* selFlag, int* actFrame);
void selectButton(int selButt, int menu, int pushButt);
char **getMenuText(int currMenu);
void genMainMenu(int choseButt);
void genRunMenu(int choseButt);
void drawUnderline(int xCorn, int yCorn, int strLength, uint16_t col);
void readPushedButt();
void printMenu(int choseButt, int menuLength, int menuNum);
void blankMenuButtons(int menuLen, int xCorner, int yCorner);
void genEditMenu(int* choseButt, int* numButtDisplayed);
void runProgram(int progNum);
void editProgram(int progNum);
void genCalibrateMenu();
void blankMenu();
void writeText(char *text, uint16_t color, int textSize);

#endif