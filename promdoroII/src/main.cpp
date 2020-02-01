#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#include <RemoteDebug.h>

const int CLK = 3;       //Narancs //Set the CLK pin connection to the display
const int DIO = 13;      //Citrom//Set the DIO pin connection to the display
const int buzzer = 15;   //buzzer to arduino pin 9
const int ledRing = 2;   // szürke
const int greenLed = 22; // piros
const int redLed = 23;   //  barna
const int button1 = 34;
const int button2 = 39;

#define HOST_NAME "myesp32"

const char *ssid = "Chopper_Neptun";
const char *password = "69Apollo19";

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
RemoteDebug Debug;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  MDNS.addService("telnet", "tcp", 23);
  
	Debug.begin(HOST_NAME); // Initialize the WiFi server
  Debug.setResetCmdEnabled(true); // Enable the reset command

	Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	Debug.showColors(true); // Colors
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(HOST_NAME);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });

  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("hostName:");
  Serial.println(ArduinoOTA.getHostname());

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

void loop()
{
  ArduinoOTA.handle();
  Debug.handle();
  debugD("* This is a message of debug level DEBUG");
}
