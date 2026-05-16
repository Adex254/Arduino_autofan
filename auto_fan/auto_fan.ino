#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
const int tempPin = A0;      // Thermistor Analog Output connected to A0
const int fanPin = 3;        // Digital PWM pin to drive the fan transistor

// Temperature Threshold to trigger fan (in Celsius)
const float tempThreshold = 30.0; 

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW); // Fan off initially
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("SMART FAN SYSTEM");
  delay(1500);
  lcd.clear();
}

void loop() {
  // 1. Read Analog Value from Thermistor
  int rawAnalog = analogRead(tempPin);
  
  // 2. Convert raw analog reading to Celsius
  // (Standard calculation for NTC 10k thermistor modules)
  float resistance = (1023.0 / (float)rawAnalog) - 1.0;
  resistance = 10000.0 / resistance; // Get actual resistance
  
  // Steinhart-Hart equation constants for temperature conversion
  float temperatureC = resistance / 10000.0;     // (R/Ro)
  temperatureC = log(temperatureC);               // ln(R/Ro)
  temperatureC /= 3950.0;                         // 1/B * ln(R/Ro)
  temperatureC += 1.0 / (25.0 + 273.15);          // + (1/To)
  temperatureC = 1.0 / temperatureC;              // Invert
  temperatureC -= 273.15;                         // Convert absolute Kelvin to Celsius

  // 3. Display Data on LCD
  lcd.setCursor(0, 0);
  lcd.print("TEMP: ");
  lcd.print(temperatureC, 1); // Display with 1 decimal place
  lcd.print((char)223);       // Degree symbol character
  lcd.print("C   ");

  lcd.setCursor(0, 1);
  if (temperatureC >= tempThreshold) {
    digitalWrite(fanPin, HIGH); // Turn fan ON
    lcd.print("FAN STATE: ON   ");
  } else {
    digitalWrite(fanPin, LOW);  // Turn fan OFF
    lcd.print("FAN STATE: OFF  ");
  }

  delay(500); // Update twice a second
}
