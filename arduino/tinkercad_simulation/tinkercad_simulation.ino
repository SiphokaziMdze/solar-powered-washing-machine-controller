/*
  Solar-Powered Washing Machine — Tinkercad Simulation
  =====================================================
  LED-based simulation of the washing machine controller. The drum
  motor, inlet valve, and pump are represented by LEDs so the full
  wash/rinse/drain/spin logic can be designed and tested in Tinkercad
  before building the physical circuit.

  - Drum motor  -> 4 LEDs on pins 4-7
  - Inlet valve -> blue LED on pin 8
  - Pump        -> red LED on pin 9
  - Start/pause -> push button on pin 10 (pull-down)
  - Water level -> potentiometer on A5 (simulates the water sensor)
  - Solar power -> typed into the Serial Monitor (0-3), simulating the
                   IR-received power level from the solar system

  Type a number 0-3 into the Serial Monitor to set the available power
  level at any time; each wash stage checks against a required level
  and pauses until enough "power" is available.
*/

// ==========================
// Pin Assignments
// ==========================
const int drumPins[4] = {4, 5, 6, 7};       // Drum motor LEDs
const int valveLED = 8;                     // Water inlet valve
const int pumpPin = 9;                      // Pump LED / motor
const int startButton = 10;                 // Start/pause button
const int waterSensor = A5;                 // Potentiometer for water level

// ==========================
// Global Variables
// ==========================
bool machineRunning = false;
int availablePower = 0;     // Set manually via Serial Monitor (0-3)

// ==========================
// Setup
// ==========================
void setup() {
    Serial.begin(9600);

    for (int i = 0; i < 4; i++) {
        pinMode(drumPins[i], OUTPUT);
    }

    pinMode(valveLED, OUTPUT);
    pinMode(pumpPin, OUTPUT);
    pinMode(startButton, INPUT);

    Serial.println("Washing Machine Ready. Press Start to Begin.");
    Serial.println("Type a number (0-3) in Serial Monitor to set available power.");
}

// ==========================
// Check Power Availability
// ==========================
bool checkPower(int requiredLevel) {
    Serial.print("Required Power: ");
    Serial.print(requiredLevel);
    Serial.print(" | Available Power: ");
    Serial.println(availablePower);

    while (availablePower < requiredLevel) {
        Serial.println("Not enough power. Waiting...");
        delay(500);

        // Check if a new value was entered in the Serial Monitor
        if (Serial.available() > 0) {
            availablePower = Serial.parseInt();
            Serial.print("New power level set: ");
            Serial.println(availablePower);
        }
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

    digitalWrite(valveLED, HIGH);

    // Wait until water level > ~80% (818 / 1023)
    while (analogRead(waterSensor) < 818) {
        delay(100);
    }

    digitalWrite(valveLED, LOW);
    Serial.println("Water level reached (80%).");
}

// ==========================
// Wash / Rinse Cycle
// ==========================
void wash_rinse(String stage) {
    Serial.print(stage);
    Serial.println(" cycle started...");

    for (int repeat = 0; repeat < 3; repeat++) {
        // Clockwise
        for (int t = 0; t < 3; t++) {
            for (int i = 0; i < 4; i++) {
                digitalWrite(drumPins[i], HIGH);
                delay(200);
                digitalWrite(drumPins[i], LOW);
            }
            delay(500);
        }
        delay(1000);

        // Anticlockwise
        for (int t = 0; t < 3; t++) {
            for (int i = 3; i >= 0; i--) {
                digitalWrite(drumPins[i], HIGH);
                delay(200);
                digitalWrite(drumPins[i], LOW);
            }
            delay(500);
        }
        delay(1000);
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
    Serial.println("Drum drained (Water = 0%).");
}

// ==========================
// Spinning Cycle
// ==========================
void spinning() {
    Serial.println("Spinning cycle started...");

    for (int turns = 0; turns < 20; turns++) {
        for (int i = 0; i < 4; i++) {
            digitalWrite(drumPins[i], HIGH);
            delay(100); // fast spin
            digitalWrite(drumPins[i], LOW);
        }
    }

    Serial.println("Finished! Clothes are clean.");
}

// ==========================
// Main Loop
// ==========================
void loop() {
    // Allow the user to set available power from the Serial Monitor
    if (Serial.available() > 0) {
        availablePower = Serial.parseInt();
        Serial.print("Available power set to: ");
        Serial.println(availablePower);
    }

    if (digitalRead(startButton) == HIGH) {
        delay(200); // debounce
        machineRunning = !machineRunning;

        if (machineRunning) {
            Serial.println("Washing cycle started");

            // Step 1: Fill Drum (before washing)
            checkPower(1);
            fillDrum("washing");

            // Step 2: Washing
            checkPower(2);
            wash_rinse("Washing");

            // Step 3: Drain
            checkPower(1);
            draining();

            // Step 4: Fill Drum (before rinsing)
            checkPower(1);
            fillDrum("rinsing");

            // Step 5: Rinsing
            checkPower(2);
            wash_rinse("Rinsing");

            // Step 6: Drain
            checkPower(1);
            draining();

            // Step 7: Spin
            checkPower(3);
            spinning();

            machineRunning = false;
            Serial.println("Ready for next load.");
        }
    }
}
