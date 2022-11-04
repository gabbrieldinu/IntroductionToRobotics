// Pin declaration
const int carsRedPin = 10;
const int carsYellowPin = 11;
const int carsGreenPin = 12;
const int pedestrianRedPin = 8;
const int pedestrianGreenPin = 9;
const int buttonPin = 2;
const int buzzerPin = 6;
const int serialPort = 9600;

byte buttonState = LOW;

// State Duration declaration
const int state1Duration = 8000;
const int state2Duration = 1000;
const int state3Duration = 8000;
const int state4Duration = 4000;

// Number of times the buzzer will, well, buzz during a state
const int state3BuzzNumber = 10;
const int state4BuzzNumber = 20;

// Sound frequency of the buzz
const int state3SoundFrequency = 1000;
const int state4SoundFrequency = 1600;

// Calculating the duration of a buzz based on duration and number of buzzes
const int state3SoundInterval = state3Duration / state3BuzzNumber;
const int state4SoundInterval = state4Duration / state4BuzzNumber;

byte buttonChanged = 0;
// variable that checks buttons state
byte currentButtonReading = 0;
// variable that helps with button realeases verification
byte lastButtonReading = 0;
// variable that helps turn on/off the buzzer
byte buzzerOn = 1;
// variable that indicates if state1 needs to be changed (button was pressed and realeased)
byte changeState1 = 0;

//variables to track debounce
const unsigned long debounceDelay = 70;
unsigned long lastDebounceTime = 100000;

// universal variable that tracks when an action starts
unsigned long actionStartTime = 10000;
// variable that tracks when tracks when a state starts
unsigned long stateStartTime = 0;

// variable that tracks the state the program is at a given moment
int currentState = 1;


void setup() {
  // put your setup code here, to run once:
  pinMode(carsRedPin, OUTPUT);
  pinMode(carsYellowPin, OUTPUT);
  pinMode(carsGreenPin, OUTPUT);
  pinMode(pedestrianRedPin, OUTPUT);
  pinMode(pedestrianGreenPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(serialPort);
}

// function to blink a led
// parameter: pin of the led 
void blinkLed(int ledPin)
{
  int ledState = digitalRead(ledPin);
  digitalWrite(ledPin, !ledState);
}

// function to turn on the buzzer
// parameters: # pin of the buzzer
//             # sound frequency
void buzz(int buzzerPin, int frequency)
{
  if (buzzerOn == 0)
    noTone(buzzerPin );
  else
    tone(buzzerPin,frequency);

  buzzerOn = !buzzerOn;
}

// function the checks if a state is finished
// parameters: # state start time
//             # defined state duration
byte checkFinishedState(unsigned long enterTime, int stateDuration)
{
  int finished = 0;
  if (millis() - enterTime >= stateDuration)
    finished = 1;
  return finished;
}

// stateX: function to perform the actions in state X
void state1()
{
  
  digitalWrite(pedestrianRedPin, HIGH);
  digitalWrite(pedestrianGreenPin, LOW);

  digitalWrite(carsRedPin, LOW);
  digitalWrite(carsYellowPin, LOW);
  digitalWrite(carsGreenPin, HIGH);
}

void state2()
{
    
  digitalWrite(pedestrianRedPin, HIGH);
  digitalWrite(pedestrianGreenPin, LOW);

  digitalWrite(carsGreenPin, LOW);
  digitalWrite(carsRedPin, LOW);
  digitalWrite(carsYellowPin, HIGH);
}


void state3()
{
  digitalWrite(pedestrianRedPin, LOW);
  digitalWrite(pedestrianGreenPin, HIGH);

  digitalWrite(carsRedPin, HIGH);
  digitalWrite(carsYellowPin, LOW);
  digitalWrite(carsGreenPin, LOW);
    
    // repurpose the checkFInishedState function to check if buzzer hit the sound duration threshold
  if(checkFinishedState(actionStartTime, state3SoundInterval))
  {
    buzz(buzzerPin, state3SoundFrequency);
    // reset actionStartTime after buzz is on or off 
    actionStartTime = millis();
  }
}

void state4()
{
  digitalWrite(pedestrianRedPin, LOW);

  digitalWrite(carsRedPin, HIGH);
  digitalWrite(carsYellowPin, LOW);
  digitalWrite(carsGreenPin, LOW);

  // same logic as state3
  // repurpose the checkFInishedState function to check if buzzer hit the sound duration threshold
  if(millis() - actionStartTime >= state4SoundInterval)
  {
    buzz(buzzerPin, state4SoundFrequency);
    blinkLed(pedestrianGreenPin); 
    actionStartTime = millis();
  }
}

// function to revert the effects of a state
void exitState()
{
  
  digitalWrite(pedestrianRedPin, LOW);
  digitalWrite(pedestrianGreenPin, LOW);

  digitalWrite(carsRedPin, LOW);
  digitalWrite(carsYellowPin, LOW);
  digitalWrite(carsGreenPin, LOW);

  noTone(buzzerPin);
  actionStartTime = 0;
}

void loop() {
  
  
  //Serial.println(String(currentButtonReading)+"  "+String(lastButtonReading))

  //put your main code here, to run repeatedly:
  if (currentState == 1)
  {
    state1();
    // check for button press
    currentButtonReading = digitalRead(buttonPin);
    Serial.println(String(currentButtonReading)+"  "+String(lastButtonReading));

    if (currentButtonReading != lastButtonReading) 
    {
    // reset the debouncing timer
        lastDebounceTime = millis();
    }
    
    
      if ((millis() - lastDebounceTime) > debounceDelay)
      {
          if (currentButtonReading != buttonState) 
          {
            buttonState = currentButtonReading;

            if (buttonState == LOW)
            {
            changeState1 = 1;
            actionStartTime = millis();
            }
          }
      }
      
      // before entering state 2, wait for the duration
      if (changeState1 == 1 && checkFinishedState(actionStartTime, state1Duration))
      {
        changeState1 = 0;
        exitState();
        currentState = 2;
        stateStartTime = millis();
      }
    
      lastButtonReading = currentButtonReading;
  }
  else
  {
    if (currentState == 2)
    {
      state2();

      if (checkFinishedState(stateStartTime, state2Duration))
      {
        exitState();
        currentState = 3;
        stateStartTime = actionStartTime = millis();
      }
    }
    else
    {
      if (currentState == 3)
      {
        state3();

        if (checkFinishedState(stateStartTime, state3Duration))
        {
          exitState();
          currentState = 4;
          stateStartTime = actionStartTime = millis();
        }
      }
      else
      {
        if (currentState == 4)
        {
          state4();
          if (checkFinishedState(stateStartTime, state4Duration))
          {
            exitState();
            currentState = 1;
          }
        }
      }
    } 
  }
}
