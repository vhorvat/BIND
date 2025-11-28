const int PIR_PIN = 13;          // SOUT on PIR sensor
const int BUTTON_PIN = 3;       // Button for arm/disarm
const int GREEN_LED = 4;        // Green LED: armed
const int RED_LED = 5;          // Red LED: disarmed
const int BLUE_LED = 6;         // Blue LED: motion detected


bool armed = false;
int buttonState = HIGH;         
int lastButtonReading = HIGH;   
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;  


void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Internal pull-up
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);


  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
}


void loop() {
  // ── Button handling with debounce ─────────────────────────
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {  // Button pressed
        armed = !armed;


        if (armed) {
          digitalWrite(GREEN_LED, HIGH);
          digitalWrite(RED_LED, LOW);
        } else {
          digitalWrite(GREEN_LED, LOW);
          digitalWrite(RED_LED, HIGH);
          digitalWrite(BLUE_LED, LOW);
        }
      }
    }
  }
  lastButtonReading = reading;


  bool motion = (digitalRead(PIR_PIN) == HIGH);


  if (armed && motion) {
    digitalWrite(BLUE_LED, HIGH);
  } else {
    digitalWrite(BLUE_LED, LOW);
  }
}
