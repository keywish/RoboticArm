#include<Arduino.h>
#include<Wire.h>
#include "Panther_Tank.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "debug.h"
#include "KeyMap.h"
ProtocolParser *mProtocol = new ProtocolParser();
Tank mTank(mProtocol);
byte count = 0;
uint16_t Ps2xLeftAngle, Ps2xRightAngle;

void setup()
{
  Serial.begin(9600);
  mTank.init(M2, M1);
  mTank.SetControlMode(E_BLUETOOTH_CONTROL);
  mTank.InitServo();
  mTank.InitPs2x();
  mTank.InitRgb();
  mTank.InitBuzzer();
  mTank.SetSpeed(60);
  mTank.InitUltrasonic();
  mTank.SetServoBaseDegree(90);
  mTank.SetServoDegree(1, 90);
  mTank.SetServoDegree(2, 90);
  mTank.SetServoDegree(3, 90);
  mTank.SetServoDegree(4, 90);
}

void HandleBluetoothRemote(bool recv_flag)
{
  if (recv_flag) {
    switch (mProtocol->GetRobotControlFun()) {
      case E_BUTTON:
        switch (mProtocol->GetBluetoothButton())
        {
          case BT_PAD_UP:
            mTank.GoForward();
            break;
          case BT_PAD_DOWN:
            mTank.GoBack();
            break;
          case BT_PAD_LEFT:
            mTank.Drive(160);
            break;
          case BT_PAD_RIGHT:
            mTank.Drive(20);
            break;
          case BT_PINK:
            mTank.TurnLeft();
            break;
          case BT_RED:
            mTank.TurnRight();
            break;
          case BT_GREEN:
            mTank.sing(S_connection);
            mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
            mTank.SpeedUp(10);
            break;
          case BT_BLUE:
            mTank.sing(S_disconnection);
            mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
            mTank.SpeedDown(10);
            break;
        }
        break;
      case E_ROBOT_CONTROL_DIRECTION:
        mTank.Drive(mProtocol->GetRobotDegree());
        break;
      case E_ROBOT_CONTROL_SPEED:
        mTank.SetSpeed(mProtocol->GetRobotSpeed());
        break ;
      case E_CONTROL_MODE:
        mTank.SetControlMode(mProtocol->GetControlMode());
        break;
      case E_LED:
        mTank.SetRgbColor(E_RGB_ALL, mProtocol->GetRgbValue());
        break;
      case E_SERVER_DEGREE:
        mTank.SetServoDegree(mProtocol->GetServoDegreeNum(), mProtocol->GetServoDegree());
        break;
      case E_BUZZER_MODE:
        if (mProtocol->GetBuzzerMode() == E_SOUND) {
          mTank.sing(mProtocol->GetBuzzerSound());
        }
        break;
    }
    mTank.LightOff();
  }
}

void HandlePS2(void)
{
  int Ps2xKey;

  mTank.Ps2x->read_gamepad(false, 0);
  if (mTank.Ps2x->ButtonDataByte()) {
    if (mTank.Ps2x->Button(PSB_TRIANGLE)) {
      mTank.sing(S_connection);
      mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      mTank.SpeedUp(5);
      return;
    }
    if (mTank.Ps2x->Button(PSB_CROSS)) {
      mTank.sing(S_disconnection);
      mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      mTank.SpeedDown(5);
      return;
    }
    if (mTank.Ps2x->Button(PSB_PAD_UP)) {
      mTank.GoForward();
    }
    if (mTank.Ps2x->Button(PSB_PAD_DOWN)) {
      mTank.GoBack();
    }
    if (mTank.Ps2x->Button(PSB_PAD_LEFT)) {
      mTank.TurnLeft();
    }
    if (mTank.Ps2x->Button(PSB_PAD_RIGHT)) {
      mTank.TurnRight();
    }
    if (mTank.Ps2x->Button(PSB_SQUARE)) {
      mTank.TurnLeftRotate();
    }
    if (mTank.Ps2x->Button(PSB_CIRCLE)) {
      mTank.TurnRightRotate();
    }
  } else
    mTank.KeepStop();

  delay(50);
}


void HandleInfaredRemote (byte irKeyCode)
{
  switch ((E_EM_IR_KEYCODE) mTank.IR->getIrKey(irKeyCode, IR_TYPE_NORMAL)) {
    case IR_KEYCODE_STAR:
      mTank.sing(S_connection);
      mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      mTank.SpeedUp(10);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "mTank.Speed = %d \n", mTank.Speed);
      break;
    case IR_KEYCODE_POUND:
      DEBUG_LOG(DEBUG_LEVEL_INFO, " start Degree = %d \n", mTank.Degree);
      mTank.sing(S_disconnection);
      mTank.SetRgbColor(E_RGB_ALL, mTank.GetSpeed() * 2.5);
      mTank.SpeedDown(10);
      break;
    case IR_KEYCODE_UP:
      mTank.GoForward();
      break;
    case IR_KEYCODE_DOWN:
      mTank.GoBack();
      break;
    case IR_KEYCODE_OK:
      mTank.KeepStop();
      break;
    case IR_KEYCODE_LEFT:
      mTank.TurnLeft();
      break;
    case IR_KEYCODE_RIGHT:
      mTank.TurnRight();
      break;
    default:
      break;
  }
}

