#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define LCD I2C address and initialize the library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if needed

// Pin definitions for stepper motors on Mega 2560
#define X_STEP_PIN 22  
#define X_DIR_PIN 23
#define Y_STEP_PIN 24
#define Y_DIR_PIN 25
#define Z_STEP_PIN 26
#define Z_DIR_PIN 27

// Laser control pin on Mega 2560
#define LASER_PIN 11  // PWM-capable pin on Mega 2560

// Button pins for manual control
#define BUTTON_X_PLUS 28
#define BUTTON_X_MINUS 29
#define BUTTON_Y_PLUS 30
#define BUTTON_Y_MINUS 31
#define BUTTON_Z_PLUS 32
#define BUTTON_Z_MINUS 33

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

void setLaserPower(int powerLevel) {
  // Adjust laser power according to material thickness
  analogWrite(LASER_PIN, powerLevel);
}

void scalePattern(float scaleFactor) {
  // Scale the pattern size by the factor provided
  lcd.setCursor(0, 1);
  lcd.print("Scaling: ");
  lcd.print(scaleFactor);
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

  // Set button pins as inputs with pull-up resistors
  pinMode(BUTTON_X_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_X_MINUS, INPUT_PULLUP);
  pinMode(BUTTON_Y_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_Y_MINUS, INPUT_PULLUP);
  pinMode(BUTTON_Z_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_Z_MINUS, INPUT_PULLUP);

  // Record the initial time as last activity
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
    updateActivity();  // Reset inactivity timer
  }
  if (digitalRead(BUTTON_Y_PLUS) == LOW) {
    moveStepper(Y_STEP_PIN, Y_DIR_PIN, 1);
    updateLCD("Y", 1);
    updateActivity();  // Reset inactivity timer
  }
  if (digitalRead(BUTTON_Y_MINUS) == LOW) {
    moveStepper(Y_STEP_PIN, Y_DIR_PIN, -1);
    updateLCD("Y", -1);
    updateActivity();  // Reset inactivity timer
  }
  if (digitalRead(BUTTON_Z_PLUS) == LOW) {
    moveStepper(Z_STEP_PIN, Z_DIR_PIN, 1);
    updateLCD("Z", 1);
    updateActivity();  // Reset inactivity timer
  }
  if (digitalRead(BUTTON_Z_MINUS) == LOW) {
    moveStepper(Z_STEP_PIN, Z_DIR_PIN, -1);
    updateLCD("Z", -1);
    updateActivity();  // Reset inactivity timer
  }

  // Control laser on/off
  if (digitalRead(LASER_PIN) == LOW) {
    digitalWrite(LASER_PIN, HIGH);  // Turn on laser
    lcd.setCursor(10, 1);
    lcd.print("Laser ON ");
    updateActivity();  // Reset inactivity timer
  } else {
    digitalWrite(LASER_PIN, LOW);  // Turn off laser
    lcd.setCursor(10, 1);
    lcd.print("Laser OFF");
  }

  delay(100);  // Debounce delay
}
