// Pin Definitions
const int ledPin = 13;
const int vibPin = 7;
const int pirPin = 2;

// Variable Declarations
int vibVal = 0;
int pirState = LOW;
int pirVal = 0;
unsigned long lastOutputTime = 0;
unsigned long lastActivityTime = 0;
int activityCount = 0;

// Time Constants
const unsigned long activityWindow = 10 * 60 * 1000;
const unsigned long outputCooldown = 60 * 60 * 1000;
bool outputOnCooldown = false;

void setup() {
  // Initialize pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(vibPin, INPUT);
  
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Get current time
  unsigned long currentTime = millis();
  
  // Read sensor values
  pirVal = digitalRead(pirPin);
  vibVal = digitalRead(vibPin);

  // Check if motion and vibration detected
  if (pirVal == HIGH && vibVal == HIGH) {   
    if (!outputOnCooldown && currentTime - lastOutputTime >= 1000) {
      Serial.println("Vibration detected!");
      if (pirState == LOW) {
        Serial.println("Motion detected!");
        pirState = HIGH;
        
        // Update activity count
        unsigned long timeDiff = currentTime - lastActivityTime;
        if (timeDiff > activityWindow) {
          activityCount = 1;
        } else {
          activityCount++;
        }
        lastActivityTime = currentTime;
        
        // Activate LED output if activity count threshold reached
        if (activityCount >= 3) {
          digitalWrite(ledPin, HIGH);
          delay(5000);
          digitalWrite(ledPin, LOW);
          activityCount = 0;
          lastOutputTime = currentTime;
          outputOnCooldown = true;
          Serial.println("LED output activated.");
        }
        Serial.print("Activity count within last 10 minutes: ");
        Serial.println(activityCount);
      }
    }
  } else {
    // Check if motion ended
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      pirState = LOW;
      lastOutputTime = currentTime;
    }
  }

  // Check if cooldown period ended
  if (outputOnCooldown && currentTime - lastOutputTime >= outputCooldown) {
    outputOnCooldown = false;
    Serial.println("LED output cooldown ended.");
  }
}