void HandleUltrasonicAvoidance(void)
{
  uint16_t UlFrontDistance, UlLeftDistance, UlRightDistance;
  UlFrontDistance = mTank.GetUltrasonicValue(0);
  if (count++ > 50) {
    mTank.SendUltrasonicData();
    count = 0;
  }
  DEBUG_LOG(DEBUG_LEVEL_INFO, "UlFrontDistance = %d \n", UlFrontDistance);
  if (UlFrontDistance < UL_LIMIT_MIN)
  {
    mTank.SetSpeed(80);
    mTank.GoBack();
    delay(200);
  }
  if (UlFrontDistance < UL_LIMIT_MID)
  {
    mTank.KeepStop();
    delay(100);
    UlRightDistance = mTank.GetUltrasonicValue(2);
    delay(50);
    UlLeftDistance = mTank.GetUltrasonicValue(1);
    if ((UlRightDistance > UL_LIMIT_MIN) && (UlRightDistance < UL_LIMIT_MAX)) {
      mTank.SetSpeed(100);
      mTank.TurnRight();
      delay(400);
    } else if ((UlLeftDistance > UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MAX)) {
      mTank.SetSpeed(100);
      mTank.TurnLeft();
      delay(400);
    } else if ((UlRightDistance < UL_LIMIT_MIN) && (UlLeftDistance < UL_LIMIT_MIN) ) {
      mTank.SetSpeed(400);
      mTank.TurnLeft();
      delay(800);
    }
  } else {
    mTank.SetSpeed(80);
    mTank.GoForward();
  }
}

void UltrasonicFollow()
{
  mTank.SetSpeed(40);
  uint16_t UlFrontDistance =  mTank.GetUltrasonicValue(FRONT);
  delay(10);
  if (UlFrontDistance < 10) {
    mTank.GoBack();
  } else if (UlFrontDistance > 14) {
    mTank.GoForward();
  } else if (10 <= UlFrontDistance <= 4) {
    mTank.KeepStop();
  }
}

void loop()
{
  static byte mode;
  static bool recv_flag;
  mProtocol->RecevData();
  if (recv_flag = mProtocol->ParserPackage()) {
    if (mProtocol->GetRobotControlFun() == E_CONTROL_MODE) {
      mTank.SetControlMode(mProtocol->GetControlMode());
      return;
    }
  }
  switch (mTank.GetControlMode()) {
    case E_BLUETOOTH_CONTROL:
      HandleBluetoothRemote(recv_flag);
      DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUETOOTH_CONTROL \n");
      break;
    case E_INFRARED_REMOTE_CONTROL:
      byte irKeyCode;
      if (mode != E_INFRARED_REMOTE_CONTROL) {
        mTank.InitIr();
        mode = E_INFRARED_REMOTE_CONTROL;
      }
      if (irKeyCode = mTank.IR->getCode())
      {
        DEBUG_LOG(DEBUG_LEVEL_INFO, "irKeyCode = %lx \n", irKeyCode);
        HandleInfaredRemote(irKeyCode);
        delay(110);
      } else {
        if (mTank.GetStatus() != E_STOP ) {
          mTank.KeepStop();
        }
      }
      break;
    case E_ULTRASONIC_AVOIDANCE:
      if (mode != E_ULTRASONIC_AVOIDANCE) {
        mTank.InitUltrasonic();
        mTank.SetServoDegree(1, 90);
        mode = E_ULTRASONIC_AVOIDANCE;
      }
      HandleUltrasonicAvoidance();
      break;
    case E_ULTRASONIC_FOLLOW_MODE:
      if (mode != E_ULTRASONIC_FOLLOW_MODE) {
        mTank.InitUltrasonic();
        mTank.SetServoDegree(1, 90);
        mode = E_ULTRASONIC_FOLLOW_MODE;
      }
      UltrasonicFollow();
      break;
    case E_PIANO_MODE:
      if (recv_flag) {
        if (mProtocol->GetRobotControlFun() == E_BUZZER_MODE) {
          switch (mProtocol->GetBuzzerMode())
          {
            case E_NOTE:
              mTank.Sensors->mBuzzer->_tone(mProtocol->GetBuzzerNote(), 120, 2);
              break;
            case E_SOUND:
              mTank.sing(mProtocol->GetBuzzerSound());
              break;
            case E_MUSIC:
              mTank.PianoSing(mProtocol->GetBuzzerMusic());
              break;
          }
        }
      }
      break;
    case E_PS2_REMOTE_CONTROL:
      HandlePS2();
      break;
    case E_RGB_MODE:
      if (recv_flag) {
        if (mProtocol->GetRobotControlFun() == E_LED) {
          if (mProtocol->GetRgbMode() == 1) {
            mTank.SetRgbColor(E_RGB_ALL, mProtocol->GetRgbValue());
          } else {
            mTank.SetRgbEffect(E_RGB_ALL, mProtocol->GetRgbValue(), mProtocol->GetRgbEffect());
          }
        }
      }
      return;
    default:
      break;
  }
  switch (mTank.GetStatus()) {
    case E_FORWARD:
      mTank.SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT_ROTATE:
      mTank.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT_ROTATE:
      mTank.SetRgbColor(E_RGB_RIGHT, RGB_WHITE);
      break;
    case E_BACK:
      mTank.SetRgbColor(E_RGB_ALL, RGB_RED);
      break;
    case E_STOP:
      mTank.LightOff();
      break;
    default:
      break;
  }
}
