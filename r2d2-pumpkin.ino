#include <SD.h>
#define SD_ChipSelectPin 4
#include <TMRpcm.h>
#include <SPI.h>
#include <ServoTimer2.h>

// Pins
const int SPEAKER_PIN = 9;
const int PIR_PIN = 2;
const int SERVO_PIN = 10;

// Total number of sound effects
const int TOTAL_SOUNDS = 3;

// Servo positions
const int LEFT = 2200; // 2000 = 135 degrees
const int MIDDLE = 1400; // 1000 = 90 degrees
const int RIGHT = 800; // 100 = 45 degrees
const int RESET_RIGHT = 1100;
const int RESET_LEFT = 1700;

// Servo movement
const int STEP = 5;
const int DELAY = 10;

// Initialize audio
TMRpcm audio;
int sound = 0;
char filename[11];

// Initialize servo
ServoTimer2 servo; 
int position = MIDDLE;

// PIR state
int pirState = LOW;

// Initial state
boolean reset = true;

// Play a sound effect
void play_sound() {
  sprintf(filename, "r2d2-%d.wav", sound);
  if (Serial.available()) {
    Serial.println(filename);
  }
  audio.play(filename);
  if (++sound == TOTAL_SOUNDS) {
    sound = 0;
  }
}

// Move the servo to a position
void move_to(int value) {
  if (!servo.attached()) {
    return;
  }
  int current = servo.read();
  if (Serial.available()) {
    Serial.println("*** move_to ***");
    Serial.println(current);
    Serial.println(value);
  }
  if (current == value) {
    return;
  }
  if (current < value) {
    current += STEP;
    while (current <= value) {
      servo.write(current);
      delay(DELAY);
      current += STEP;
    }
  } else {
    current -= STEP;
    while (current >= value) {
      servo.write(current);
      delay(DELAY);
      current -= STEP;
    }
  }    
  if (current != value) {
    servo.write(value);
    delay(DELAY);
  } 
}

// Move the servo to a random position
void move_servo(int right, int left) {
  int val;
  if (position != MIDDLE && random(0, 3) == 0) {
    // If not in middle, 33% chance to reset
    val = MIDDLE;
  } else if (position >= MIDDLE) {
    val = random(right, position);
  } else {
    val = random(position, left + 1);
  }
  position = val;
  move_to(val);
}

void setup() {
  // Turn off built-in LED
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize audio
  audio.speakerPin = SPEAKER_PIN;
  audio.setVolume(7);

  // Initialize servo
  servo.attach(SERVO_PIN);
  
//  Serial.begin(9600); // FOR DEBUGGING ONLY
  
  // Check SD card
  if (!SD.begin(SD_ChipSelectPin)) {
    if (Serial.available()) {    
      Serial.println("Failed to initialize SD card");
    }
    return;
  }
  if (Serial.available()) {
    Serial.println("Ready");
  }
  // Play a random sound
  randomSeed(analogRead(0));
  sound = random(0, TOTAL_SOUNDS);
  play_sound();
}

void loop() {
  if (reset) {
    reset = false;
    move_to(position);
  }
  int pirValue = digitalRead(PIR_PIN);
  if (pirValue == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED
    if (pirState == LOW) {
      // Motion detected
      pirState = HIGH;
      digitalWrite(LED_BUILTIN, HIGH);
      if (Serial.available()) {
        Serial.println("Motion start");
      }
      move_servo(RIGHT, LEFT);
      play_sound();
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW); // Turn off LED
    if (pirState == HIGH) {
      pirState = LOW;
      digitalWrite(LED_BUILTIN, LOW);
      if (Serial.available()) {
        Serial.println("Motion end");
      }
      move_servo(RESET_RIGHT, RESET_LEFT);
    }
  }
}

