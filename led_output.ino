// Pin Definitions
const int ledPin = 12;
const int vibPin = 7;
const int pirPin = 2;

// Variable Declarations
int vibVal = 0;
int pirState = 0;
int pirVal = 0;
unsigned long lastOutputTime = 0;
unsigned long lastVibActivityTime = 0;
unsigned long lastDualActivityTime = 0;
int vibActivityCount = 0;
int dualActivityCount = 0;

// Time Constants
const unsigned long dualActivityWindow = 600000;
const unsigned long vibActivityWindow = 180000;
const unsigned long outputCooldown = 10000;
bool outputOnCooldown = false;

void setup() {
  // Initialize pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(vibPin, INPUT);
  
  // Initialize serial communication
  Serial.begin(9600);
}

void output() {
  // Output sequence
  unsigned long currentTime = millis();
  Serial.println("Output activated. Cooldown started.");
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);
  dualActivityCount = 0;
  vibActivityCount = 0;
  lastOutputTime = currentTime;
  outputOnCooldown = true;
}

void loop() {
  // Read sensor values
  pirVal = digitalRead(pirPin);
  delay(700); // Delay to account for swing motion during logging
  vibVal = digitalRead(vibPin);
  
  // Get current time
  unsigned long currentTime = millis();

  // Check idleness
  if (currentTime - lastDualActivityTime > dualActivityWindow) {
    dualActivityCount = 0;
  }
  if (currentTime - lastVibActivityTime > vibActivityWindow) {
    vibActivityCount = 0;
  }

  // Check if vibration detected
  if (vibVal == 1) {
    if (!outputOnCooldown && currentTime - lastVibActivityTime < activityWindow) {
      Serial.println("Vibration detected!");

      // Update vibration activity count
      vibActivityCount++;
      Serial.print("Vibration activity count within last 10 minutes: ");
      Serial.println(vibActivityCount);
      lastVibActivityTime = currentTime;

      // Activate LED output if sustained vibration activity detected
      if (vibActivityCount >= 7) {
        output();
      }
    }

    // Check if motion (and vibration) activity detected
    if (pirVal == 1) {
      if (!outputOnCooldown && currentTime - lastDualActivityTime < activityWindow) {
        Serial.println("Dual activity detected!");

        // Update activity count
        dualActivityCount++;
        Serial.print("Dual activity count within last 10 minutes: ");
        Serial.println(dualActivityCount);
        lastDualActivityTime = currentTime;

        // Activate LED output if dual activity count threshold reached
        if (dualActivityCount >= 3) {
          output();
        }
      }
    }
  }
  delay(300); // Delay to limit false positives due to narrow detection timeframe

  // Check if cooldown period ended
  if (outputOnCooldown && currentTime - lastOutputTime >= outputCooldown) {
    outputOnCooldown = false;
    Serial.println("Output cooldown ended.");
  }
}
