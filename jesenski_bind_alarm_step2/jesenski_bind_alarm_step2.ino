const int BUTTON_PIN = 3;       // Button for arm/disarm
const int GREEN_LED = 4;        // Green LED: armed
const int RED_LED = 5;          // Red LED: disarmed
const int BLUE_LED = 6;         // Blue LED: blinking


bool armed = false;
bool blueBlinkEnabled = false;
int buttonState = HIGH;         
int lastButtonReading = HIGH;   
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;

unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 500;  
bool blueLedState = LOW;


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);


  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
}


void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {  
        armed = !armed;
        blueBlinkEnabled = !blueBlinkEnabled;


        if (armed) {
          digitalWrite(GREEN_LED, HIGH);
          digitalWrite(RED_LED, LOW);
        } else {
          digitalWrite(GREEN_LED, LOW);
          digitalWrite(RED_LED, HIGH);
        }
        
        if (!blueBlinkEnabled) {
          digitalWrite(BLUE_LED, LOW);
        }
      }
    }
  }
  lastButtonReading = reading;

  if (blueBlinkEnabled) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = currentTime;
      blueLedState = !blueLedState;
      digitalWrite(BLUE_LED, blueLedState);
    }
  }
}
