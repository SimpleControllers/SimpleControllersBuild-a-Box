#include "Nintendo.h"
/* This code uses the Nicohood Library
 * Use this code at your own risk
 * Code written by Simple Controllers and this code is open source.
 * Meaning its free to share, improve on, or anything you like!
 * Just remember to mention you used my code!
 */
//This makes the controller bidirection data line on pin number8
CGamecubeConsole GamecubeConsole(8);      //Defines a "Gamecube Console" sending data to the console on pin 8
Gamecube_Data_t d = defaultGamecubeData;   //Structure for data to be sent to console

//This is needed but you don't need a controller on pin 7
CGamecubeController GamecubeController1(7);

//This is the pinout of the controller.  Can be changed to your liking.  I may have mixed up some of the tilt pins but you can change that physically in your build or through the code.  Just do test runs along the way.
const int A = 22;
const int B = 24;
const int X = 26;
const int Y = 28;
const int Z = 30;
const int START = 31;

const int R = 34;
const int L = 35;
const int RLIGHT = 36;
//This is the value of analog shielding 74 is lightest possible on gamecube.  It varies from gamecube to dolphin no idea why.
const int RLIGHTv = 74;

const int LEFT = 38;
const int RIGHT = 39;
const int UP = 40;
const int DOWN = 41;

//NEW!! Fixed the mixup of X1 and X2 buttons, they now correspond to the correct buttons.  
//If you are updating from 1.0/1.1 you might have to change the internal pins on your box or just swap the pin numbers here.
const int X1 = 44;
const int X2 = 45;
const int Y1 = 46;
const int Y2 = 47;

//This is analog tilt modifiers and can be changed to your liking
const int X1v = 27;
const int X2v = 55;
const int X3v = 73;

const int Y1v = 27;
const int Y2v = 53;
const int Y3v = 74;

const int CLEFT = 48;
const int CRIGHT = 49;
const int CUP = 50;
const int CDOWN = 51;

//THIS IS THE SWITCH/BUTTON TO TOGGLE MODIFIERS (X1, X2, Y1, Y2) TO DPAD
const int SWITCH = 12;

void setup()
{
  //This is establishing the pin assignments up there to input pins
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  pinMode(X, INPUT_PULLUP);
  pinMode(Y, INPUT_PULLUP);
  pinMode(Z, INPUT_PULLUP);
  pinMode(START, INPUT_PULLUP);
  
  pinMode(R, INPUT_PULLUP);
  pinMode(L, INPUT_PULLUP);
  pinMode(RLIGHT, INPUT_PULLUP);
  
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  
  pinMode(X1, INPUT_PULLUP);
  pinMode(X2, INPUT_PULLUP);
  pinMode(Y1, INPUT_PULLUP);
  pinMode(Y2, INPUT_PULLUP);
  
  pinMode(CLEFT, INPUT_PULLUP);
  pinMode(CRIGHT, INPUT_PULLUP);
  pinMode(CUP, INPUT_PULLUP);
  pinMode(CDOWN, INPUT_PULLUP);

  pinMode(SWITCH, INPUT_PULLUP);

  //This is needed to run the code.
  GamecubeController1.read();
}

