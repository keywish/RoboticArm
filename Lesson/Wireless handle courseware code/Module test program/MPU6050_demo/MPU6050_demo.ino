// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

struct RAW_type{
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

int16_t ax, ay, az;
int16_t gx, gy, gz;
struct RAW_type accel_self_enable ,accel_self_diable;
struct RAW_type accel_zero_offsent ,gyro_zero_offsent;

#define LED_PIN 13   //NANO LED PIN
bool blinkState = false;

const float pi = 3.1415926;
const float Rad = 57.3 ;  //180.0/pi;
int w = 0;
float tmpf = 0.0;
int currentTime, signRzGyro;
int lastTime = 0;
int interval = 0;
float wGyro = 10.0;

float RwAcc[3];         // ACC raw data
float Gyro[3];          // 
float RwGyro[3];        // Gyro raw dat
float Awz[2];           // 
float RwEst[3];

float AcceRatio = 16384.0;
float GyroRatio = 131.0*57.3;

float accx, accy, accz;
float gyrox, gyroy, gyroz;

float roll, pitch, yaw = 0;

void normalize3DVec(float vector[3]) {
    float R;
    R = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
    vector[0] /= R;
    vector[1] /= R;  
    vector[2] /= R;
}

float squared(float x) {
    return x*x;
}

void readSensors(void)
{
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    RwAcc[0] =(ax-accel_zero_offsent.x)/AcceRatio;
    RwAcc[1] =(ay-accel_zero_offsent.y)/AcceRatio;
    RwAcc[2] =(az-accel_zero_offsent.z)/AcceRatio;
    Gyro[0]=(gx-gyro_zero_offsent.x)/GyroRatio;
    Gyro[1]=(gy-gyro_zero_offsent.y)/GyroRatio;
    Gyro[2]=(gz-gyro_zero_offsent.z)/GyroRatio;
}

void getInclination() {
  static boolean firstSample = true;
  readSensors();
  normalize3DVec(RwAcc);
 
  currentTime = millis();
  interval = currentTime - lastTime;
  lastTime = currentTime;
 
  if (firstSample /*|| Float.isNaN(RwEst[0])*/) {
    for (w=0;w<=2;w++) {
      RwEst[w] = RwAcc[w];
    }
  } else {
    if (abs(RwEst[2]) < 0.1) {
      for (w=0;w<=2;w++) {
        RwGyro[w] = RwEst[w];
      }
    }
    else {
      for (w=0;w<=1;w++) {
        tmpf = Gyro[w];
        tmpf *= interval / 1000.0f;
        Awz[w] = atan2(RwEst[w], RwEst[2]) * 180 / PI;
        Awz[w] += tmpf;
      }
      signRzGyro = ( cos(Awz[0] * PI / 180) >=0 ) ? 1 : -1;
 
      // http://starlino.com/imu_guide.html
      for (w=0;w<=1;w++) {
        RwGyro[0] = sin(Awz[0] * PI / 180);
        RwGyro[0] /= sqrt( 1 + squared(cos(Awz[0] * PI / 180)) * squared(tan(Awz[1] * PI / 180)) );
        RwGyro[1] = sin(Awz[1] * PI / 180);
        RwGyro[1] /= sqrt( 1 + squared(cos(Awz[1] * PI / 180)) * squared(tan(Awz[0] * PI / 180)) );
      }
      RwGyro[2] = signRzGyro * sqrt(1 - squared(RwGyro[0]) - squared(RwGyro[1]));
    }
    for (w=0;w<=2;w++) RwEst[w] = (RwAcc[w] + wGyro * RwGyro[w]) / (1 + wGyro);
 
    normalize3DVec(RwEst);
  }
  firstSample = false;
  roll = RwEst[1];
  pitch = RwEst[0];
}

void setup()
{
    int i ;
    int32_t ax_zero = 0,ay_zero = 0,az_zero = 0,gx_zero =0 ,gy_zero = 0,gz_zero = 0 ;
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
    Serial.begin(9600);
    delay(2000);
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    delay(500) ;
    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
    // enable Accel Self Test
    accelgyro.setAccelXSelfTest(1); // enable accel self test
    accelgyro.setAccelYSelfTest(1);
    accelgyro.setAccelZSelfTest(1);
    delay(500);
    accel_self_enable.x = accelgyro.getAccelXSelfTestDate();
    accel_self_enable.y = accelgyro.getAccelYSelfTestDate();
    accel_self_enable.z = accelgyro.getAccelZSelfTestDate();

    accelgyro.setAccelXSelfTest(0); // disable accel self test
    accelgyro.setAccelYSelfTest(0);
    accelgyro.setAccelZSelfTest(0);
    accel_self_diable.x = accelgyro.getAccelXSelfTestDate();
    accel_self_diable.y = accelgyro.getAccelYSelfTestDate();
    accel_self_diable.z = accelgyro.getAccelZSelfTestDate();

    accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
   //  verify connection
     Serial.println("Testing device connections...");
     Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    /* get zero accel/gyro value */
    for( i = 0 ; i < 200 ; i++){
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); //get raw accel/gyro measurements 
        ax_zero += az ;
        ay_zero += ay ;
        az_zero += az ;
        gx_zero += gx ;
        gy_zero += gy ;
        gz_zero += gz ;
    }
    accel_zero_offsent.x = ax_zero/200;
    accel_zero_offsent.y = ay_zero/200;
    accel_zero_offsent.z = az_zero/200;
    gyro_zero_offsent.x = gx_zero/200;
    gyro_zero_offsent.y = gy_zero/200;
    gyro_zero_offsent.z = gz_zero/200;
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // get inclination measurements from device
    getInclination();
    Serial.print(pitch);Serial.print(",");
    Serial.print(roll);Serial.print(",");
    Serial.print(RwEst[2]); Serial.print("\n");
}
