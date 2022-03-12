#include <Arduino.h>

#include "motor.h"
#include "network.h"

// ---------------------------------------------------------------
// SETUP
// ---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  } // debug only
  Serial.println("initializing...");
  motorSetup();

  Serial.println("network starting...");
  networkSetup();
}

// ---------------------------------------------------------------
// LOOP
// ---------------------------------------------------------------
void loop() {
  networkLoop();
}
