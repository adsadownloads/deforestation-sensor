/* Pin definitions */
int PIR = 2;
int VIB = 7;
int LED = 12;

/* Sensor state and counter variables */
int pirState = 0;
int vibState = 0;
int bothCount = 0;
int vibCount = 0;

/* Time variables in ms */
const unsigned long READ_TIMING = 1000;
const unsigned long BOTH_WINDOW = 600000;
const unsigned long VIB_WINDOW = 15000;
const unsigned long OUTPUT_CD = 3000; // 3600000, lowered for debugging
unsigned long previousReadTime = 0;
unsigned long previousBothTime = 0;
unsigned long previousVibTime = 0;

void setup() {
  pinMode(PIR, INPUT);
  pinMode(VIB, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

/* Output sequence */
void output() {
  digitalWrite(LED, HIGH);
  delay(3000);
  digitalWrite(LED, LOW);

  Serial.println("Cooldown started");
  delay(OUTPUT_CD); // Processor does nothing for OUTPUT_CD ms
  Serial.println("Cooldown ended");
}

void loop() {
  unsigned long currentTime = millis();

  /* Check if within window */
  if ( currentTime - previousBothTime >= BOTH_WINDOW ) {
    bothActivityCount = 0;
    previousBothTime = currentTime;
  }
  if ( currentTime - previousVibTime >= VIB_WINDOW ) {
    vibActivityCount = 0;
    previousVibTime = currentTime;
  }
  
  /* Check for timing */
  if ( currentTime - previousReadTime >= READ_TIMING ) {
    pirState = digitalRead(PIR);
    while ( millis() - current time <= 700 ) {
    }
    vibState = digitalRead(VIB);
    previousReadTime = currentTime;

    /* Output condition 1 */
    if ( pirState == 1 && vibState == 1 ) {
      bothCount++;
      Serial.println("Count: Motion + vibration detected");
      previousBothTime = currentTime;

      if ( bothCount == 3 ) {
        Serial.println("Output: Frequent motion + vibration detected");
        output();
      }
    }

    /* Output Condition 2 */
    if ( vibState == 1 ) {
      vibCount++;
      Serial.println("Count: Vibration detected");
      previousVibTime = currentTime;

      if ( vibCount == 7 ) {
        Serial.println("Output: Sustained vibration detected");
        output();
      }
    }
  }
}
