const int PIR_PIN = 13;          // SOUT on PIR sensor
const int BUTTON_PIN = 3;       // Button for arm/disarm
const int GREEN_LED = 4;        // Green LED: armed
const int RED_LED = 5;          // Red LED: disarmed
const int BLUE_LED = 6;         // Blue LED: motion detected
const int BUZZER_PIN = 8;       // Speaker (piezo buzzer)

bool armed = false;
int buttonState = HIGH;         
int lastButtonReading = HIGH;   
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;  

unsigned long alarmStartTime = 0;
const unsigned long ALARM_DURATION = 30000;  

bool sirenActive = false;
const int MIN_FREQ = 600;       
const int MAX_FREQ = 1200;      
const unsigned long SIREN_CYCLE = 2000;  

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Internal pull-up
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);


  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
  noTone(BUZZER_PIN);
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
        noTone(BUZZER_PIN);      // Stop siren immediately
        sirenActive = false;
        alarmStartTime = 0;

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

  // ── Motion detection & siren (only when armed) ────────────
  bool motion = (digitalRead(PIR_PIN) == HIGH);

  if (armed && motion) {
    digitalWrite(BLUE_LED, HIGH);

    if (!sirenActive) {
      sirenActive = true;
      alarmStartTime = millis();  // Start timer
    }

    unsigned long currentTime = millis();

    // Check if alarm duration exceeded
    if (ALARM_DURATION > 0 && currentTime - alarmStartTime > ALARM_DURATION) {
      noTone(BUZZER_PIN);
      return;  // Skip siren until next trigger
    }

    // Non-blocking siren sweep
    unsigned long sirenTime = currentTime % SIREN_CYCLE;
    int freq;
    if (sirenTime < SIREN_CYCLE / 2) {
      freq = map(sirenTime, 0, SIREN_CYCLE / 2, MIN_FREQ, MAX_FREQ);  // Sweep up
    } else {
      freq = map(sirenTime - SIREN_CYCLE / 2, 0, SIREN_CYCLE / 2, MAX_FREQ, MIN_FREQ);  // Sweep down
    }
    tone(BUZZER_PIN, freq);
  } else {
    digitalWrite(BLUE_LED, LOW);
    noTone(BUZZER_PIN);
    sirenActive = false;
    alarmStartTime = 0;
  }
}