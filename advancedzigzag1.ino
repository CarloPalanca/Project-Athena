#include <AFMotor.h>
#define forward   255
#define turn      255

AF_DCMotor motor2(4, MOTOR34_1KHZ );
AF_DCMotor motor1(1, MOTOR12_1KHZ );

int B = digitalRead(A0); // bumper
int r = digitalRead(A1); // far right
int R = digitalRead(A2); // right
int C = digitalRead(A3); // center
int L = digitalRead(A4); // left
int l = digitalRead(A5); // far left
int zzturn;

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

  if (l && !r) //0 1
  {
    zzturn = 1;
  }
  else if (!l && r) //1 0
  {
    zzturn = 0;
  }
  if (L && !C && R) // 010
  {
    motor1.setSpeed(forward);
    motor2.setSpeed(forward);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }
  else if (!L && !C && R) //110
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(FORWARD);
    motor2.run(RELEASE);
  }
  else if (L && !C && !R) //011
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(RELEASE);
    motor2.run(FORWARD);
  }
  else if (!L && C && R) //100
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  }
  else if (L && C && !R) //001
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
  }
  else if (L && C && R) //000
  {
    if (zzturn = 1) // turn right
    {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
    }
    else // turn left
    {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
    }
  }
}
