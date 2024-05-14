/* Pin definitions */
int PIR = 2;
int VIB = 7;
int LED = 12;

/* Sensor state and counter variables */
int pirState = 0;
int vibState = 0;
int bothActivityCount = 0;
int vibActivityCount = 0;

/* Time variables in ms */
const unsigned long READ_TIMING = 1000;
const unsigned long BOTH_EVENT_WINDOW = 600000;
const unsigned long VIB_EVENT_WINDOW = 15000;
const unsigned long OUTPUT_CD = 3000; // 3600000, lowered for debugging
unsigned long previousReadTime = 0;
unsigned long previousBothEventTime = 0;
unsigned long previousVibEventTime = 0;

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
  if ( currentTime - previousBothEventTime >= BOTH_EVENT_WINDOW ) {
    bothActivityCount = 0;
    previousBothEventTime = currentTime;
  }
  if ( currentTime - previousVibEventTime >= VIB_EVENT_WINDOW ) {
    vibActivityCount = 0;
    previousVibEventTime = currentTime;
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
      bothActivityCount++;
      Serial.println("Count: Motion + vibration detected");
      previousBothEventTime = currentTime;

      if ( bothActivityCount == 3 ) {
        Serial.println("Output: Frequent motion + vibration detected");
        output();
      }
    }

    /* Output condition 2 */
    if ( pirState != 1 && vibState == 1 ) {
      vibActivityCount++;
      Serial.println("Count: Vibration detected");
      previousVibEventTime = currentTime;

      if ( vibActivityCount == 7 ) {
        Serial.println("Output: Sustained vibration detected");
        output();
      }
    }
  }
}
