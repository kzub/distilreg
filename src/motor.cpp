#include <Arduino.h>

#define MOTOR_PIN_A 4
#define MOTOR_PIN_B 5
#define MOTOR_PIN_C 12
#define MOTOR_PIN_D 13

// total 4100 step per 360 degree rotation
// 24 steps per 360 oven rotater. 15 degree per 1 power level change
// 4100/24 = 179,83 per power level
// 175 +- ok

long motorPosition = 0;

byte currentStep = 0;
byte steps[8][4] = {
  { HIGH, LOW,  LOW,  LOW },
  { HIGH, HIGH, LOW,  LOW },
  { LOW,  HIGH, LOW,  LOW },
  { LOW,  HIGH, HIGH, LOW },
  { LOW,  LOW,  HIGH, LOW },
  { LOW,  LOW,  HIGH, HIGH },
  { LOW,  LOW,  LOW,  HIGH },
  { HIGH, LOW,  LOW,  HIGH }
};

void motorSetup() {
  pinMode(MOTOR_PIN_A, OUTPUT);
  pinMode(MOTOR_PIN_B, OUTPUT);
  pinMode(MOTOR_PIN_C, OUTPUT);
  pinMode(MOTOR_PIN_D, OUTPUT);
}

void sequence() {
  digitalWrite(MOTOR_PIN_A, steps[currentStep][0]);
  digitalWrite(MOTOR_PIN_B, steps[currentStep][1]);
  digitalWrite(MOTOR_PIN_C, steps[currentStep][2]);
  digitalWrite(MOTOR_PIN_D, steps[currentStep][3]);
  delay(1);
}

void sequenceRaw(byte a, byte b, byte c, byte d) {
  digitalWrite(MOTOR_PIN_A, a);
  digitalWrite(MOTOR_PIN_B, b);
  digitalWrite(MOTOR_PIN_C, c);
  digitalWrite(MOTOR_PIN_D, d);
}

void stepChange(bool direction) {
  if (direction) {
    currentStep++;
    if (currentStep > 7) {
      currentStep = 0;
    }
  } else {
    if (currentStep == 0) {
      currentStep = 7;
    } else {
      currentStep--;
    }
  }
}

void motorBackwardStep() {
  stepChange(true);
  sequence();
  motorPosition--;
}

void motorForwardStep() {
  stepChange(false);
  sequence();
  motorPosition++;
}

void motorTurn(int steps) {
  int times = abs(steps);
  // Serial.printf("motorTurn steps: %d\n", steps);
  for (int i = 0; i < times; i++) {
    if (steps > 0) {
      motorForwardStep();
    } else {
      motorBackwardStep();
    }
  }
  sequenceRaw(LOW, LOW, LOW, LOW); // stop
}
