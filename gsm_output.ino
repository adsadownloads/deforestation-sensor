#include <GSM.h> // Include the library for your GSM module

int ledPin = 13;                    // choose the pin for the LED
int vibPin = 7;
int pirPin = 2;                     // choose the input pin (for PIR sensor)
int vibVal = 0;
int pirState = LOW;                 // we start, assuming no motion detected
int pirVal = 0;                     // variable for reading the pin status
unsigned long lastOutputTime = 0;   // to track the time of the last output
unsigned long outputDelay = 1000;   // delay between outputs in milliseconds

unsigned long lastActivityTime = 0; // to track the time of the last activity
int activityCount = 0;              // count of activity within the last 10 minutes
unsigned long activityWindow = 10 * 60 * 1000; // 10 minutes in milliseconds

unsigned long lastMessageTime = 0;  // to track the time of the last message
unsigned long messageDelay = 1 * 60 * 60 * 1000; // 1 hour in milliseconds

GSM gsmAccess;                      // Initialize GSM module

void setup() {
  pinMode(ledPin, OUTPUT);          // declare LED as output
  pinMode(pirPin, INPUT);           // declare sensor as input
  pinMode(vibPin, INPUT);

  Serial.begin(9600);

  // Initialize GSM connection
  while (!gsmAccess.begin()) {
    Serial.println("Failed to connect to GSM network");
    delay(1000);
  }
  Serial.println("GSM connected");
}

void loop() {
  unsigned long currentTime = millis();  // current time
  pirVal = digitalRead(pirPin);          // read input value
  vibVal = digitalRead(vibPin);

  // Check if both PIR sensor and vibration are activated
  if (pirVal == HIGH && vibVal == 1) {   
    if (currentTime - lastOutputTime >= outputDelay) {
      Serial.println("Vibration detected!");
      if (pirState == LOW) {
        // we have just turned on
        Serial.println("Motion detected!");
        pirState = HIGH;
        // Update activity count and time
        unsigned long timeDiff = currentTime - lastActivityTime;
        if (timeDiff > activityWindow) {
          // If the last activity was outside the window, reset count
          activityCount = 1;
        } else {
          activityCount++;
        }
        lastActivityTime = currentTime;
        // Check if activity count reaches 3 within the window
        if (activityCount >= 3) {
          digitalWrite(ledPin, HIGH);    // Turn on the LED
          unsigned long messageTimeDiff = currentTime - lastMessageTime;
          if (messageTimeDiff >= messageDelay) {
            sendTextMessage("Activity count reached 3!"); // Send text message
            lastMessageTime = currentTime; // Update last message time
          }
        }
        Serial.print("Activity count within last 10 minutes: ");
        Serial.println(activityCount);
      }
      lastOutputTime = currentTime;  // update last output time
    }
  } else {
    if (pirState == HIGH){
      // we have just turned off
      Serial.println("Motion ended!");
      pirState = LOW;
      lastOutputTime = currentTime;  // update last output time
    }
  }
}

void sendTextMessage(String message) {
  Serial.println("Sending SMS...");
  // Replace "your_template_phone_number" with your actual phone number
  String phoneNumber = "your_template_phone_number";
  GSM_SMS sms;

  // SMS sending
  sms.beginSMS(phoneNumber);
  sms.print(message);
  sms.endSMS();
  Serial.println("SMS sent successfully.");
}
