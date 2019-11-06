#include "FourWheelDrive.h"
#include "ProtocolParser.h"
#include "BluetoothHandle.h"
#include "debug.h"
#include "KeyMap.h"
ProtocolParser *mProtocol = new ProtocolParser();
FourWheelDrive mARM(mProtocol);
void setup()
{
  Serial.begin(9600);
  mARM.init(M2, M1, M4, M3);
  mARM.SetControlMode(E_PS2_REMOTE_CONTROL);
  mARM.InitServo();
  mARM.InitRgb();
  mARM.InitBuzzer();
  mARM.SetSpeed(100);
  mARM.SetServoBaseDegree(90);
  mARM.SetServoDegree(1, 90);
  mARM.InitPs2x();
  mARM.sing(S_connection);
}

void HandlePS2(void)
{
  static byte UpServo = 180;
  static byte DownServo = 90;
  static byte LeftServo = 30;
  static byte RightServo = 120;
  int Ps2xRightAngle, Ps2xLeftAngle;
  mARM.Ps2x->read_gamepad(false, 0);
  //Serial.print("RightServo===");
 //  Serial.print(RightServo);
 //   Serial.print("       LeftServo============");
 //    Serial.println(LeftServo);
  if (mARM.Ps2x->Button(PSB_L1)) {//按下右侧 1号按键，合上夹子
    if (UpServo < 165)
      UpServo += 4;
  }
  if (mARM.Ps2x->Button(PSB_R1)) {//按下左侧 1号按键,打开夹子
    if (UpServo > 80)
      UpServo -= 4;
  } else {
    mARM.KeepStop();
    Ps2xRightAngle = mARM.Ps2x->RightHart(); //获取右侧摇杆的值
    Ps2xLeftAngle = mARM.Ps2x->LeftHart();  //获取左侧摇杆的值

    if ((Ps2xRightAngle != 0xFFF) || (Ps2xLeftAngle != 0xFFF)) {  //检测到摇杆被操作
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
        if ((RightServo < 95) && ( 10 < LeftServo))
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
         if ((RightServo < 95) && (70 > LeftServo))
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
        if ((RightServo < 95) && ( 70 > LeftServo))
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
     //   if ((RightServo > 95) && ( 10 < LeftServo))
     //  {
     //      LeftServo -= 2;
     //   }
     //   else if ((RightServo > 50) && ( 50 < LeftServo))
     //    {
     //      RightServo -= 2;
     //    }
     if ((RightServo > 95) && ( 10 < LeftServo))
        {
         
          RightServo -= 2;
        }
        else if ( 15 < LeftServo) 
        {
           LeftServo -= 2;
        }
        
      }
      mARM.SetServoDegree(1, DownServo);
      mARM.SetServoDegree(2, LeftServo);
      mARM.SetServoDegree(3, RightServo);
    }
  }
  mARM.SetServoDegree(4, UpServo);
  delay(50);
}

void loop()
{
  if (mARM.GetControlMode() == E_PS2_REMOTE_CONTROL) {
    HandlePS2();
    DEBUG_LOG(DEBUG_LEVEL_INFO, "E_BLUETOOTH_CONTROL \n");
  }
}
