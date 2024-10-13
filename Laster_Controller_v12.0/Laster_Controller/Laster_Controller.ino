#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <VoiceRecognitionV3.h>
#include <SoftwareSerial.h>

// Define LCD I2C address and initialize the library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if needed (e.g., 0x3F)

// Define pins for SoftwareSerial communication with the voice module
#define VR_RX_PIN 10  // RX pin for voice recognition module
#define VR_TX_PIN 11  // TX pin for voice recognition module

VR myVoice(VR_RX_PIN, VR_TX_PIN);  // Initialize the voice recognition module

// Pin definitions for stepper motors on Mega 2560
#define X_STEP_PIN 22  // Use any available digital pins
#define X_DIR_PIN 23
#define Y_STEP_PIN 24
#define Y_DIR_PIN 25
#define Z_STEP_PIN 26
#define Z_DIR_PIN 27

// Laser control pin on Mega 2560
#define LASER_PIN 11  // PWM-capable pin on Mega 2560

// Commands
uint8_t startLaser[] = {0x01};  // Command 1: start laser
uint8_t stopLaser[] = {0x02};   // Command 2: stop laser

// Setup voice recognition
void setupVoiceRecognition() {
  myVoice.begin(9600);  // Start voice module at 9600 baud rate
  lcd.print("VR Module Ready");
  myVoice.clear();  // Clear previous commands
  myVoice.load(startLaser, 1);  // Load command 1: start laser
  myVoice.load(stopLaser, 2);   // Load command 2: stop laser
}

// Start laser cutting
void startLaserCut() {
  digitalWrite(LASER_PIN, HIGH);  // Turn on laser
  lcd.setCursor(0, 1);
  lcd.print("Laser ON ");
}

// Stop laser cutting
void stopLaserCut() {
  digitalWrite(LASER_PIN, LOW);  // Turn off laser
  lcd.setCursor(0, 1);
  lcd.print("Laser OFF");
}

void checkVoiceCommands() {
  uint8_t records[7];
  if (myVoice.recognize(records, 50)) {
    switch (records[1]) {
      case 1:
        startLaserCut();
        break;
      case 2:
        stopLaserCut();
        break;
    }
  }
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CNC Laser Cutter");

  // Initialize motors, laser, and voice recognition
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Z_STEP_PIN, OUTPUT);
  pinMode(Z_DIR_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);  // Turn off laser initially

  // Setup voice recognition
  setupVoiceRecognition();
}

void loop() {
  // Check for voice commands
  checkVoiceCommands();
  
  // Other code handling user input or movement...
}
