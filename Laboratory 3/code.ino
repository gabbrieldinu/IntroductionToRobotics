
// declare all the joystick pins
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int segSize = 8;
int index = 0;
int directionX;
int directionY;

// modify if you have common anode
bool commonAnode = false; 

const int noOfDigits = 10;
int state = 1;
byte dpState = HIGH;
byte swState = HIGH;
byte lastSwState = HIGH;
byte segState = HIGH;
int xValue = 0;
int yValue = 0;

bool joyMovedOnX = false;
bool joyMovedOnY = false;
int currentSegPosition = 0;
int minThreshold = 400;
int maxThreshold = 600;
byte segmentOn = LOW;

unsigned long lastActionTime = 0;
unsigned long startTimer = -1;

class Segment{
  public:
    int pinNumber;
    int neighborUp;
    int neighborDown;
    int neighborLeft;
    int neighborRight;
    byte state;

    Segment()
    {
      this->pinNumber = 0;
      this->neighborUp = 0;
      this->neighborDown = 0;
      this->neighborLeft = 0;
      this->neighborRight = 0;
      this->state = LOW;

    }

    Segment(const int pinNumber, int neighborUp, int neighborDown, int neightborLeft, int neighborRight)
    {
      this->pinNumber = pinNumber;
      this->neighborUp = neighborUp;
      this->neighborDown = neighborDown;
      this->neighborLeft = neighborLeft;
      this->neighborRight = neighborRight;
      this->state = LOW;

    }

    void changeSegmentState()
    {
      this->state = !this->state;
    }

    int blinkSegment(byte on)
    {
      if (on == HIGH)
        return this->state;
      else
        return !this->state;
    }
};

Segment currentSegment;

Segment segments[segSize] = { 
  Segment(pinA,-1,6,5,1), Segment(pinB,0,6,5,-1), Segment(pinC,6,3,4,2), Segment(pinD,6,-1,4,2), Segment(pinE,6,3,-1,2), Segment(pinF,0,6,-1,1), Segment(pinG,0,3,-1,-1), Segment(pinDP,-1,-1,2,-1)
};




void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i].pinNumber, OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  
  Serial.begin(9600);
}

int checkJoyMovementOnX()
{
  int direction = 0;
  xValue = analogRead(pinX);
  
    if (xValue < minThreshold && joyMovedOnX == false) {
  
    direction = -1;
    joyMovedOnX = true;
  }
	if (xValue > maxThreshold && joyMovedOnX == false) {
    
    direction = 1;
    joyMovedOnX = true;
  }
  
	if (xValue > minThreshold && xValue < maxThreshold) {
    joyMovedOnX = false;
  }
 
  return direction;
}
int checkJoyMovementOnY()
{
  int direction = 0;
  yValue = analogRead(pinY);
    if (yValue < minThreshold && joyMovedOnY == false) {
  
    direction = -1;
    joyMovedOnY = true;
  }
	if (yValue > maxThreshold && joyMovedOnY == false) {
    
    direction = 1;
    joyMovedOnY = true;
  }
  
	if (yValue > minThreshold && xValue < maxThreshold) {
    joyMovedOnY = false;
  }
  return direction;
}

void loop() {
  // TODO: use joystick movement to cycle through the numbers from 0 to 9, with a state changer (aka if you move the joystick, only one increment should happen. The next change should be after the joystick went back to initial position and is moved again).
	
  swState = digitalRead(pinSW);

  directionX = checkJoyMovementOnX();
  directionY = checkJoyMovementOnY();
  

  for (int i = 0; i < segSize;i++)
  {
    digitalWrite(segments[i].pinNumber,segments[i].state);
  }
  currentSegment = segments[currentSegPosition];
  Serial.println(currentSegPosition);
  if (swState != lastSwState) 
  {


    if (swState == HIGH)
    {
      if (state == 1)
      {
        if (startTimer != -1)
      {
        if (millis()-startTimer>5000)
        {  
          for (int i = 0; i < segSize-1;i++)
          {
            segments[i].state = LOW;
          }
         currentSegPosition = segSize - 1;
        }
        else
        {
          state = 2;
        }
      }
      
      }
      else
      {
        state = 1;
      }
  
      
    }
    else
    {
      
      startTimer = millis();
    }
    
  }
  
  if (state == 1)
  {
    

    if (joyMovedOnX == true)
    {
      if (directionX == 1 && currentSegment.neighborRight != -1)
        {
          currentSegPosition = currentSegment.neighborRight;
        }
      else
      {
        if (directionX == -1 && currentSegment.neighborLeft != -1)
        {
          currentSegPosition = currentSegment.neighborLeft;
        }
      }
    }
    
    if (joyMovedOnY == true)
    {
      if (directionY == 1 && currentSegment.neighborUp != -1)
        {
          currentSegPosition = currentSegment.neighborUp;
        }
      else
      {
        if (directionY == -1 && currentSegment.neighborDown != -1)
        {
          currentSegPosition = currentSegment.neighborDown;
        }
      }
    }
      
    
  
    if (millis() - lastActionTime > 1000)
    {
      segmentOn = segments[currentSegPosition].blinkSegment(segmentOn);
      lastActionTime = millis();
    }
    
digitalWrite(currentSegment.pinNumber,segmentOn);

    
    
  }
  else
  {
    digitalWrite(currentSegment.pinNumber,currentSegment.state);
    if (joyMovedOnX == true)
      segments[currentSegPosition].changeSegmentState();
  
  }
  
	lastSwState = swState;
}
       