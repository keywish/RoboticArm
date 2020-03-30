#include "Panther_Tank.h"
#include "ProtocolParser.h"
#include "Sounds.h"
#include "KeyMap.h"
#include "debug.h"

//MotorDriverBoard V4.0
Tank::Tank(ProtocolParser *Package): SmartCar("Tank", E_PANTHER_TANK, 0x01, E_BLUETOOTH_CONTROL)
{
  LeftFoward = RightFoward = LeftBackward = RightBackward = NULL;
  IR = NULL;
  Buzzer  = NULL;
  Rgb = NULL;
  //UT = NULL;
  Ps2x = NULL;
  Nrf24L01 = NULL;
  mServo1 = NULL;
  mServo2 = NULL;
  mServo3 = NULL;
  mServo4 = NULL;
  mServo5 = NULL;
  mServo6 = NULL;
  SetStatus(E_STOP);
  mProtocolPackage = Package;
}

Tank::~Tank()
{
  delete LeftFoward;
  delete RightFoward;
  delete LeftBackward;
  delete RightBackward;
  delete IR;
  delete Buzzer;
  delete Rgb;
  delete Ps2x;
  delete Nrf24L01;
  delete mServo1;
  delete mServo2;
  delete mServo3;
  delete mServo4;
  delete mServo5;
  delete mServo6;
  delete mServo7;
  delete mServo8;
}

void Tank::init(int leftward, int rightfoward, int leftbackward, int rightbackward)
{
    MotorDriver = Emakefun_MotorDriver(0x60,MOTOR_DRIVER_BOARD_V5);
    Sensors = (Emakefun_Sensor *)MotorDriver.getSensor(E_SENSOR_MAX);
    LeftFoward = MotorDriver.getMotor(leftward);
    RightFoward = MotorDriver.getMotor(rightfoward);
    LeftBackward = MotorDriver.getMotor(leftbackward);
    RightBackward = MotorDriver.getMotor(rightbackward);
    delay(200);
    MotorDriver.begin(50);
}

void Tank::init(int leftward, int rightfoward)
{
    MotorDriver = Emakefun_MotorDriver(0x60,MOTOR_DRIVER_BOARD_V5);
    Sensors = (Emakefun_Sensor *)MotorDriver.getSensor(E_SENSOR_MAX);
    LeftFoward = MotorDriver.getMotor(leftward);
    RightFoward = MotorDriver.getMotor(rightfoward);
    delay(200);
    MotorDriver.begin(50);
}


#if ARDUINO > 10609
void Tank::Move(int directions = 1)
#else
void Tank::Move(int directions)
#endif
{
  if (directions == 1) {
    GoForward();
  } else if (directions == 2) {
    GoBack();
  } else if (directions == 3) {
    TurnLeft();
  } else if (directions == 4) {
    TurnRight();
  } else {
    KeepStop();
  }
}

void Tank::DriveSpeed(int s)
{
    if (s >= 0 && s <= 100) {
        LeftFoward->setSpeed((s / 10) * 25.5);
        RightFoward->setSpeed((s / 10) * 25.5);
        LeftBackward->setSpeed((s / 10) * 25.5);
        RightBackward->setSpeed((s / 10) * 25.5);
    }
}

void Tank::GoForward(void)
{
    SetStatus(E_FORWARD);
    DriveSpeed(Speed);
    LeftFoward->run(FORWARD);
    RightFoward->run(FORWARD);
    LeftBackward->run(FORWARD);
    RightBackward->run(FORWARD);
}

void Tank::GoBack(void)
{
    SetStatus(E_BACK);
    DriveSpeed(Speed);
    LeftFoward->run(BACKWARD);
    RightFoward->run(BACKWARD);
    LeftBackward->run(BACKWARD);
    RightBackward->run(BACKWARD);
}
void Tank::KeepStop(void)
{
    SetStatus(E_STOP);
    DriveSpeed(0);
    LeftFoward->run(BRAKE);
    RightFoward->run(BRAKE);
    LeftBackward->run(BRAKE);
    RightBackward->run(BRAKE);
}

void Tank::TurnLeft(void)
{
    int s;
    s = (Speed / 10) * 25.5;
    SetStatus(E_LEFT);
      LeftFoward->setSpeed(s/2);
      LeftBackward->setSpeed(s/2);
      RightFoward->setSpeed(s);
      RightBackward->setSpeed(s);
      LeftFoward->run(FORWARD);
      RightFoward->run(FORWARD);
      LeftBackward->run(FORWARD);
      RightBackward->run(FORWARD);
}

