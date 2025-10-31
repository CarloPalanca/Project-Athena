#include <AFMotor.h>

#define forward 255
#define turn    255

AF_DCMotor motor1(2, MOTOR12_1KHZ);
AF_DCMotor motor2(3, MOTOR34_1KHZ);

unsigned long previousMillis = 0;
const unsigned long phase1Duration = 9000;   // 9 seconds
const unsigned long phase2Duration = 14000;  // 14 seconds

int currentPhase = 1;
int lastPhase = -1;
int zzturn = 0; // For Phase 1 zigzag memory

void setup() {
  pinMode(A0, INPUT); // far right
  pinMode(A1, INPUT); // right
  pinMode(A2, INPUT); // center back
  pinMode(A3, INPUT); // center front
  pinMode(A4, INPUT); // left
  pinMode(A5, INPUT); // far left

  Serial.begin(9600);
  previousMillis = millis(); // Start timer
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long elapsed = (currentMillis - previousMillis) % (phase1Duration + phase2Duration);

  // Determine phase
  if (elapsed < phase1Duration) {
    currentPhase = 1;
  } else {
    currentPhase = 2;
  }

  // Smooth transition between phases
  if (currentPhase != lastPhase) {
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(100); // Let robot settle briefly
    lastPhase = currentPhase;
    Serial.print("Switched to Phase ");
    Serial.println(currentPhase);
  }

  // Read sensors
  int r  = digitalRead(A0); // far right
  int R  = digitalRead(A1); // right
  int CB = digitalRead(A2); // center back
  int CA = digitalRead(A3); // center front
  int C  = CB | CA;         // combined center
  int L  = digitalRead(A4); // left
  int l  = digitalRead(A5); // far left

  // === PHASE 1 LOGIC ===
  if (currentPhase == 1) {
    if (!l && r) zzturn = 1;
    else if (l && !r) zzturn = 0;

    Serial.print("Sensors: ");
    Serial.print(l); Serial.print(L); Serial.print(C); Serial.print(R); Serial.println(r);

    if (!L && C && !R) {
      motor1.setSpeed(forward);
      motor2.setSpeed(forward);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    } else if (L && C && !R) {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn / 2);
      motor1.run(FORWARD);
      motor2.run(RELEASE);
    } else if (!L && C && R) {
      motor1.setSpeed(turn / 2);
      motor2.setSpeed(turn);
      motor1.run(RELEASE);
      motor2.run(FORWARD);
    } else if (L && !C && !R) {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
    } else if (!L && !C && R) {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
    } else if (!L && !C && !R) {
      if (zzturn == 0) {
        motor1.setSpeed(turn);
        motor2.setSpeed(turn);
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
      } else {
        motor1.setSpeed(turn);
        motor2.setSpeed(turn);
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
      }
    } else if (L && C && R) {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    }
  }

  // === PHASE 2 LOGIC ===
  else if (currentPhase == 2) {
    if (
      (!L && C && !R) ||
      (L && C && R) ||
      (!L && !C && !R) ||
      (!l && !L && C && !R && !r) ||
      (l && L && C && R && r)
    ) {
      motor1.setSpeed(forward);
      motor2.setSpeed(forward);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    } else if (!l && !L && !C && !R && !r) {
      motor1.setSpeed(turn);
      motor2.setSpeed(turn);
      motor1.run(FORWARD);
      motor2.run(FORWARD);
    } else if (
      (L && C && !R) ||
      (L && !C && !R) ||
      (!l && L && !C && !R && !r) ||
      (l && L && !C && !R && !r) ||
      (l && !L && !C && !R && !r)
    ) {
      motor1.setSpeed(forward);
      motor2.setSpeed(forward);
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
    } else if (
      (!L && C && R) ||
      (!L && !C && R) ||
      (!l && !L && !C && R && !r) ||
      (!l && !L && !C && R && r) ||
      (!l && !L && !C && !R && r)
    ) {
      motor1.setSpeed(forward);
      motor2.setSpeed(forward);
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
    }
  }

  // Optional: small delay to reduce jitter
  // delay(10);
}
