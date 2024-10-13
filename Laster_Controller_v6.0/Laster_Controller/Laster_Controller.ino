#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Constants for a 10k thermistor
#define THERMISTOR_PIN A0
#define SERIES_RESISTOR 10000  // 10k ohm resistor in series with thermistor
#define NOMINAL_RESISTANCE 10000  // Resistance at 25°C
#define NOMINAL_TEMPERATURE 25  // Temperature for nominal resistance (25°C)
#define B_COEFFICIENT 3950  // Beta coefficient of the thermistor
#define ADC_MAX_VALUE 1023  // Maximum value for 10-bit ADC (1023)

// Define LCD I2C address and initialize the library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if needed (e.g., 0x3F)

// Pin definitions for stepper motors and laser on Mega 2560
#define X_STEP_PIN 22
#define X_DIR_PIN 23
#define Y_STEP_PIN 24
#define Y_DIR_PIN 25
#define Z_STEP_PIN 26
#define Z_DIR_PIN 27
#define LASER_PIN 11  // Laser control pin

// Read thermistor temperature
float readThermistor() {
  int adcValue = analogRead(THERMISTOR_PIN);  // Read analog value from thermistor
  float resistance = SERIES_RESISTOR / (1023.0 / adcValue - 1.0);  // Calculate thermistor resistance
  float temperatureKelvin = 1.0 / (log(resistance / NOMINAL_RESISTANCE) / B_COEFFICIENT + 1.0 / (NOMINAL_TEMPERATURE + 273.15));
  return temperatureKelvin - 273.15;  // Convert to Celsius
}

void checkLaserTemperature() {
  float temperature = readThermistor();
  if (temperature > 70) {  // Turn off laser if temp exceeds 70°C
    digitalWrite(LASER_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Laser Overheat!");
  }
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CNC Laser Cutter");

  // Set motor pins as outputs
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);

  // Initial state of laser (off)
  digitalWrite(LASER_PIN, LOW);
}

void loop() {
  // Call temperature check function periodically
  checkLaserTemperature();

  // Other CNC control code
}
