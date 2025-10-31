#include <AFMotor.h>
#define forward   255
#define turn      255

AF_DCMotor motor2(4, MOTOR34_1KHZ );
AF_DCMotor motor1(1, MOTOR12_1KHZ );

void setup()
{
  pinMode(A0, INPUT); // far right
  pinMode(A1, INPUT); // right
  pinMode(A2, INPUT); // center back
  pinMode(A3, INPUT); // center front
  pinMode(A4, INPUT); // left
  pinMode(A5, INPUT); // far left

  // Serial.begin(9600); // Uncomment for debugging
}

void loop()
{
  int r = digitalRead(A0);   // far right
  int R = digitalRead(A1);   // right
  int CB = digitalRead(A2);  // center back
  int CA = digitalRead(A3);  // center front
  int C = CB | CA;           // combined center reading
  int L = digitalRead(A4);   // left
  int l = digitalRead(A5);   // far left

  // Uncomment to debug sensor readings
  /*
    Serial.print(l); Serial.print(" ");
    Serial.print(L); Serial.print(" ");
    Serial.print(C); Serial.print(" ");
    Serial.print(R); Serial.print(" ");
    Serial.println(r);
  */

  // ------------------- FORWARD CONDITIONS -------------------
  if (
    (!L && C && !R) ||             // 010
    (L && C && R) ||               // 111
    (!L && !C && !R) ||            // 000
    (!l && !L && C && !R && !r) || // 00100
    (l && L && C && R && r)        // 11111
  ) {
    motor1.setSpeed(forward);
    motor2.setSpeed(forward);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }

  // Special case: no line detected at all (00000)
  else if (!l && !L && !C && !R && !r) {
    motor1.setSpeed(turn);
    motor2.setSpeed(turn);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }

  // ------------------- LEFT TURN CONDITIONS -------------------
  else if (
    (L && C && !R) ||              // 110
    (L && !C && !R) ||             // 100
    (!l && L && !C && !R && !r) || // 01000
    (l && L && !C && !R && !r) ||  // 11000
    (l && !L && !C && !R && !r)    // 10000
  ) {
    motor1.setSpeed(forward);
    motor2.setSpeed(forward);
    motor1.run(FORWARD);
    motor2.run(BACKWARD); 
  }

  // ------------------- RIGHT TURN CONDITIONS -------------------
  else if (
    (!L && C && R) ||              // 011
    (!L && !C && R) ||             // 001
    (!l && !L && !C && R && !r) || // 00010
    (!l && !L && !C && R && r) ||  // 00011
    (!l && !L && !C && !R && r)    // 00001
  ) {
    motor1.setSpeed(forward);
    motor2.setSpeed(forward);
    motor1.run(BACKWARD); 
    motor2.run(FORWARD);
  }

  // Optionally add a small delay to avoid flickering
  // delay(10);
}
