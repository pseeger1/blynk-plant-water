#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>

////////////////////
// Blynk Settings //
////////////////////
char BlynkAuth[] = "0de846993357443096b7843853812320";
char WiFiNetwork[] = "Stinknet";
char WiFiPassword[] = "WiggleWiggle";

///////////////////////
// Hardware Settings //
///////////////////////
#define WS2812_PIN 4 // Pin connected to WS2812 LED
#define BUTTON_PIN 0
#define LED_PIN    5
#define PUMP_PIN   12
#define ADC_PIN    A0
Adafruit_NeoPixel rgb = Adafruit_NeoPixel(1, WS2812_PIN, NEO_GRB + NEO_KHZ800);

/*BLYNK_WRITE(V0) // Handle RGB from the zeRGBa
{
  if (param.getLength() < 5)
    return;

  byte red = param[0].asInt();
  byte green = param[1].asInt();
  byte blue = param[2].asInt();

  uint32_t rgbColor = rgb.Color(red, green, blue);
  rgb.setPixelColor(0, rgbColor);
  rgb.show();
}*/

void setup()
{
  // Initialize hardware
  Serial.begin(9600); // Serial
  rgb.begin(); // RGB LED
  pinMode(BUTTON_PIN, INPUT); // Button input
  pinMode(LED_PIN, OUTPUT); // LED output
  pinMode(PUMP_PIN, OUTPUT);

  // Initialize Blynk
  Blynk.begin(BlynkAuth, WiFiNetwork, WiFiPassword);
}

void loop()
{
  // Execute Blynk.run() as often as possible during the loop
  int moistureReading = analogRead(ADC_PIN);

  if(moistureReading < 500)
  {
    digitalWrite(PUMP_PIN, HIGH);
  }
  else
  {
    digitalWrite(PUMP_PIN, LOW);
  }

  if(moistureReading > 850)
  {
    moistureReading = 850;
  }

//  Serial.println("Moisture: " + moistureReading);

  float redMultiplier = (1.0- moistureReading/850.0);
  float greenMultiplier = (moistureReading/850.0);

  int redInt = 255*redMultiplier;
  int greenInt = 255*greenMultiplier;

  byte red = (byte) redInt;
  byte green = (byte) greenInt;
  byte blue = 0;

//  Serial.print("Red: ");
//  Serial.println(redMultiplier, 3);
//  Serial.print("Green: ");
//  Serial.println(greenMultiplier, 3);
  
  uint32_t rgbColor = rgb.Color(red, green, blue);
  rgb.setPixelColor(0, rgbColor);
  rgb.show();
  delay(2000);
  Blynk.run(); 
}
