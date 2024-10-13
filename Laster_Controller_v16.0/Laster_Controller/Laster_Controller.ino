#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define LCD I2C address and initialize the library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if needed (e.g., 0x3F)

// Pin definitions for stepper motors on Mega 2560
#define X_STEP_PIN 22
#define X_DIR_PIN 23
#define Y_STEP_PIN 24
#define Y_DIR_PIN 25
#define Z_STEP_PIN 26
#define Z_DIR_PIN 27

// Laser control pin on Mega 2560
#define LASER_PIN 11  // PWM-capable pin on Mega 2560

// Global variable to track last activity time (in milliseconds)
unsigned long lastActivityTime = 0;
const unsigned long inactivityLimit = 300000;  // 5 minutes of inactivity

void updateActivity() {
  lastActivityTime = millis();
}

void checkInactivity() {
  if (millis() - lastActivityTime >= inactivityLimit) {
    // Turn off laser if inactive for 5 minutes
    digitalWrite(LASER_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Laser Off (Inactive)");
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

  // Initial state of the laser (off)
  digitalWrite(LASER_PIN, LOW);

  // Record the initial time as last activity
  updateActivity();
}

void loop() {
  // Check for inactivity and auto-shutdown laser
  checkInactivity();

  // Other code handling user input or movement...
  // Example:
  // if (someCondition) {
  //    updateActivity();
  // }
}

void moveStepper(int stepPin, int dirPin, int direction) {
  digitalWrite(dirPin, direction > 0 ? HIGH : LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(800);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(800);
}

void updateLCD(String axis, int increment) {
  static int x = 0, y = 0, z = 0;
  if (axis == "X") x += increment;
  if (axis == "Y") y += increment;
  if (axis == "Z") z += increment;

  lcd.setCursor(0, 1);
  lcd.print("X:" + String(x) + " Y:" + String(y) + " Z:" + String(z) + " ");
}
