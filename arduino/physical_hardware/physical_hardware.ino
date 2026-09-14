/*
  Solar-Powered Washing Machine — Physical Hardware
  ====================================================
  Drives the real hardware build: a 28BYJ-48 stepper motor (drum) via
  a ULN2003 driver, a small DC motor (pump) switched through a
  transistor/relay, a valve LED, and two potentiometers simulating
  water level and available solar power. No AccelStepper/Stepper
  library is used — the half-step sequence is written out manually so
  every step of the motor control is explicit.

  - Drum motor  -> 28BYJ-48 stepper + ULN2003 driver, IN1-IN4 on pins 4-7
  - Pump        -> DC motor via transistor/relay on pin 9
  - Inlet valve -> LED (or solenoid) on pin 8
  - Start/pause -> push button on pin 10 (pull-down)
  - Water level -> potentiometer on A5
  - Solar power -> potentiometer or IR receiver on A1 (0-3 scale)

  See docs/circuit-physical.md for the full wiring guide, including
  the transistor/diode arrangement for the pump and the ULN2003
  connections for the stepper.
*/

// ==========================
// Pin Assignments
// ==========================
const int stepperPins[4] = {4, 5, 6, 7};    // Stepper IN1-IN4 (via ULN2003)
const int valvePin = 8;                     // Inlet valve (LED or solenoid)
const int pumpPin = 9;                      // Pump (DC motor via transistor/relay)
const int startButton = 10;                 // Start/pause button
const int waterSensor = A5;                 // Potentiometer (0-1023)
const int solarInput = A1;                  // Pot/IR to simulate solar levels (0-3)

// ==========================
// Global Variables
// ==========================
bool machineRunning = false;
int availablePower = 0;

// Stepper half-step sequence (8 steps) — smoother than full-step
const byte HALFSTEP[8][4] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {1,0,0,1}
};

// ==========================
// Setup
// ==========================
void setup() {
    Serial.begin(9600);

    for (int i = 0; i < 4; i++) pinMode(stepperPins[i], OUTPUT);
    pinMode(valvePin, OUTPUT);
    pinMode(pumpPin, OUTPUT);
    pinMode(startButton, INPUT);

    Serial.println("Washing Machine Ready. Press Start to Begin.");
}

// ==========================
// Stepper Motor Control (no library — manual half-step sequence)
// ==========================
void stepMotor(int idx) {
    for (int i = 0; i < 4; i++) digitalWrite(stepperPins[i], HALFSTEP[idx][i]);
}

void rotateCW(int steps, int speedMicros) {
    for (int s = 0; s < steps; s++) {
        for (int i = 0; i < 8; i++) {
            stepMotor(i);
            delayMicroseconds(speedMicros);
        }
    }
}

void rotateCCW(int steps, int speedMicros) {
    for (int s = 0; s < steps; s++) {
        for (int i = 7; i >= 0; i--) {
            stepMotor(i);
            delayMicroseconds(speedMicros);
        }
    }
}

// ==========================
// Update Power Level (0-3)
// ==========================
void updatePowerLevel() {
    int raw = analogRead(solarInput);
    availablePower = map(raw, 0, 1023, 0, 3); // scale to 0-3
}

// ==========================
// Check Power Availability
// ==========================
bool checkPower(int requiredLevel) {
    updatePowerLevel();

    Serial.print("Required Power: ");
    Serial.print(requiredLevel);
    Serial.print(" | Available Power: ");
    Serial.println(availablePower);

    while (availablePower < requiredLevel) {
        Serial.println("Not enough power. Waiting...");
        delay(500);
        updatePowerLevel();
    }

    Serial.println("Power sufficient, continuing...");
    return true;
}

// ==========================
// Fill Drum with Water
// ==========================
void fillDrum(String stage) {
    Serial.print("Filling drum before ");
    Serial.println(stage);

    digitalWrite(valvePin, HIGH);

    while (analogRead(waterSensor) < 818) { // ~80% full
        delay(100);
    }

    digitalWrite(valvePin, LOW);
    Serial.println("Water level reached.");
}

// ==========================
// Wash / Rinse Cycle
// ==========================
void wash_rinse(String stage) {
    Serial.print(stage);
    Serial.println(" cycle started...");

    for (int repeat = 0; repeat < 3; repeat++) {
        // Clockwise 3 turns
        for (int t = 0; t < 3; t++) {
            rotateCW(128, 2000);    // adjust steps/speed as needed
            delay(1000);
        }

        // Anticlockwise 3 turns
        for (int t = 0; t < 3; t++) {
            rotateCCW(128, 2000);
            delay(1000);
        }
    }

    Serial.print(stage);
    Serial.println(" cycle completed.");
}

// ==========================
// Draining Cycle
// ==========================
void draining() {
    Serial.println("Draining drum...");
    digitalWrite(pumpPin, HIGH);
    delay(3000);
    digitalWrite(pumpPin, LOW);
    Serial.println("Drum drained.");
}

// ==========================
// Spinning Cycle
// ==========================
void spinning() {
    Serial.println("Spinning cycle started...");

    for (int turns = 0; turns < 20; turns++) {
        rotateCW(64, 1000); // faster than wash
    }

    Serial.println("Finished! Clothes are clean.");
}

// ==========================
// Main Loop
// ==========================
void loop() {
    if (digitalRead(startButton) == HIGH) {
        delay(200); // debounce
        machineRunning = !machineRunning;

        if (machineRunning) {
            Serial.println("Washing cycle started");

            checkPower(1);
            fillDrum("Washing");

            checkPower(2);
            wash_rinse("Washing");

            checkPower(1);
            draining();

            checkPower(1);
            fillDrum("Rinsing");

            checkPower(2);
            wash_rinse("Rinsing");

            checkPower(1);
            draining();

            checkPower(3);
            spinning();

            machineRunning = false;
            Serial.println("Ready for next load.");
        }
    }
}
