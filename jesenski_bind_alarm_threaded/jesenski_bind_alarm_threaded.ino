const int PIR_PIN = 2;          // SOUT on PIR sensor
const int BUTTON_PIN = 3;       // Button for arm/disarm
const int GREEN_LED = 4;        // Green LED: armed
const int RED_LED = 5;          // Red LED: disarmed
const int BLUE_LED = 6;         // Blue LED: motion detected
const int BUZZER_PIN = 8;       // Speaker/piezzo

bool armed = false;
int button_state = HIGH;         
int last_button_reading = HIGH;   
unsigned long last_debounce_time = 0;
const unsigned long DEBOUNCE_DELAY = 50;  

unsigned long alarm_start_time = 0;
const unsigned long ALARM_DURATION = 30000;  

bool siren_active = false;
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
  int reading = digitalRead(BUTTON_PIN);
  if (reading != last_button_reading) {
    last_debounce_time = millis();
  }
  if ((millis() - last_debounce_time) > DEBOUNCE_DELAY) {
    if (reading != button_state) {
      button_state = reading;
      if (button_state == LOW) {  
        armed = !armed;
        noTone(BUZZER_PIN);     
        siren_active = false;
        alarm_start_time = 0;

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
  last_button_reading = reading;

  bool motion = (digitalRead(PIR_PIN) == HIGH);

  if (armed && motion) {
    digitalWrite(BLUE_LED, HIGH);

    if (!siren_active) {
      siren_active = true;
      alarm_start_time = millis(); 
    }

    unsigned long current_time = millis();

    if (ALARM_DURATION > 0 && current_time - alarm_start_time > ALARM_DURATION) {
      noTone(BUZZER_PIN);
      return;  
    }

    unsigned long siren_time = current_time % SIREN_CYCLE;
    int freq;

    if (siren_time < SIREN_CYCLE / 2) {
      freq = map(siren_time, 0, SIREN_CYCLE / 2, MIN_FREQ, MAX_FREQ);  
    } else {
      freq = map(siren_time - SIREN_CYCLE / 2, 0, SIREN_CYCLE / 2, MAX_FREQ, MIN_FREQ);  
    }
    tone(BUZZER_PIN, freq);

  } else {
    digitalWrite(BLUE_LED, LOW);
    noTone(BUZZER_PIN);
    siren_active = false;
    alarm_start_time = 0;
  }
}