void Tank::TurnRight(void)
{
    int s;
    SetStatus(E_RIGHT);
    s = (Speed / 10) * 25.5;
      LeftFoward->setSpeed(s);
      LeftBackward->setSpeed(s);
      RightFoward->setSpeed(s/2);
      RightBackward->setSpeed(s/2);
      LeftFoward->run(FORWARD);
      RightFoward->run(FORWARD);
      LeftBackward->run(FORWARD);
      RightBackward->run(FORWARD);
}

void Tank::TurnLeftRotate(void)
{
    SetStatus(E_LEFT_ROTATE);
    DriveSpeed(Speed);
    LeftFoward->run(BACKWARD);
    LeftBackward->run(BACKWARD);
    RightFoward->run(FORWARD);
    RightBackward->run(FORWARD);
}

void Tank::TurnRightRotate(void)
{
    SetStatus(E_RIGHT_ROTATE);
    DriveSpeed(Speed);
    LeftFoward->run(FORWARD);
    LeftBackward->run(FORWARD);
    RightFoward->run(BACKWARD);
    RightBackward->run(BACKWARD);
}

void Tank::Drive(void)
{
  Drive(Degree);
}

void Tank::Drive(int degree)
{
  DEBUG_LOG(DEBUG_LEVEL_INFO, "degree = %d speed = %d\n", degree, Speed);
  byte value = (Speed / 10) * 25.5;	 //app contol hbot_speed is 0 ~ 100 ,pwm is 0~255
  float f;
  if (degree >= 0 && degree <= 90) {
    f = (float)(degree) / 90;
    if (degree <= 5) {
      SetStatus(E_RIGHT);
    } else  if (degree >= 85) {
      SetStatus(E_FORWARD);
    }
    LeftFoward->setSpeed(value);
    RightFoward->setSpeed((float)(value * f));
    LeftBackward->setSpeed(value);
    RightBackward->setSpeed((float)(value * f));
    LeftFoward->run(FORWARD);
    RightFoward->run(FORWARD);
    LeftBackward->run(FORWARD);
    RightBackward->run(FORWARD);
  } else if (degree > 90 && degree <= 180) {
    f = (float)(180 - degree) / 90;
    if (degree <= 95) {
      SetStatus(E_FORWARD);
    } else  if (degree >= 175) {
      SetStatus(E_LEFT);
    }
    LeftFoward->setSpeed((float)(value * f));
    RightFoward->setSpeed(value);
    LeftBackward->setSpeed((float)(value * f));
    RightBackward->setSpeed(value);
    LeftFoward->run(FORWARD);
    RightFoward->run(FORWARD);
    LeftBackward->run(FORWARD);
    RightBackward->run(FORWARD);
  } else if (degree > 180 && degree <= 270) {
    f = (float)(degree - 180) / 90;
    if (degree <= 185) {
      SetStatus(E_LEFT);
    } else  if (degree >= 265) {
      SetStatus(E_BACK);
    }
    LeftFoward->setSpeed((float)(value * f));
    RightFoward->setSpeed(value);
    LeftBackward->setSpeed((float)(value * f));
    RightBackward->setSpeed(value);
    LeftFoward->run(BACKWARD);
    RightFoward->run(BACKWARD);
    LeftBackward->run(BACKWARD);
    RightBackward->run(BACKWARD);
  } else if (degree > 270 && degree <= 360) {
    f = (float)(360 - degree) / 90;
    if (degree <= 275) {
      SetStatus(E_BACK);
    } else  if (degree >= 355) {
      SetStatus(E_RIGHT);
    }
    LeftFoward->setSpeed(value);
    RightFoward->setSpeed((float)(value * f));
    LeftBackward->setSpeed(value);
    RightBackward->setSpeed((float)(value * f));
    LeftFoward->run(BACKWARD);
    RightFoward->run(BACKWARD);
    LeftBackward->run(BACKWARD);
    RightBackward->run(BACKWARD);
  }
  else {
    KeepStop();
    return;
  }
}

void Tank::InitIr(void)
{
  IR = MotorDriver.getSensor(E_IR);
}

void Tank::InitBuzzer(void)
{
  Buzzer = MotorDriver.getSensor(E_BUZZER);
}

void Tank::sing(byte songName)
{
  Sensors->Sing(songName);
}

void Tank::PianoSing(ST_MUSIC_TYPE music)
{
  Buzzer->_tone(music.note, music.beat, 2);
}

