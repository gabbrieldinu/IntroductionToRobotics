const int serialPort = 9600; // serial port

const int latchPin = 11; // STCP
const int clockPin = 10; // SHCP
const int dataPin = 12; // DS 

// displays pins
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int pinSW = 2; // joy sw pin
const int pinX = A0; // joy X axis 
const int pinY = A1; // joy y axis

const byte regSize = 8; // 8 bits to shift

int state = 2;

// joy variables
int minThreshold = 300;
int maxThreshold = 700;
int stationaryMinThreshold = 450;
int stationaryMaxThreshold = 650;
byte swReading = LOW; 
bool joyMoved = false;
int joyValueX = 0; 
int joyValueY = 0; 


int blinkDP = 0; // used for state of DP ( on / off )
unsigned long timerBlinkDP = 0; // timer for blinking
const int blinkInterval = 200; // interval between blinks

int multiplex = 0; // used for multiplexation state
unsigned long timerMultiplex = 0; // timer for multiplexation
const int multiplexInterval = 3; // interval between multplixes


int buttonPressed = 0; // check if button pressed
unsigned long timerLongPress = 0; // time for long press

int currentDisplay = 0;

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};

const int encodingsNumber = 16;

int encodingArray[encodingsNumber][regSize] = {
  {1, 1, 1, 1, 1, 1, 0, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1, 0},  // 2
  {1, 1, 1, 1, 0, 0, 1, 0},  // 3
  {0, 1, 1, 0, 0, 1, 1, 0},  // 4
  {1, 0, 1, 1, 0, 1, 1, 0},  // 5
  {1, 0, 1, 1, 1, 1, 1, 0},  // 6
  {1, 1, 1, 0, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1, 0},  // 8
  {1, 1, 1, 1, 0, 1, 1, 0},  // 9
  {1, 1, 1, 0, 1, 1, 1, 0},  // a
  {0, 0, 1, 1, 1, 1, 1, 0},  // b
  {1, 0, 0, 1, 1, 1, 0, 0},  // c
  {0, 1, 1, 1, 1, 0, 1, 0},  // d
  {1, 0, 0, 1, 1, 1, 1, 0},  // e
  {1, 0, 0, 0, 1, 1, 1, 0},  // f
};
const int minEncodingValue = 0; // min value
const int maxEncodingValue = encodingsNumber - 1; // max value 
const int displayCount = 4; // number of displays

// Array for the number on each display
int numberDisplayed[displayCount] = {
  0, 0, 0, 0
};

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++ ) 
  {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  Serial.begin(serialPort);
}

void writeReg(int encoding[]) 
{
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < regSize; i++) 
  {  
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, encoding[i]);
    digitalWrite(clockPin, HIGH);
  }

  digitalWrite(latchPin, HIGH);
}


void activateDisplay(int display)
{
  for (int i = 0; i < displayCount; i++)
  {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[display], LOW);
}

void setDpState(int state)
{
  for (int i = 0; i < encodingsNumber; i++)
  {
    encodingArray[i][regSize - 1] = state;
  }
}
void writeOnDisplay()
{
  int currentPosition = 0;
  int displayNumber = displayCount - 1;

  while (currentPosition < displayCount)
  {
    if (multiplex == 0)
    {
      timerMultiplex = millis();
      multiplex = 1;
    }
    if (currentDisplay == displayNumber)
    {
      if (blinkDP == 0)
      {
        timerBlinkDP = millis();
        blinkDP = 1;
      }
      if ( state == 1)
      {
        if (millis() - timerBlinkDP > blinkInterval && blinkDP == 1)
        {
          setDpState(1);
          blinkDP = 0;
        }
      }
      else    
      {
        setDpState(1);        
      }
    }
    else
    {
      setDpState(0);
    }
    
    writeReg(encodingArray[numberDisplayed[currentPosition]]);
    activateDisplay(displayNumber);

    if (millis() - timerMultiplex >= multiplexInterval)
    {
      currentPosition++;
      displayNumber--;
      multiplex = 0;
    }
  }
}


void changeDisplayNumber(int yValue)
{
  if (yValue < minThreshold && joyMoved == false ) 
  {
    numberDisplayed[displayCount - currentDisplay - 1]++;
    joyMoved = true;
  }
	if (yValue > maxThreshold && joyMoved == false ) 
  {
    numberDisplayed[displayCount-currentDisplay-1]--;
    joyMoved = true;
  }
  if (numberDisplayed[displayCount-currentDisplay-1] > maxEncodingValue )
     numberDisplayed[displayCount-currentDisplay-1] = minEncodingValue;

  if (numberDisplayed[displayCount-currentDisplay-1] < minEncodingValue)
     numberDisplayed[displayCount-currentDisplay-1] = maxEncodingValue;
     
	if (yValue > stationaryMinThreshold && yValue < stationaryMaxThreshold) 
  {
    joyMoved = false;
  }
}

void checkButtonPress()
{
  if (swReading == LOW && buttonPressed == 0) 
  {
    timerLongPress = millis(); 
    buttonPressed = 1;
  }

  if (millis() - timerLongPress >= 3000 && state == 1)
  {  
    for(int i = 0; i < 4 ; i++)
    {
      numberDisplayed[i] = 0;
    }
    currentDisplay = 0;
  }

  if (millis() - timerLongPress >= 100 && millis() - timerLongPress <= 3000 && swReading != LOW)
  {
    if (state == 1)
    {
      state = 2;
    }
    else
    {
      state = 1;
    }
  }

  if(swReading == HIGH)
  {
    timerLongPress = millis(); 
    buttonPressed = 0;
  }
}

void checkJoyMovementOnX(int xValue)
{
  if (xValue > maxThreshold && joyMoved == false) 
  {
    currentDisplay++;
    joyMoved = true;
  }
	if (xValue < minThreshold && joyMoved == false) 
  {
    currentDisplay--;
    joyMoved = true;
  }
  if ( currentDisplay > displayCount - 1)
    currentDisplay = 0;

  if ( currentDisplay < 0) 
    currentDisplay = displayCount-1;
    
	if (xValue > stationaryMinThreshold && xValue < stationaryMaxThreshold) 
  {
    joyMoved = false;
  }
}



void loop() 
{
  joyValueX = analogRead(pinY);
  joyValueY = analogRead(pinX);
  swReading = digitalRead(pinSW);

  if (state == 1)
  {
    checkJoyMovementOnX(joyValueX);
  }
  if (state == 2)
  {
    changeDisplayNumber(joyValueY);
  }

  writeOnDisplay();
  checkButtonPress();
}