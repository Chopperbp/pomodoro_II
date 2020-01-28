#include <Arduino.h>

#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>

const int CLK = 3;       //Narancs //Set the CLK pin connection to the display
const int DIO = 13;      //Citrom//Set the DIO pin connection to the display
const int buzzer = 15;   //buzzer to arduino pin 9
const int ledRing = 2;   // szürke
const int greenLed = 22; // piros
const int redLed = 23;   //  barna
const int button1 = 34;
const int button2 = 39;

// #define PIN 6

int numCounter = 0;
int freq = 2000;
int channel = 0;
int resolution = 8;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, ledRing, NEO_GRB + NEO_KHZ800);
TM1637Display display(CLK, DIO); //set up the 4-Digit Display.

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzer, channel);

  strip.begin();
  strip.clear();
  display.clear();
  display.setBrightness(5);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  Serial.println("Hello world!");
}
void colorWipe(uint32_t c, uint8_t wait)
{
  strip.clear();
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void colorWipeDown(uint32_t c, uint8_t wait)
{
  strip.clear();
  for (int16_t i = strip.numPixels(); i >= 0; i--)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void loop()
{

  // Some example procedures showing how to display to the pixels:
  if (digitalRead(button1) == HIGH)
  {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    colorWipe(strip.Color(0, 15, 0), 20); // Green
    numCounter++;
    ledcWrite(channel, 10);
    ledcWriteNote(channel, NOTE_A, 5);
    delay(50);
    ledcWrite(channel, 0);
  }
  if (digitalRead(button2) == HIGH)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    colorWipeDown(strip.Color(22, 6, 0), 20); // Red
    numCounter--;
    ledcWrite(channel, 10);
    ledcWriteNote(channel, NOTE_A, 6);
    delay(50);
    ledcWrite(channel, 0);
  }
  display.showNumberDec(numCounter);
}
// Fill the dots one after the other with a color
