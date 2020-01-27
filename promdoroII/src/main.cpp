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
  strip.show(); // Initialize all pixels to 'off'

  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, HIGH);
  display.setBrightness(0x0a); //set the diplay to maximum brightness
}
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void loop()
{
  Serial.println("Hello world!");
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50);                // Red
  colorWipe(strip.Color(0, 255, 0), 50);                // Green
  colorWipe(strip.Color(0, 0, 255), 50);                // Blue
  for (numCounter = 0; numCounter < 9999; numCounter++) //Iterate numCounter
  {
    if (digitalRead(button1) == HIGH)
    {
      digitalWrite(greenLed, HIGH);
    }
    else
    {
      digitalWrite(greenLed, LOW);
    }
    if (digitalRead(button2) == HIGH)
    {
      digitalWrite(redLed, HIGH);
    }
    else
    {
      digitalWrite(redLed, LOW);
    }
    display.showNumberDecEx(numCounter,0b01000000); //Display the numCounter value;

    // ledcWrite(channel, 10);
    // delay(50);
    // ledcWriteTone(channel, 0);
    //delay(500);
  }
}
// Fill the dots one after the other with a color
