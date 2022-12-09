#include "LedControl.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>


const byte dinPin =12;
const byte clockPin =11;
const byte loadPin =10;
const byte matrixSize =8;
int dotX = 6;
int dotY = 6;
int playerX=0;
int playerY=0;
int score=0;
int xMovementGame;
int yMovementGame; 
bool joyMovedGame=false;
int stepsGame= 131;

// lcd display pins
const byte rs =9;
const byte en =8;
const byte d4 =7;
const byte d5 =6;
const byte d6 =5;
const byte d7 =4;

//joystick pins
const byte pinSW = 2;
const byte pinX = A4;
const byte pinY = A5;

const char about[] = "";
const char howToPlay[] = "";

int xValue = 0;
int yValue = 0;
int xMovement = 0;
int yMovement = 0;
const int minThreshold=300;
const int maxThreshold=800;
bool joyMovedOnY;
bool joyMovedOnX;
int menuPosition=0;
int settingsMenuPosition=0;
int barLevelMenuPosition=0;
int currentPosition=0;
int currentMenu=1;
int valueToAdjust=0;
char adjustmentName[20];
int adjustmentElementPos=0;

LedControl lc = LedControl(dinPin, clockPin, loadPin,1);
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

const int menuOptionsNumber = 6;
const int settingsMenuOptionsNumber = 4;
const int maxNumbersOfChars = 20;

int brightnessLevels[6]={0,50,100,150,200,250};
int currentBrightness=1;

byte fullMatrix[matrixSize] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

struct AdjustableSetting
{
  char name[12];
  int value;
};

// Menu
char menuOptions[menuOptionsNumber][maxNumbersOfChars] = {
  "---- Menu ----",
  " Start Game",
  " Settings ",
  " Highscore ",
  "How to play",
  "About"
};

AdjustableSetting menuSettingsOptions[settingsMenuOptionsNumber]= {
  {"Contrast", 1},
  {"Brightness",1},
  {"Matrix Bright ",1},
  {"",0}
};

bool matrix[8][8];

int i = 0;

int checkJoyMovementOnY()
{
  int dir=0;
  yValue = analogRead(pinY);
   if (yValue < minThreshold && joyMovedOnY == false ) 
  {
    dir= 1;
    joyMovedOnY = true;
  }

  if (yValue >= minThreshold && yValue <= maxThreshold && joyMovedOnY == true)
  {
    joyMovedOnY = false;
  }

  if (yValue > maxThreshold && joyMovedOnY == false ) 
  {
    
    joyMovedOnY = true;
    dir = -1;
  }
  return dir;
}
int checkJoyMovementOnX()
{
  int dir=0;
  xValue = analogRead(pinX);
  if (xValue >= minThreshold && xValue <= maxThreshold && joyMovedOnX==true)
  {
    joyMovedOnX = false;
  }
   if (xValue < minThreshold && joyMovedOnX == false ) 
  {
    dir = 1;
    joyMovedOnX = true;
  }

  if (xValue > maxThreshold && joyMovedOnX == false ) 
  {
    
    joyMovedOnX = true;
    dir = -1;
  }
  return dir;
}

void displayCurrentPosition()
{
  lcd.setCursor(0, currentPosition%2);
  lcd.print("*");
}

void displayRightMenu()
{
  lcd.setCursor(15, currentPosition%2);
  lcd.print(">");
}
void displayLeftMenu()
{
  lcd.setCursor(1, currentPosition%2);
  lcd.print("<");
}

void writeToScreen(char s1[], char s2[])
{
  lcd.setCursor(2,0);
  lcd.print(s1);
  lcd.setCursor(2,1);
  lcd.print(s2);
}

void displayLevelBar(int value)
{
  int offset = 3;
  for (int i = offset;i < value + offset;i++)
  {
      lcd.setCursor(i,1);
      lcd.print("X");
  }
  
}

void mainMenu()
{
  if (currentPosition % 2 == 0)
    writeToScreen(menuOptions[currentPosition],menuOptions[currentPosition+1]);
  else
    writeToScreen(menuOptions[currentPosition-1],menuOptions[currentPosition]);
  displayCurrentPosition();
  if (currentPosition == 1)
  {
    startGame();
  }
  if (currentPosition == 2)
  {
      xMovement = checkJoyMovementOnX();
      displayRightMenu();
      if (xMovement == 1)
        {
          currentMenu = 2;
          currentPosition = 0;
          menuPosition = 0;
        }
  }
  yMovement = checkJoyMovementOnY();
  if(yMovement == -1 && currentPosition < menuOptionsNumber - 1 )
  {
    currentPosition++;
    lcd.clear();
  }
  else
  {
    if(yMovement == 1 && currentPosition > 0)
    {
        currentPosition--;
      lcd.clear();
    }
      
  }

    
}