void loop()
{
  //This resets and establishes all the values after the controller sends them to the console and helps with initial "zeroing"
  int pinA = 0;
  int pinB = 0;
  int pinX = 0;
  int pinY = 0;
  int pinZ = 0;
  int pinSTART = 0;

  int pinR = 0;
  int pinL = 0;
  int pinRLIGHT = 0;

  int pinLEFT = 0;
  int pinRIGHT = 0;
  int pinUP = 0;
  int pinDOWN = 0;

  int pinX1 = 0;
  int pinX2 = 0;
  int pinY1 = 0;
  int pinY2 = 0;

  int pinCLEFT = 0;
  int pinCRIGHT = 0;
  int pinCUP = 0;
  int pinCDOWN = 0;

  int pinxAxisC = 128;
  int pinyAxisC = 128;
  
  int pinxAxis = 128;
  int xmod = 0;
  int pinyAxis = 128;

  int rightOne = 0;
  int leftOne = 0;
  int downOne = 0;

  int pinSWITCH = 0;

  //This reads control stick as neutral when both left/right or up/down is pressed at the same time.  Also sets parameters for the diffrent analog tilt modifiers IE: X1+X2 = X3
  //UPDATE: NOW CORRESPONDS TO PROPER SMASHBOX ANGLES
  if (digitalRead(LEFT) == HIGH && digitalRead(RIGHT) == LOW){
    pinxAxis = 128+86;
    if (digitalRead(X1) == LOW && digitalRead(X2) == HIGH)pinxAxis = X1v + 128;
    if (digitalRead(X1) == HIGH && digitalRead(X2) == LOW)pinxAxis = X2v + 128;
    if (digitalRead(X1) == LOW && digitalRead(X2) == LOW)pinxAxis = X3v + 128;
    rightOne = 1;
  }
  if (digitalRead(LEFT) == LOW && digitalRead(RIGHT) == HIGH){
    pinxAxis = 128-86;
    if (digitalRead(X1) == LOW && digitalRead(X2) == HIGH)pinxAxis = 128 - X1v;
    if (digitalRead(X1) == HIGH && digitalRead(X2) == LOW)pinxAxis = 128 - X2v;
    if (digitalRead(X1) == LOW && digitalRead(X2) == LOW)pinxAxis = 128 - X3v;
    leftOne = 1;
  }
    
  if (digitalRead(DOWN) == HIGH && digitalRead(UP) == LOW){
    pinyAxis = 128+86;
    if (digitalRead(Y1) == LOW && digitalRead(Y2) == HIGH)pinyAxis = 128 + Y1v;
    if (digitalRead(Y1) == HIGH && digitalRead(Y2) == LOW)pinyAxis = 128 + Y2v;
    if (digitalRead(Y1) == LOW && digitalRead(Y2) == LOW)pinyAxis = 128 + Y3v;
  }
  if (digitalRead(DOWN) == LOW && digitalRead(UP) == HIGH){
    pinyAxis = 128-86;
    if (digitalRead(Y1) == LOW && digitalRead(Y2) == HIGH)pinyAxis = 128 - Y1v;
    if (digitalRead(Y1) == HIGH && digitalRead(Y2) == LOW)pinyAxis = 128 - Y2v;
    if (digitalRead(Y1) == LOW && digitalRead(Y2) == LOW)pinyAxis = 128 - Y3v;
    downOne = 1;
  }

  //NEW: Axe Shield Drops
  if (digitalRead(X1) == HIGH && digitalRead(X2) == HIGH && digitalRead(Y1) == HIGH && digitalRead(Y2) == HIGH && downOne == 1 && leftOne == 1){
    pinxAxis = 128-80;
    pinyAxis = 128-78;
  }

  if (digitalRead(X1) == HIGH && digitalRead(X2) == HIGH && digitalRead(Y1) == HIGH && digitalRead(Y2) == HIGH && downOne == 1 && rightOne == 1){
    pinxAxis = 128+80;
    pinyAxis = 128-78;
  }

  //Reads CStick pins, same logic as controlstick values.
  if (digitalRead(CLEFT) == HIGH && digitalRead(CRIGHT) == LOW)pinxAxisC = 255;
  if (digitalRead(CLEFT) == LOW && digitalRead(CRIGHT) == HIGH)pinxAxisC = 0;
  if (digitalRead(CDOWN) == HIGH && digitalRead(CUP) == LOW)pinyAxisC = 255;
  if (digitalRead(CDOWN) == LOW && digitalRead(CUP) == HIGH)pinyAxisC = 0;

  if (digitalRead(A) == LOW)pinA = 1;
  if (digitalRead(B) == LOW)pinB = 1;
  if (digitalRead(X) == LOW)pinX = 1;
  if (digitalRead(Y) == LOW)pinY = 1;
  if (digitalRead(Z) == LOW)pinZ = 1;
  if (digitalRead(START) == LOW)pinSTART = 1;
  //This is for analog shield
  if (digitalRead(RLIGHT) == LOW)pinRLIGHT = RLIGHTv;
  //This is for digital shield
  if (digitalRead(R) == LOW)pinR = 1;
  if (digitalRead(L) == LOW)pinL = 1;

  if (digitalRead(SWITCH) == LOW)pinSWITCH = 1;

  d.report.dup = 0;
  d.report.dright = 0;
  d.report.ddown = 0;
  d.report.dleft = 0;
    
  //NEW WHEN SWITCH/BUTTON ON PIN 12 IS PRESSED/ACTIVATED SWAPS X1,X2,Y1,Y2 TO DPAD 
  if (pinSWITCH == 1){
    if(digitalRead(X1) == LOW)d.report.dleft = 1;
    if(digitalRead(X2) == LOW)d.report.ddown = 1;
    if(digitalRead(Y1) == LOW)d.report.dup = 1;
    if(digitalRead(Y2) == LOW)d.report.dright = 1;
  }
  


  //reports data
  d.report.a = pinA;
  d.report.b = pinB;
  d.report.x = pinX;
  d.report.y = pinY;
  d.report.z = pinZ;
  d.report.start = pinSTART;
  d.report.r = pinR;
  d.report.l = pinL;
  d.report.left = pinRLIGHT;
  d.report.xAxis = pinxAxis;
  d.report.yAxis = pinyAxis;
  d.report.cxAxis = pinxAxisC;
  d.report.cyAxis = pinyAxisC;
  //sends the complied data to console when console polls for the input
  GamecubeConsole.write(d);

}

