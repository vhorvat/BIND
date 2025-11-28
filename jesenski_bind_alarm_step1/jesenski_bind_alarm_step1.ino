const int BLUE_LED = 6;         // Blue LED: blinking

unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 500;  
bool blueLedState = LOW;


void setup() {
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
    lastBlinkTime = currentTime;
    blueLedState = !blueLedState;
    digitalWrite(BLUE_LED, blueLedState);
  }
}
