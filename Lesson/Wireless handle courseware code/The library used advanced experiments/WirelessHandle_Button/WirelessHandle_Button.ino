#include "WirelessHandle.h"
#include "ProtocolParser.h"
#include "debug.h"

WirelessHandle mHandle(E_BLUTOOTH_MODE);

byte readbyte(void) {
    if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
        return Serial.read();
    } else {
        return;
    }
}

int writedata(byte buf[], int len)
{
    if ((mHandle.GetControlMode() == E_BLUTOOTH_MODE) || (mHandle.GetControlMode() == E_WIFI_MODE)) {
        Serial.write(buf, len);
        Serial.flush();
        return len;
    } else {
        mHandle.mNrf24L01->setTADDR((uint8_t *)"WirelessHandle");           //Set the receiver address
        mHandle.mNrf24L01->payload = 12;
        mHandle.mNrf24L01->send(buf);
        while (mHandle.mNrf24L01->isSending())
           delay(2);
    }
}

ProtocolParser Package(0xAA, 0x55, readbyte, writedata);


void setup()
{
    Serial.begin(9600);
    mHandle.SetControlSelect(E_WIRELESS_CONTROL_MODE);
    mHandle.SetControlMode(E_NRF24L01_MODE);   //E_BLUETOOTH_CONTROL //E_NRF24L01_MODE //E_WIFI_MODE
    mHandle.InitNrf24l01(NRF24L01_CE, NRF24L01_CSN);
    mHandle.InitMpu6050();
    mHandle.mProtocolPackage = &Package;
}

void loop() {
    mHandle.ScanKeyPad();
    int ss =mHandle.ButtonDataByte();
    if (mHandle.ButtonDataByte()) {
        if (mHandle.ButtonPressed(BT_PAD_UP)) {
            Serial.println("BT_PAD_UP Button Pressed");
//            Package.SendButton(BT_PAD_UP);
        } else if (mHandle.ButtonPressed(BT_PAD_DOWN)) {
            Serial.println("BT_PAD_DOWN Button Pressed");
//            Package.SendButton(BT_PAD_DOWN);
        } else if (mHandle.ButtonPressed(BT_PAD_LEFT)) {
            Serial.println("BT_PAD_LEFT Button Pressed");
//            Package.SendButton(BT_PAD_LEFT);
        } else if (mHandle.ButtonPressed(BT_PAD_RIGHT)) {
            Serial.println("BT_PAD_RIGHT Button Pressed");
//            Package.SendButton(BT_PAD_RIGHT);
        } else if (mHandle.ButtonPressed(BT_R)) {
            Serial.println("BT_R Button Pressed");
//            Package.SendButton(BT_R);
        } else if (mHandle.ButtonPressed(BT_L)) {
            Serial.println("BT_L Button Button Pressed");
            Serial.println(mHandle.Analog(BT_X));
            Serial.println(mHandle.Analog(BT_Y));
//            Package.SendButton(BT_L);
        } else if (mHandle.ButtonPressed(BT_ENTER)) {
            Serial.println("BT_ENTER Button Pressed");
//            Package.SendButton(BT_ENTER);
        } else if (mHandle.ButtonPressed(BT_SELECT)) {
            Serial.println("BT_SELECT pressed");
//            Package.SendButton(BT_SELECT);
        } else if (mHandle.ButtonPressed(BT_MODE)) {
            Serial.println("BT_MODE pressed");
//            Package.SendButton(BT_MODE);
        }
        if (mHandle.Button(BT_PAD_UP)) {
            Serial.println("BT_PAD_UP Button");
            //mHandle.SendButton(BT_PAD_UP);
        } else if (mHandle.Button(BT_PAD_DOWN)) {
            Serial.println("BT_PAD_DOWN Button");
            //mHandle.SendButton(BT_PAD_DOWN);
        } else if (mHandle.Button(BT_PAD_LEFT)) {
            Serial.println("BT_PAD_LEFT Button");
            //mHandle.SendButton(BT_PAD_LEFT);
        } else if (mHandle.Button(BT_PAD_RIGHT)) {
            Serial.println("BT_PAD_RIGHT Button");
            //mHandle.SendButton(BT_PAD_RIGHT);
        } else if (mHandle.Button(BT_R)) {
            Serial.println("BT_R Button");
            //mHandle.SendButton(BT_R);
        } else if (mHandle.Button(BT_L)) {
            Serial.println("BT_L Button Button");
            Serial.println(mHandle.Analog(BT_X));
            Serial.println(mHandle.Analog(BT_Y));
            //mHandle.SendButton(BT_L);
        } else if (mHandle.Button(BT_ENTER)) {
            Serial.println("BT_ENTER Button");
            //mHandle.SendButton(BT_ENTER);
        } else if (mHandle.Button(BT_SELECT)) {
            Serial.println("BT_SELECT pressed");
        } else if (mHandle.Button(BT_MODE)) {
            Serial.println("BT_MODE pressed");
        }
       int degree = mHandle.GetDegree();
//       Package.SendDirection(degree);
       Serial.print("degree");
       Serial.println(degree);
        delay(100);
    }
}
