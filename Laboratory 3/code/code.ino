
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

// declaration of segement positions in array
const int noSeg = -1;
const int segA = 0;
const int segB = 1;
const int segC = 2;
const int segD = 3;
const int segE = 4;
const int segF = 5;
const int segG = 6;
const int segDP = 7;

// segment array size
const int segSize = 8;

// variable to store the direction of joy movement
int directionX;
int directionY;


// current state
int state = 1;

// button state varaibles
byte swState = HIGH;
byte lastSwState = HIGH;

// check joy movement on given axis
bool joyMovedOnX = false;
bool joyMovedOnY = false;

// current array position
int currentSegPosition = 0;

// joy threshholds
int minThreshold = 400;
int maxThreshold = 600;

// timers
unsigned long lastActionTime = 0;
unsigned long startTimer = -1;


// class for a segment that contains the pin number, neighbors, 
//       state and variable to track blink
class Segment{
  public:
    int pinNumber;
    int neighborUp;
    int neighborDown;
    int neighborLeft;
    int neighborRight;
    byte state;
    byte blink;

    // basic constructor
    Segment()
    {
      this->pinNumber = 0;
      this->neighborUp = 0;
      this->neighborDown = 0;
      this->neighborLeft = 0;
      this->neighborRight = 0;
      this->state = LOW;
      this->blink = LOW;

    }

    // main constructor
    Segment(const int pinNumber, int neighborUp, int neighborDown, 
            int neighborLeft, int neighborRight)
    {
      this->pinNumber = pinNumber;
      this->neighborUp = neighborUp;
      this->neighborDown = neighborDown;
      this->neighborLeft = neighborLeft;
      this->neighborRight = neighborRight;
      this->state = LOW;
      this->blink = LOW;

    }

    void changeSegmentState()
    {
      this->state = !this->state;
    }

    int blinkSegment()
    {
      this->blink = !this->blink;
    }
};

// object variable for current seg
Segment currentSegment;

// array of segments
Segment segments[segSize] = { 
  Segment(pinA,noSeg,segG,segF,segB), 
  Segment(pinB,segA,segG,segF,noSeg), 
  Segment(pinC,segG,segD,segE,segDP),
  Segment(pinD,segG,noSeg,segE,segC), 
  Segment(pinE,segG,segD,noSeg,segC),
  Segment(pinF,segA,segG,noSeg,segB), 
  Segment(pinG,segA,segD,noSeg,noSeg), 
  Segment(pinDP,noSeg,noSeg,segC,noSeg)
};




void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i].pinNumber, OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  
  Serial.begin(9600);
}

// check for x axis movement
// return 1 for right movement
// return -1 for left movement
// return 0 for no movement
int checkJoyMovementOnX()
{
  int direction = 0;
  int xValue = analogRead(pinX);
  
  // if joy moved left
  if (xValue < minThreshold && joyMovedOnX == false) {
  
    direction = -1;
    joyMovedOnX = true;
  }
  // if joy moved right
	if (xValue > maxThreshold && joyMovedOnX == false) {
    
    direction = 1;
    joyMovedOnX = true;
  }
  
  // if joy didn't move
	if (xValue > minThreshold && xValue < maxThreshold) {
    joyMovedOnX = false;
  }
 
  return direction;
}

// checks for  axis movement
// return 1 for up movement
// return -1 for down movement
// return 0 for no movement
int checkJoyMovementOnY()
{
  int direction = 0;
  int yValue = analogRead(pinY);
  // if joy moved down
  if (yValue < minThreshold && joyMovedOnY == false) {
  
    direction = -1;
    joyMovedOnY = true;
  }
  // if joy moved up  
	if (yValue > maxThreshold && joyMovedOnY == false) {
    
    direction = 1;
    joyMovedOnY = true;
  }
  // if joy didn't move
	if (yValue > minThreshold && yValue < maxThreshold) {
    joyMovedOnY = false;
  }
  return direction;
}

void loop() {
  
	// button state
  swState = digitalRead(pinSW);

  // joy movements
  directionX = checkJoyMovementOnX();
  directionY = checkJoyMovementOnY();
  
  // set currentSegemnt object
  currentSegment = segments[currentSegPosition];


  // write all segments states and if in state 1 instead of writing the state,
  // blink the led (based of blink variable)
  for (int i = 0; i < segSize;i++)
  {
    if (state == 1 && i == currentSegPosition)
      digitalWrite(segments[i].pinNumber,segments[i].blink);
    else
      digitalWrite(segments[i].pinNumber,segments[i].state);
  }
  
  // if button state changed
  if (swState != lastSwState) 
  {
    // if it was pressed and now released
    if (swState == HIGH)
    {
      // if state is 1 check for long press
      // if it's not a long press go to state 2
      // it it is reset board
      // if the state is 2 and button was pressed then go to state 1
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
      // if the button changed from not pressed to pressed start counting for
      // long press
      startTimer = millis();
    }
    
  }
  
  if (state == 1)
  {
    
    // check for x axis joy movement and move to corresponding neighbor
    // if it exists
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
    
    // check for y axis joy movement and move to corresponding neighbor
    // if it exists
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
      
    
    // blink the led every given seconds
    if (millis() - lastActionTime > 1000)
    {
      segments[currentSegPosition].blinkSegment();
      lastActionTime = millis();
    }

    
    
  }
  else
  {
    // if in state 2 write the current state instead of blink state
    digitalWrite(currentSegment.pinNumber,currentSegment.state);
    if (joyMovedOnX == true)
      segments[currentSegPosition].changeSegmentState();
  
  }
  
	lastSwState = swState;
}
       