#include <AFMotor.h>
#define forward   255
#define turn      255

AF_DCMotor motor2(4, MOTOR34_1KHZ);
AF_DCMotor motor1(1, MOTOR12_1KHZ);

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
    motor1.setSpeed(forward);
    motor2.setSpeed(forward);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }
  else if (L && C && !R) { // 110
    motor1.setSpeed(turn);
    motor2.setSpeed(turn / 2);
    motor1.run(FORWARD);
    motor2.run(RELEASE);
  }
  else if (!L && C && R) { // 011
    motor1.setSpeed(turn / 2);
    motor2.setSpeed(turn);
    motor1.run(RELEASE);
    motor2.run(FORWARD);
  }
  else if (L && !C && !R) { // 100
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  }
  else if (!L && !C && R) { // 001
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
  }
  else if (!L && !C && !R) { // Lost Line, Use Last Turn Direction
    if (zzturn == 0) { // Last turn was left, turn right
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
    }
    else if (zzturn == 1) { // Last turn was right, turn left
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
    }
  }
  else if (L && C && R) // 111
  {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }
}
