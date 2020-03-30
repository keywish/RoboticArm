#include "FourWheelDrive.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "debug.h"
#include "KeyMap.h"
ProtocolParser *mProtocol = new ProtocolParser();
FourWheelDrive m4wd(mProtocol);

void setup()
{
  Serial.begin(9600);
  Serial.println("PS2X control 4wd");
  m4wd.init(M2, M1, M4, M3);
  Serial.println("end");
  m4wd.SetControlMode(E_PS2_REMOTE_CONTROL);
  m4wd.InitServo();
  m4wd.InitRgb();
  m4wd.InitBuzzer();
  m4wd.SetSpeed(60);
  m4wd.SetServoBaseDegree(90);
  m4wd.SetServoDegree(1, 90);
  m4wd.InitPs2x();
  Serial.println("init ok");
  m4wd.sing(S_connection);
}

void HandlePS2(void)
{
  static byte UpServo = 100;
  static byte DownServo = 90;
  static byte LeftServo = 30;
  static byte RightServo = 120;
  int Ps2xRightAngle, Ps2xLeftAngle;
  m4wd.Ps2x->read_gamepad(false, 0);

  if (m4wd.Ps2x->ButtonDataByte()) {
    if (m4wd.Ps2x->Button(PSB_TRIANGLE)) {
      Serial.print("PS2X PSB_TRIANGLE:");
      m4wd.SpeedUp(5);
      Serial.println(m4wd.GetSpeed());
      return;
    }
    if (m4wd.Ps2x->Button(PSB_CROSS)) {
      Serial.print("PS2X PSB_CROSS:");
      m4wd.SpeedDown(5);
      Serial.println(m4wd.GetSpeed());
      return;
    }

    if (m4wd.Ps2x->Button(PSB_PAD_UP)) {
      Serial.println("PS2X PSB_PAD_UP:");
      m4wd.GoForward();
    }
    if (m4wd.Ps2x->Button(PSB_PAD_DOWN)) {
      Serial.println("PS2X PSB_PAD_DOWN:");
      m4wd.GoBack();
    }
    if (m4wd.Ps2x->Button(PSB_PAD_LEFT)) {
      Serial.println("PS2X PSB_PAD_LEFT:");
      m4wd.TurnLeft();
    }
    if (m4wd.Ps2x->Button(PSB_PAD_RIGHT)) {
      Serial.println("PS2X PSB_PAD_RIGHT:");
      m4wd.TurnRight();
    }
    if (m4wd.Ps2x->Button(PSB_SQUARE)) {
      Serial.println("PS2X PSB_SQUARE:");
      m4wd.TurnLeftRotate();
    }
    if (m4wd.Ps2x->Button(PSB_CIRCLE)) {
      Serial.println("PS2X PSB_CIRCLE:");
      m4wd.TurnRightRotate();
    }
    if (m4wd.Ps2x->Button(PSB_L1))
    {
      if (UpServo < 165)
        UpServo += 4;
    }
    if (m4wd.Ps2x->Button(PSB_R1))
    {
      if (UpServo > 80)
      {
        UpServo -= 4;
      }
    }

  } else {
    m4wd.KeepStop();
    Ps2xRightAngle = m4wd.Ps2x->RightHart(); //获取右侧摇杆的值
    Ps2xLeftAngle = m4wd.Ps2x->LeftHart();  //获取左侧摇杆的值
    if ((Ps2xRightAngle != 0xFFF) || (Ps2xLeftAngle != 0xFFF))  //检测到摇杆被操作
    {
      if ((90 < Ps2xLeftAngle) && (Ps2xLeftAngle <= 270)) //左侧摇杆左摇,机械臂向左偏转
      {
        if (DownServo < 180)
        {
          DownServo += 2;
        }
      }
      else if ((270 < Ps2xLeftAngle) && (Ps2xLeftAngle <= 360) || (0 <= Ps2xLeftAngle) && (Ps2xLeftAngle <= 90)) //左侧摇杆右摇，机械臂向右偏转
      {
        if (DownServo > 20)
        {
          DownServo -= 2;
        }
      }
      else if ((45 <= Ps2xRightAngle) && (Ps2xRightAngle < 135))   //右侧摇杆向前推,夹子落下
      {
        if ((RightServo < 95) && ( 20 < LeftServo))
        {
          LeftServo -= 2;
        }
       else if ((RightServo >120) && ( 50 < LeftServo))
        {
          LeftServo -= 2;
        }
        else if ((RightServo < 140))
        {
          RightServo += 2;
        }
      }
      else if ((225 <= Ps2xRightAngle) && (Ps2xRightAngle < 315)) //右侧摇杆向后推，夹子抬起
      {
        if ((RightServo < 95) && (90 > LeftServo))
        {
          LeftServo += 2;

        }
        else if ((RightServo > 93))
        {
          RightServo -= 2;
        }
      }
      else if ((135 <= Ps2xRightAngle) && (Ps2xRightAngle < 225)) //右侧摇杆向左推,夹子前伸
      {
        if ((RightServo > 95) && ( 70 > LeftServo))
        {
          LeftServo += 2;
        }
        else if ((RightServo < 130))
        {
          RightServo += 2;
        }
      }
      else if ((315 <= Ps2xRightAngle) && (Ps2xRightAngle <= 360) || (0 <= Ps2xRightAngle) && (Ps2xRightAngle < 45)) //右侧摇杆向右推，夹子后缩
      {
        
         if ((RightServo > 90) && ( 30 < LeftServo))
        {
           LeftServo -= 2;  
        }
        else if (45 <RightServo)
        {
          RightServo -= 2;
        }
      }
      m4wd.SetServoDegree(1, DownServo);
      m4wd.SetServoDegree(2, LeftServo);
      m4wd.SetServoDegree(3, RightServo);
    }
  }
  m4wd.SetServoDegree(4, UpServo);
  delay(50);
}


void loop()
{
  if (m4wd.GetControlMode() == E_PS2_REMOTE_CONTROL) {
    HandlePS2();
    DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUETOOTH_CONTROL \n");
  }

  switch (m4wd.GetStatus()) {
    case E_FORWARD:
      m4wd.SetRgbColor(E_RGB_ALL, RGB_WHITE);
      break;
    case E_LEFT:
    case E_LEFT_ROTATE:
      m4wd.SetRgbColor(E_RGB_LEFT, RGB_WHITE);
      break;
    case E_RIGHT:
    case E_RIGHT_ROTATE:
      m4wd.SetRgbColor(E_RGB_RIGHT, RGB_WHITE);
      break;
    case E_BACK:
      m4wd.SetRgbColor(E_RGB_ALL, RGB_RED);
      break;
    case E_SPEED_UP:
      m4wd.sing(S_connection);
      m4wd.SetRgbColor(E_RGB_ALL, RGB_GREEN);
      break;
    case E_SPEED_DOWN:
      m4wd.sing(S_disconnection);
      m4wd.SetRgbColor(E_RGB_ALL, RGB_BLUE);
      break;
    case E_STOP:
      m4wd.LightOff();
      break;
    default:
      break;
  }
}
