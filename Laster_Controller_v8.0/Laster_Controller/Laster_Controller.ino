#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions for stepper motors and laser on Mega 2560
#define X_STEP_PIN 22
#define X_DIR_PIN 23
#define Y_STEP_PIN 24
#define Y_DIR_PIN 25
#define Z_STEP_PIN 26
#define Z_DIR_PIN 27
#define LASER_PIN 11  // Laser control pin

// Button pins on Mega 2560
#define BUTTON_X_PLUS 28
#define BUTTON_X_MINUS 29
#define BUTTON_Y_PLUS 30
#define BUTTON_Y_MINUS 31
#define BUTTON_Z_PLUS 32
#define BUTTON_Z_MINUS 33

// Motor speed control
int motorSpeed = 200;  // Default motor speed

// Define LCD I2C address and initialize the library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if needed (e.g., 0x3F)

// Global variable to track last activity time (in milliseconds)
unsigned long lastActivityTime = 0;
const unsigned long inactivityLimit = 300000;  // 5 minutes of inactivity

// Function to update activity
void updateActivity() {
  lastActivityTime = millis();
}

// Function to check inactivity and turn off the laser
void checkInactivity() {
  if (millis() - lastActivityTime >= inactivityLimit) {
    digitalWrite(LASER_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Laser Off (Inactive)");
  }
}

// Function to set motor speed
void setMotorSpeed(int speed) {
  motorSpeed = speed;
}

// Function to move the motor
void moveMotor(int stepPin, int dirPin, int steps) {
  digitalWrite(dirPin, HIGH);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(motorSpeed);
  }
}

// Function to move the stepper motor
void moveStepper(int stepPin, int dirPin, int direction) {
  digitalWrite(dirPin, direction > 0 ? HIGH : LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(800);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(800);
}

// Function to update the LCD with current axis positions
void updateLCD(String axis, int increment) {
  static int x = 0, y = 0, z = 0;
  if (axis == "X") x += increment;
  if (axis == "Y") y += increment;
  if (axis == "Z") z += increment;

  lcd.setCursor(0, 1);
  lcd.print("X:" + String(x) + " Y:" + String(y) + " Z:" + String(z) + " ");
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CNC Laser Cutter");

  // Set motor and laser pins as outputs
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);  // Initial laser state is off

  // Set button pins as inputs with pull-up resistors
  pinMode(BUTTON_X_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_X_MINUS, INPUT_PULLUP);
  pinMode(BUTTON_Y_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_Y_MINUS, INPUT_PULLUP);
  pinMode(BUTTON_Z_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_Z_MINUS, INPUT_PULLUP);

  // Initialize last activity time
  updateActivity();
}

void loop() {
  // Check for inactivity and auto-shutdown laser
  checkInactivity();

  // Check button states and move motors accordingly
  if (digitalRead(BUTTON_X_PLUS) == LOW) {
    moveStepper(X_STEP_PIN, X_DIR_PIN, 1);
    updateLCD("X", 1);
    updateActivity();  // Reset inactivity timer
  }
  if (digitalRead(BUTTON_X_MINUS) == LOW) {
    moveStepper(X_STEP_PIN, X_DIR_PIN, -1);
    updateLCD("X", -1);
    updateActivity();
  }
  if (digitalRead(BUTTON_Y_PLUS) == LOW) {
    moveStepper(Y_STEP_PIN, Y_DIR_PIN, 1);
    updateLCD("Y", 1);
    updateActivity();
  }
  if (digitalRead(BUTTON_Y_MINUS) == LOW) {
    moveStepper(Y_STEP_PIN, Y_DIR_PIN, -1);
    updateLCD("Y", -1);
    updateActivity();
  }
  if (digitalRead(BUTTON_Z_PLUS) == LOW) {
    moveStepper(Z_STEP_PIN, Z_DIR_PIN, 1);
    updateLCD("Z", 1);
    updateActivity();
  }
  if (digitalRead(BUTTON_Z_MINUS) == LOW) {
    moveStepper(Z_STEP_PIN, Z_DIR_PIN, -1);
    updateLCD("Z", -1);
    updateActivity();
  }

  // Control laser on/off
  if (digitalRead(LASER_PIN) == LOW) {
    digitalWrite(LASER_PIN, HIGH);  // Turn on laser
    lcd.setCursor(10, 1);
    lcd.print("Laser ON ");
  } else {
    digitalWrite(LASER_PIN, LOW);  // Turn off laser
    lcd.setCursor(10, 1);
    lcd.print("Laser OFF");
  }

  delay(100);  // Debounce delay
}
