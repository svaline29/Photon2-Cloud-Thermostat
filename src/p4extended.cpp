#include "Particle.h"
#include "neopixel.h"
#define COOL 0
#define HEAT 2
#define OFF 1
#define REDALERT 3

int PIXEL_COUNT = 1;
#define PIXEL_PIN SPI
int PIXEL_TYPE = WS2812;

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


int mode = OFF;
int cF_setMode(String inputString);
int cF_setTargetTempFromString(String inputString);

int tempPIN = A0;
double tempC = 0.0;
double targetTemp = 25.0;

void setup() {
  strip.begin();
  Serial.begin(9600);
  pinMode(tempPIN,INPUT); // Set the pin mode for the temperature sensor pin
  Particle.variable("tempC", &tempC, DOUBLE); // Register tempC as a cloud variable of type double
  Particle.variable("targetTemp", &targetTemp, DOUBLE); // Register targetTemp
  Particle.variable("mode", &mode, INT); // Register mode as a cloud variable of type int
  Particle.function("cF_setMode", cF_setMode); // Register the function to set mode from string
  Particle.function("cF_setTargetTemp", cF_setTargetTempFromString);
  Serial.println("Setup complete.");
}

void loop() {
  int ADCreading = analogRead(tempPIN);
  tempC = (ADCreading - 620) * 0.0806;
  delay(500);
  Serial.print("Temp C: ");
  Serial.println(tempC);  
  Serial.print("Current mode: ");
  Serial.println(mode);  
  Serial.println();

  if (mode == COOL) {
    strip.setPixelColor(0, strip.Color(0, 0, 255));
  } else if (mode == HEAT) {
    strip.setPixelColor(0, strip.Color(165, 225, 0));
  } else if (mode == REDALERT) {
    strip.setPixelColor(0, strip.Color(0, 255, 0));
  } else {
    strip.setPixelColor(0, strip.Color(0, 0, 0));
  }
  strip.show();

}

int cF_setTargetTempFromString(String inputString) {
  double val = inputString.toFloat();
  if (val>90) {
    targetTemp = 90.0;
  } else if (val<50) {
    targetTemp = 50.0;
  } else {
    targetTemp = val;
  }
  return 0;
  }



int cF_setMode(String inputString) {
  if (inputString == "Cool") {
    mode = COOL;
    return 0;
  } else if (inputString == "Heat") {
    mode = HEAT;
    return 2;
  } else if (inputString == "Off") {
    mode = OFF;
    return 1;
  } else if (inputString == "REDALERT") {
    mode = REDALERT;
    return 3;
  } else {
    return -1; // Invalid input
  }
}
