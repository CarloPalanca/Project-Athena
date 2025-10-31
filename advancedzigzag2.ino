#include <AFMotor.h>
#define forward   255
#define turn      255

AF_DCMotor motor2(4, MOTOR34_1KHZ );
AF_DCMotor motor1(1, MOTOR12_1KHZ );

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
}

void loop()
{
  int r = digitalRead(A0); // far right
  int R = digitalRead(A1); // right
  int CB = digitalRead(A2); // center
  int CA = digitalRead(A3); // center
  int C = CB | CA;
  int L = digitalRead(A4); // left
  int l = digitalRead(A5); // far left
  int zzturn;
  //Serial.print(r);
  //Serial.print(R);
  //Serial.print(C);
  //Serial.print(L);
  //Serial.println(l);
  Serial.println(zzturn);

  if (!l && r) //0 1
  {
    zzturn = 1;
  }
  else if (l && !r) //1 0
  {
    zzturn = 0;
  }

  if (!L && C && !R) // 010
  {
    motor1.setSpeed(forward);
    motor2.setSpeed(forward);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }
  else if (L && !C && !R) //110
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(FORWARD);
    motor2.run(RELEASE);
  }
  else if (!L && C && R) //011
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(RELEASE);
    motor2.run(FORWARD);
  }
  else if (L && !C && !R) //100
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  }
  else if (!L && !C && R) //001
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
  }
  else if (!L && !C && !R) //000
  {
    if (zzturn = 0) // turn right
    {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
    }
    else if (zzturn = 1) // turn left
    {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
    }
  }
}
