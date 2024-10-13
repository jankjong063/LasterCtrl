#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define LCD I2C address and initialize the library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if needed (e.g., 0x3F)

// Pin definitions for stepper motors on Mega 2560
#define X_STEP_PIN 22  // Use any available digital pins
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

// Function to update last activity time
void updateActivity() {
  lastActivityTime = millis();
}

// Function to check inactivity and turn off laser if inactive for 5 minutes
void checkInactivity() {
  if (millis() - lastActivityTime >= inactivityLimit) {
    // Turn off laser if inactive
    digitalWrite(LASER_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Laser Off (Inactive)");
  }
}

// Function to set laser power based on material thickness
void setLaserPower(int powerLevel) {
  analogWrite(LASER_PIN, powerLevel);  // Adjust laser power with PWM
}

// Function to let user select a cutting mode
void chooseCuttingMode() {
  lcd.clear();
  lcd.print("Select Mode:");
  // Implement logic for user selection of cutting mode
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

  // Set laser control pin as output
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);  // Turn off laser initially

  // Record the initial time as last activity
  updateActivity();
}

void loop() {
  // Check for inactivity and auto-shutdown laser
  checkInactivity();

  // Example of moving motors (add your motor control logic)
  // Example of checking buttons and moving motors
  if (digitalRead(X_STEP_PIN) == LOW) {
    updateActivity();  // Reset the inactivity timer on action
    // Motor control logic here...
  }

  // Add more motor or laser control logic as needed
}
