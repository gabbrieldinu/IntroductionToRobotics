//default values which will be used thoroughout the code
#define VOLTAGE_LEVEL 5
#define MIN_ANALOG_VALUE 0
#define MAX_ANALOG_VALUE 1023
#define MIN_LED_COLOR_MAP_VALUE 0
#define MAX_LED_COLOR_MAP_VALUE 255

// define pin values for the 3 potentiometers and the RGB led (a pin for each color)
const int potentiometer1Pin = A5;
const int potentiometer2Pin = A4;
const int potentiometer3Pin = A3;
const int ledRedPin = 11;
const int ledGreenPin = 10;
const int ledBluePin = 9;


void setup()
{
  pinMode(potentiometer1Pin, INPUT);
  pinMode(potentiometer2Pin, INPUT);
  pinMode(potentiometer3Pin, INPUT);

  pinMode(ledGreenPin, OUTPUT); 
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);

  Serial.begin(9600);
}


// auxiliary function to map the value of potentiometer to a value 
// appropiate for a led color (with intermediary steps)
int mapAnalogToLedColorValue (int potentiometerInput)
{
  return map(potentiometerInput, MIN_ANALOG_VALUE, MAX_ANALOG_VALUE, MIN_LED_COLOR_MAP_VALUE, MAX_LED_COLOR_MAP_VALUE);
}

// auxiliary function to set the variable output color of the led based
// on the mapped input of each potentiometer
void setLedColor(int redValue=0, int greenValue=0, int blueValue=0)
{
  analogWrite(ledRedPin, 255 - redValue);
  analogWrite(ledGreenPin, 255 - greenValue);
  analogWrite(ledBluePin, 255 - blueValue);
}


void loop()
{
  int ledRedValue = mapAnalogToLedColorValue(analogRead(potentiometer1Pin));
  int ledGreenValue = mapAnalogToLedColorValue(analogRead(potentiometer2Pin));
  int ledBlueValue = mapAnalogToLedColorValue(analogRead(potentiometer3Pin));
  delay(100); // add a slight delay to make the transitions smoother
  setLedColor(ledRedValue, ledGreenValue, ledBlueValue);
}