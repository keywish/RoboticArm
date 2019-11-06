#define PIN_ANALOG_X A6
#define PIN_ANALOG_Y A7
void setup() {
 Serial.begin(9600);
}

void loop() {
 Serial.print("x: ");
 Serial.println(analogRead(PIN_ANALOG_X));
 Serial.print("y: ");
 Serial.println(analogRead(PIN_ANALOG_Y));
 delay(500);
}
