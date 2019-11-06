//Receiver program
#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

#include"ServoLib.h"
#define SERVO_PIN A3

Servo_Test mServo(SERVO_PIN);  // Create a servo motor object
Nrf24l Mirf = Nrf24l(10, 9);
int value;

void setup()
{
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"WirelessSwitch"); //Set your own address (receiver address) using 5 characters
  Mirf.payload = sizeof(value);
  Mirf.channel = 90;             //Set the used channel
  Mirf.config();
  Serial.println("Listening...");  //Start listening to received data
}

void loop()
{
  if (Mirf.dataReady()) { //When the program is received, the received data is output from the serial port
    Mirf.getData((byte *) &value);
    Serial.print("Got an data: ");
    Serial.println(value);
    if (value == 10 ) {
       Serial.println(10);
        mServo.SetServoDegree(20);
    } else if (value == 20 ) {
        Serial.println(20);
         mServo.SetServoDegree(90);
    }
  }
}