void settingsMenu()
{
   if (currentPosition % 2 == 0)
    writeToScreen(menuSettingsOptions[currentPosition].name,menuSettingsOptions[currentPosition+1].name);
  else
    writeToScreen(menuSettingsOptions[currentPosition-1].name,menuSettingsOptions[currentPosition].name);
  
  displayCurrentPosition();
  displayLeftMenu();
  displayRightMenu();

  yMovement = checkJoyMovementOnY();
  xMovement = checkJoyMovementOnX();

  if(yMovement == -1 && currentPosition < settingsMenuOptionsNumber - 2)
  {
    currentPosition++;
    lcd.clear();
  }
  else
  {
    if(yMovement == 1 && currentPosition > 0)
    {
      currentPosition--;
      lcd.clear();
    }
      
  }
  
  if (xMovement != 0)
  {
    lcd.clear();
    if (xMovement == -1)
      currentMenu = 1;
    else
    {
      currentMenu = 3;
      for (int i = 0; i < 20; i++)
        adjustmentName[i] = menuSettingsOptions[currentPosition].name[i];
      adjustmentElementPos = currentPosition;
    }
      
    currentPosition = 0;
    menuPosition = 0;
    lcd.clear();
  }
}

void displayAdjustmentSigns()
{
  lcd.setCursor(2,1);
  lcd.print("-");
  lcd.setCursor(13,1);
  lcd.print("+");
}
void adjustmentMenu(char name[], int pos)
{ 
  Serial.println(name);
  writeToScreen(name,"");
  displayLevelBar(menuSettingsOptions[pos].value*2);
  displayCurrentPosition();

  xMovement = checkJoyMovementOnX();
  yMovement = checkJoyMovementOnY();
  Serial.println(yMovement);

  if(yMovement == -1 && currentPosition < 1)
  {
    currentPosition ++;
    lcd.clear();
  }
  else
  {
    if(yMovement == 1 && currentPosition > 0 )
    {
      currentPosition -- ;
      lcd.clear();
    }
      
  }

  if (currentPosition == 0)
  {
    displayLeftMenu();
     if (xMovement == -1 )
    {
      currentMenu = 2 ;
      currentPosition = 0;
      lcd.clear();
    }
    

  }
  else
  {
    
    displayAdjustmentSigns();
    if (xMovement == -1 &&  menuSettingsOptions[pos].value > 1)
    {
      menuSettingsOptions[pos].value --;
      lcd.clear();
    }
    if (xMovement == 1 && menuSettingsOptions[pos].value < 5)
    {
      menuSettingsOptions[pos].value ++ ;
      lcd.clear();
    }
  }
  
}


// Game

void turnPixel(int posX, int posY, int value) {
  matrix[posX][posY] = value;
  lc.setLed(0, posX, posY, matrix[posX][posY]);
}

void startGame()
{

}

bool timeUp()
{
  return true;
}

void game() {
  turnPixel(playerX , playerY , 1);
  turnPixel(dotX, dotY, 1);
  int initialPlayerX = playerX;
  int initialPlayerY = playerY;
  xMovementGame = checkJoyMovementOnX();
  yMovementGame = checkJoyMovementOnY();


  playerX += xMovementGame; // if the player holds the joystick in a position it will continously register that instead of only one instance
  playerY += yMovementGame;
  
  if (playerX < 0)
    playerX = matrixSize - 1;
  if (playerY < 0)
    playerY = matrixSize - 1;
  playerX %= matrixSize;
  playerY %= matrixSize;
  if (playerX % matrixSize == dotX && playerY % matrixSize == dotY) {
    score++;
  }
  if ( initialPlayerX != playerX || initialPlayerY != playerY)
    turnPixel(initialPlayerX, initialPlayerY, 0);
  if (timeUp() == true)
    finishGame();
}

void finishGame()
{

}

void setup() 
{
  lcd.begin(16,2);// Print a message to the LCD.
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 3);
  lc.clearDisplay(0);
  }
  void loop() 
  {
    if (currentMenu == 1)
    {
      mainMenu();
    }
    else
    {
      if (currentMenu == 2)
      {
        settingsMenu();
      }
      else
      {
        if (currentMenu == 3)
        {
          adjustmentMenu(adjustmentName,adjustmentElementPos);
        }
      }
    }
  }
    

