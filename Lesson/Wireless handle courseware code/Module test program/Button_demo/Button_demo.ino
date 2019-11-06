#define BUTTON_UP 3
#define BUTTON_RIGHT 2
#define BUTTON_DOWN 5
#define BUTTON_LEFT 4
#define BUTTON_R 6
#define BUTTON_L 7
#define BUTTON_SELECT 8
#define BUTTON_MODE A3
#define BUTTON_ENTER A0
#define DELAY 500
void setup() {
  
 Serial.begin(9600);
 pinMode(BUTTON_UP, INPUT);
 digitalWrite(BUTTON_UP, HIGH);
 pinMode(BUTTON_RIGHT, INPUT);
 digitalWrite(BUTTON_RIGHT, HIGH);
 pinMode(BUTTON_DOWN, INPUT);
 digitalWrite(BUTTON_DOWN, HIGH);
 pinMode(BUTTON_LEFT, INPUT);
 digitalWrite(BUTTON_LEFT, HIGH);
 pinMode(BUTTON_L, INPUT);
 digitalWrite(BUTTON_L, HIGH);
 pinMode(BUTTON_R, INPUT);
 digitalWrite(BUTTON_R, HIGH);
 pinMode(BUTTON_SELECT, INPUT);
 digitalWrite(BUTTON_SELECT, HIGH);
 pinMode(BUTTON_MODE, INPUT);
 digitalWrite(BUTTON_MODE, HIGH);
 pinMode(BUTTON_ENTER, INPUT);
 digitalWrite(BUTTON_ENTER, HIGH);
}
void loop() {
 if(digitalRead(BUTTON_UP) == LOW) {
   Serial.println("Button UP is pressed");
   delay(DELAY);
 }
 else if(digitalRead(BUTTON_RIGHT) == LOW) {
   Serial.println("Button RIGHT is pressed");
   delay(DELAY);
 }
 else if(digitalRead(BUTTON_DOWN) == LOW) {
   Serial.println("Button DOWN is pressed");
   delay(DELAY);
 }
 else if(digitalRead(BUTTON_LEFT) == LOW) {
   Serial.println("Button LEFT is pressed");
   delay(DELAY);
 }
 else if(digitalRead(BUTTON_L) == LOW) {
   Serial.println("Button L is pressed");
   delay(DELAY);
 }
 else if(digitalRead(BUTTON_R) == LOW) {
   Serial.println("Button R is pressed");
   delay(DELAY);
 }
  else if(digitalRead(BUTTON_SELECT) == LOW) {
   Serial.println("Button SELECT is pressed");
   delay(DELAY);
 }
  else if(digitalRead(BUTTON_MODE) == LOW) {
   Serial.println("Button MODE is pressed");
   delay(DELAY);
 }  else if(digitalRead(BUTTON_ENTER) == LOW) {
   Serial.println("Button ENTER is pressed");
   delay(DELAY);
 }
}