void Tank::InitRgb(void)
{
  Rgb = MotorDriver.getSensor(E_RGB);
}

void Tank::SetRgbColor(E_RGB_INDEX index , long Color)
{
  Sensors->SetRgbColor(index, Color);
}

void Tank::LightOff(void)
{
  Sensors->SetRgbColor(E_RGB_ALL, RGB_BLACK);
}

void Tank::SetRgbEffect(E_RGB_INDEX index, long Color, uint8_t effect)
{
  if (Rgb != NULL) {
    switch ((E_RGB_EFFECT)effect) {
      case E_EFFECT_BREATHING:
        for (long i = 0; i < 256; i++) {
          SetRgbColor(index, (i << 16) | (i << 8) | i);
          delay((i < 18) ? 18 : (256 / i));
        }
        for (long i = 255; i >= 0; i--) {
          SetRgbColor(index, (i << 16) | (i << 8) | i);
          delay((i < 18) ? 18 : (256 / i));
        }
        break;
      case E_EFFECT_FLASH:
        for (byte i = 0; i < 6; i++) {
          SetRgbColor(index, Color);
          delay(100);
          SetRgbColor(index, 0);
          delay(100);
        }
        break;
    }
  }
}

void Tank::InitUltrasonic(void)
{
  MotorDriver.getSensor(E_ULTRASONIC);
}

byte Tank::GetUltrasonicValue(byte direction)
{
  byte distance;
  if (direction == 0) {
    SetServoDegree(1, 90);
    distance = Sensors->GetUltrasonicDistance();
  } else if (direction == 1) {
    SetServoDegree(1, 180);
    distance = Sensors->GetUltrasonicDistance();
    delay(400);
    SetServoDegree(1, 90);
  } else if (direction == 2) {
    SetServoDegree(1, 15);
    distance = Sensors->GetUltrasonicDistance();
    delay(400);
    SetServoDegree(1, 90);
  }
  return distance;
}

void Tank::InitServo(void)
{
  mServo1 = MotorDriver.getServo(1);
  mServo2 = MotorDriver.getServo(2);
  mServo3 = MotorDriver.getServo(3);
  mServo4 = MotorDriver.getServo(4);
  mServo5 = MotorDriver.getServo(5);
  mServo6 = MotorDriver.getServo(6);
  mServo7 = MotorDriver.getServo(7);
  mServo8 = MotorDriver.getServo(8);
}

void Tank::SetServoBaseDegree(uint8_t base)
{
  ServoBaseDegree = base;
}

void Tank::SetServoDegree(byte pin , byte Angle)
{
  int Degree = Angle;
  int servo_degree;
  ServoPin = pin;
  if (Degree > 360) {
    return;
  }
  if (Degree == 90 || Degree == 270) {
    servo_degree = ServoBaseDegree;
  } else if (Degree >= 0 && Degree <= 180) {
    servo_degree = ServoBaseDegree - 90 + Degree;   // 180-degree-diff
  }
  if (ServoPin == 1)
    mServo1->writeServo(servo_degree);
  else if (ServoPin == 2)
    mServo2->writeServo(Angle);
  else if (ServoPin == 3)
    mServo3->writeServo(Angle);
  else if (ServoPin == 4)
    mServo4->writeServo(Angle);
  else if (ServoPin == 5)
    mServo5->writeServo(Angle);
  else if (ServoPin == 6)
    mServo6->writeServo(Angle);
}

void Tank::InitPs2x(void)
{
  Ps2x = MotorDriver.getSensor(E_PS2X);
}

uint16_t Tank::GetPs2xRockerAngle(byte direction)
{
  //  Ps2x->mPs2x->read_gamepad(false, 0);
  //  if (direction == 1)
  //    return (Ps2x->LeftHart());
  //  else if (direction == 2)
  //    return (Ps2x->);
}

void Tank::InitNrf24L01(char *Rxaddr)
{
  Nrf24L01 = MotorDriver.getSensor(E_NRF24L01);
  Nrf24L01->setRADDR(Rxaddr);
}

void Tank::SendUltrasonicData(void)
{
  union
  {
    float d;
    byte data[4];
  } value;
  value.d = GetUltrasonicValue(FRONT);
  SendData.start_code = 0xAA;
  SendData.type = E_PANTHER_TANK;
  SendData.addr = 0x01;
  SendData.function = E_ULTRASONIC_MODE;
  SendData.data = value.data;
  SendData.len = 10;
  SendData.end_code = 0x55;
  mProtocolPackage->SendPackage(&SendData, 4);
}
