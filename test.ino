#include <AFMotor.h>
#define forward   255
#define turn      255

AF_DCMotor motor2(3, MOTOR34_1KHZ);
AF_DCMotor motor1(2, MOTOR12_1KHZ);

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop() {
  int r = digitalRead(A0); // far right
  int R = digitalRead(A1); // right
  int CB = digitalRead(A2); // center
  int CA = digitalRead(A3); // center
  int C = CB | CA;
  int L = digitalRead(A4); // left
  int l = digitalRead(A5); // far left
  static int zzturn = 0; // Remember previous turn direction

  motor1.setSpeed(forward);
  motor2.setSpeed(forward);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}
