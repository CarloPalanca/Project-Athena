#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motorLeft = AFMS.getMotor(3);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);

#define forward   255
#define turn      255*0.4 
#define lSensorA 5
#define cSensorA 6
#define rSensorA 7
#define lSensorB 11
#define cSensorB 12
#define rSensorB 13

void setup() {
  AFMS.begin();
  Serial.begin(9600);
  pinMode(lSensorA, INPUT);
  pinMode(cSensorA, INPUT);
  pinMode(rSensorA, INPUT);
  pinMode(lSensorB, INPUT);
  pinMode(cSensorB, INPUT);
  pinMode(rSensorB, INPUT);
  motorLeft->setSpeed(0);
  motorRight->setSpeed(0);
  delay(5000);
}

void loop() {
  int r = digitalRead(lSensorA); // far right
  int R = digitalRead(cSensorA); // right
  int CB = digitalRead(rSensorA); // center
  int CA = digitalRead(lSensorB); // center
  int C = CB | CA;
  int L = digitalRead(cSensorB); // left
  int l = digitalRead(rSensorB); // far left
  static int zzturn = 0; // Remember previous turn direction

  // Determine turn direction for zigzag handling
  if (!l && r) {
    zzturn = 1; // Last turn was right
  } else if (l && !r) {
    zzturn = 0; // Last turn was left
  }

  Serial.print("Sensors: ");
  Serial.print(l);
  Serial.print(L);
  Serial.print(C);
  Serial.print(R);
  Serial.println(r);

  // Line Following Logic
  if (!L && C && !R) { // 010
    motorLeft->setSpeed(forward);
    motorRight->setSpeed(forward);
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
  }
  else if (L && C && !R) { // 110
    motorLeft->setSpeed(turn);
    motorRight->setSpeed(turn);
    motorLeft->run(FORWARD);
    motorRight->run(BACKWARD);
  }
  else if (!L && C && R) { // 011
    motorLeft->setSpeed(turn);
    motorRight->setSpeed(turn);
    motorLeft->run(BACKWARD);
    motorRight->run(FORWARD);
  }
  else if (L && !C && !R) { // 100
    motorLeft->setSpeed(turn);
    motorRight->setSpeed(turn);
    motorLeft->run(FORWARD);
    motorRight->run(BACKWARD);
  }
  else if (!L && !C && R) { // 001
    motorLeft->setSpeed(turn);
    motorRight->setSpeed(turn);
    motorLeft->run(BACKWARD);
    motorRight->run(FORWARD);
  }
  else if (!L && !C && !R) { // 000
    if (zzturn == 0) { // Last turn was left, turn right
      motorLeft->setSpeed(turn);
      motorRight->setSpeed(turn + 0.15);
      motorLeft->run(FORWARD);
      motorRight->run(BACKWARD);
    }
    else if (zzturn == 1) { // Last turn was right, turn left
      motorLeft->setSpeed(turn + 0.15);
      motorRight->setSpeed(turn);
      motorLeft->run(BACKWARD);
      motorRight->run(FORWARD);
    }
  }
  else if ((L && C && R) || (l && L && C && R && r)) // 111
  {
    motorLeft->setSpeed(forward);
    motorRight->setSpeed(forward);
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
  }

  // Optionally add a small delay to avoid flickering
  delay(30);
}
