#include <Wire.h> // Pins A4(SDA) and A5(SCL)
#include <TimerOne.h>
#include "TrackingCamI2C.h"
#include "GyverMotor.h"

GMotor right_eng(DRIVER2WIRE, 2, 3, HIGH);// Direction + Signal
GMotor left_eng(DRIVER2WIRE,11, 10, HIGH);// Direction + Signal

TrackingCamI2C trackingCam;
unsigned long previousMillis = 0; // Stores last time cam was updated

#define Line_blob_type 0
#define Block_blob_type 1

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C

#define GYRO_FULL_SCALE_250_DPS 0x00
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2_G 0x00
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

int setg = 0;
int setl = 640 / 2; // Senter of camera picture
int dt = 50;

// Pid coefficients for Gyro
int kpg = 1;
int kig = 0;
int kdg = 0;

// Pid coefficients for line
int kpl = 1;
int kil = 0;
int kdl = 0;

int right_eng_val = 0;
int left_eng_val  = 0;

max_ax = 1;
min_ax = 0;

// Initial time
long long ti;
volatile bool intFlag = false;

// Initializations
void setup() {
  // Arduino initializations
  left_eng.setMode(AUTO);
  right_eng.setMode(AUTO);
  pinMode(13,INPUT);
  left_eng.setSmoothSpeed(10);//  https://alexgyver.ru/gyvermotor/
  right_eng.setSmoothSpeed(10);// https://alexgyver.ru/gyvermotor/

  Wire.begin();
  Serial.begin(115200);
  trackingCam.init(51, 400000);// Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS, 29, 0x06); // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS, 26, 0x06); // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS); // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_4_G); // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02); // Request continuous magnetometer measurements in 16 bits
  I2CwriteByte(MAG_ADDRESS, 0x0A, 0x16);
  pinMode(13, OUTPUT);
  Timer1.initialize(10000); // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback); // attaches callback() as a timer overflow interrupt
  // Store initial time
  ti = millis();
}
// Waiting for a button
bool f = false;             
butt = digitalRead(13);
while butt!=1{
  butt = digitalRead(13);
}
f = true;

if (f){ 
  void loop() {
    line_type = cam(0);// Get line type from camera

    if (line_type == "strait") {
      int gz = Gyro_out(1);// Get angle from the gyro
      int az = Gyro_out(2);

      /*
      int aссel = cos(gz)* az;
      */
      if (az >= max_ax) { // If we are to fast -> decrease ang
        set -= 0.5;
      }

      if (az <= min_ax) { // If we are to slow -> increase ang
        set += 0.5;
      }

      int out = PID(setg, gz, kpg, kig, kdg, dt); // Get output for eng
      int eng_out = constrain(out, 0, 510) ;
      eng_out = map(eng_out, 0, 510, -255, 255); //Fetch output to -256 +256
      right_eng_val = eng_out;
      left_eng_val = eng_out;

    }
    else {
      line_type = cam(0);// Get line type from camera

      int gz = Gyro_out(1);// Get angle from the gyro
      int az = Gyro_out(2);//Get acceleration from the accel

      /*
      int accel = cos(gz)* az;
      */
      if (az >= max_ax) { // If we are to fast -> decrease ang
        set -= 0.5;
      }
      if (az <= min_ax) { // If we are to slow -> increase ang
        set += 0.5;
      }
      int out = PID(setg, gz, kpg, kig, kdg, dt); // Get output for eng
      int eng_out = constrain(out, 0, 410) ;
      eng_out = map(eng_out, 0, 410, -205, 205); //Fetch output to -256 +256

      if (line_type = "right") {
        right_eng_val = eng_out;
        int val = PID(setl, cam(1), kpl, kil, kdl, dt);
        val = constrain(val, -50, 50);
        left_eng_val = eng_out + val;
      } 
      
      if (line_type = "left") {
        left_eng_val = eng_out;
        int val = PID(setl, cam(1), kpl, kil, kdl, dt);
        val = constrain(val, -50, 50);
        right_eng_val = eng_out + val;
      } 
      
      if (line_type = "brick") {
        int t = millis();
        if (millis - t <= 1000) {
          left_eng_val = eng_out;
          int val = PID(setl, cam(1), kpl, kil, kdl, dt);
          val = constrain(val, -50, 50);
          right_eng_val = eng_out + val;
          left_eng.smoothTick(left_eng_val);
          right_eng.smoothTick(right_eng_val);
        }
        t = millis();
        if (millis - t <= 1000) {
          right_eng_val = eng_out;
          int val = PID(setl, cam(1), kpl, kil, kdl, dt);
          val = constrain(val, -50, 50);
          left_eng_val = eng_out + val;
          left_eng.smoothTick(left_eng_val);
      right_eng.smoothTick(right_eng_val);
        }
      }
      line_type = "0" ;
    }
    left_eng.smoothTick(left_eng_val);
    right_eng.smoothTick(right_eng_val);
  }
}