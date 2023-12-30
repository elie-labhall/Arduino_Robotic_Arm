#include <Wire.h>
#include "PCA9685.h"

PCA9685 driver;
int ang1 = -10, ang2 = -50, ang3 = 10, ang4 = 70;
int servo1 = 0, servo2 = 4, servo3 = 8, servo4 = 12;
float speed = 15;  //  higer is slower
int step2 = 0, step3 = 0;
int claw=5;

// PCA9685 outputs = 12-bit = 4096 steps
// 2.5% of 20ms = 0.5ms ; 12.5% of 20ms = 2.5ms
// 2.5% of 4096 = 102 steps; 12.5% of 4096 = 512 steps

PCA9685_ServoEval pwmServo(100, 512);  // (-90deg, +90deg)

//always set ang to new angle AFTER using move

void setup() {

  Serial.begin(115200);
  Wire.begin();                // Wire must be started first
  Wire.setClock(400000);       // Supported baud rates are 100kHz,400kHz, and 1000kHz
  driver.resetDevices();       // Software resets all PCA9685 devices on Wire line
  driver.init();               // Address pins A5-A0 set to B000000 init(B000000)
  driver.setPWMFrequency(50);  // Set frequency to 50Hz

  driver.setChannelPWM(servo1, pwmServo.pwmForAngle(ang1));
  driver.setChannelPWM(servo2, pwmServo.pwmForAngle(ang2));
  driver.setChannelPWM(servo3, pwmServo.pwmForAngle(ang3));
  driver.setChannelPWM(servo4, pwmServo.pwmForAngle(ang4));

  //   // orange grey
  //   // red white
}


void loop() {

  step2 = 30;  //31 31 for benchy in black spot
  step3 = 30;
  getToDouble(ang2 + step2, ang3 + step3);

  grab();

  step3 = 25;
  getTo3(ang3 + step3);
  
  getTo1(-50);

  step2 = 8;
  step3 = 20;
  getToDouble(ang2 + step2, ang3 + step3);

  release();

  step3 = 25;
  getTo3(ang3 + step3);
  
  getTo1(-10);  
  getToDouble(-50, 10);

  while(1){}
}




void grab() {
  for (int i = 70; i > claw; i--) {
    ang4 = i;
    move(servo4, ang4);
    delay(30);
  }
}

void release() {
  for (int i = claw; i < 70; i++) {
    ang4 = i;
    move(servo4, ang4);
    delay(20);
  }
}

void move(int servo, int angle) {
  driver.setChannelPWM(servo, pwmServo.pwmForAngle(angle));
}


void getTo2(int target2) {
  float step2 = (target2 - ang2) / speed;
  float angle2 = ang2;
  for (int i = 0; i < speed; i++) {
    Serial.println(ang2);
    angle2 += step2;
    ang2 = int(angle2);
    move(servo2, ang2);
    delay(100);
  }
}

void getTo3(int target3) {
  float step3 = (target3 - ang3) / speed;
  float angle3 = ang3;
  for (int i = 0; i < speed; i++) {
    Serial.println(ang3);
    angle3 += step3;
    ang3 = int(angle3);
    move(servo3, ang3);
    delay(50);
  }
}

void getTo1(int target1) {
  float step1 = (target1 - ang1) / speed;
  float angle1 = ang1;
  for (int i = 0; i < speed; i++) {
    Serial.println(ang1);
    angle1 += step1;
    ang1 = int(angle1);
    move(servo1, ang1);
    delay(50);
  }
}

void getToDouble(int target2, int target3) {

  float step2 = (target2 - float(ang2)) / speed;
  float step3 = (target3 - ang3) / speed;
  float angle2 = ang2, angle3 = ang3;

  for (int i = 0; i < speed; i++) {

    angle2 += step2;
    angle3 += step3;
    ang2 = int(angle2);
    ang3 = int(angle3);
    move(servo2, ang2);
    move(servo3, ang3);
    delay(200);
  }
}