#define LED_BLUE A2
#define LED_GREEN A1
#define DELAY 500
void setup()
{
  Serial.begin(9600);
}
void loop()
{
  pinMode(LED_BLUE,OUTPUT);
  digitalWrite(LED_BLUE,HIGH);
  delay(DELAY);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,HIGH);
  delay(DELAY);
  digitalWrite(LED_GREEN,LOW);
}
