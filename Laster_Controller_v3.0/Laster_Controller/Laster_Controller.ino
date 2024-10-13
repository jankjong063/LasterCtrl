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

// Button pins on Mega 2560
#define BUTTON_X_PLUS 28
#define BUTTON_X_MINUS 29
#define BUTTON_Y_PLUS 30
#define BUTTON_Y_MINUS 31
#define BUTTON_Z_PLUS 32
#define BUTTON_Z_MINUS 33

// Define the pins for the homing sensors
#define HOME_X_SENSOR 34  // Pin connected to the X-axis homing sensor
#define HOME_Y_SENSOR 35  // Pin connected to the Y-axis homing sensor
#define HOME_Z_SENSOR 36  // Pin connected to the Z-axis homing sensor

// Homing functionality for CNC axes
void homeAxes() {
  // Code to move each axis to its home position using homing sensors
  // For example:
  // Move X axis until it hits the home sensor, then stop
  digitalWrite(X_DIR_PIN, LOW);
  while (digitalRead(HOME_X_SENSOR) == HIGH) {
    digitalWrite(X_STEP_PIN, HIGH);
    delay(1);
    digitalWrite(X_STEP_PIN, LOW);
    delay(1);
  }
  // Repeat for Y and Z axes...
}

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
  // Homing axes at startup
  homeAxes();  
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

  // Set initial positions on LCD
  lcd.setCursor(0, 1);
  lcd.print("X:0 Y:0 Z:0");

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
  }
  if (digitalRead(BUTTON_X_MINUS) == LOW) {
    moveStepper(X_STEP_PIN, X_DIR_PIN, -1);
    updateLCD("X", -1);
  }
  if (digitalRead(BUTTON_Y_PLUS) == LOW) {
    moveStepper(Y_STEP_PIN, Y_DIR_PIN, 1);
    updateLCD("Y", 1);
  }
  if (digitalRead(BUTTON_Y_MINUS) == LOW) {
    moveStepper(Y_STEP_PIN, Y_DIR_PIN, -1);
    updateLCD("Y", -1);
  }
  if (digitalRead(BUTTON_Z_PLUS) == LOW) {
    moveStepper(Z_STEP_PIN, Z_DIR_PIN, 1);
    updateLCD("Z", 1);
  }
  if (digitalRead(BUTTON_Z_MINUS) == LOW) {
    moveStepper(Z_STEP_PIN, Z_DIR_PIN, -1);
    updateLCD("Z", -1);
  }

  // Control laser on/off (example with a single button or condition)
  if (digitalRead(LASER_PIN) == LOW) {
    digitalWrite(LASER_PIN, HIGH);  // Turn on laser
    lcd.setCursor(10, 1);
    lcd.print("Laser ON ");
  } else {
    digitalWrite(LASER_PIN, LOW);  // Turn off laser
    lcd.setCursor(10, 1);
    lcd.print("Laser OFF");
  }

  updateActivity();  // Reset the inactivity timer on user action

  delay(100);  // Debounce delay
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
