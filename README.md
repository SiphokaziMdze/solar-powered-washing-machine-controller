# Solar-Powered Washing Machine Controller ☀️🧺

An Arduino-based washing machine controller designed to operate using available solar power. The project was developed as an **Embedded Systems** project and implemented in two stages: a **Tinkercad simulation** followed by a **physical hardware implementation**.

## 🎯 Project Overview

The system controls a complete washing cycle while checking whether enough solar power is available for each stage. If the available power is too low, the controller pauses and waits until sufficient power is available before continuing.

The washing cycle follows:

**Fill → Wash → Drain → Fill → Rinse → Drain → Spin**

| Stage | Required Power |
|---|---:|
| Fill before wash | 1 |
| Washing | 2 |
| Drain | 1 |
| Fill before rinse | 1 |
| Rinsing | 2 |
| Drain | 1 |
| Spin | 3 |

## ⚙️ Key Features

- Automated washing, rinsing, draining and spinning cycles
- Solar power availability checking before each cycle stage
- Water-level monitoring with an analogue sensor
- Automatic inlet valve control
- Pump control for draining
- Stepper motor control for the washing drum
- Clockwise and anticlockwise drum rotation
- 20-turn spin cycle
- Start/pause button
- Serial Monitor status messages
- Tinkercad simulation for testing the control logic
- Physical hardware implementation using a 28BYJ-48 stepper motor

## 🧪 Tinkercad Simulation

The first version of the project was developed and tested in Tinkercad before moving to physical hardware.

In the simulation:

- Four LEDs represent the drum motor
- A blue LED represents the water inlet valve
- A red LED represents the pump
- A potentiometer represents the water-level sensor
- A push button starts the washing cycle
- The Serial Monitor is used to set the simulated available solar power from **0-3**

The simulation makes it possible to test the complete washing-cycle logic before connecting physical motors and other hardware.

## 🔧 Physical Hardware

The physical implementation uses an Arduino Uno together with:

- **28BYJ-48 stepper motor** for the washing drum
- **ULN2003 driver board** for the stepper motor
- DC motor for the pump
- LED/valve output
- Push button for start/pause control
- Potentiometer for water-level sensing
- Potentiometer or IR receiver for solar-power input

### Stepper Motor Control

The stepper motor is controlled manually using an **8-step half-step sequence** rather than a Stepper or AccelStepper library. This provides explicit control over the motor phases and supports clockwise and anticlockwise rotation.

## 🧠 Control Logic

The controller checks the available power before starting each stage.

```text
Start
  ↓
Check Solar Power
  ↓
Enough Power?
  ├── No → Wait and check again
  └── Yes
        ↓
       Fill
        ↓
       Wash
        ↓
       Drain
        ↓
       Fill
        ↓
       Rinse
        ↓
       Drain
        ↓
       Spin
        ↓
      Finish
```

The water inlet remains active until the water-level reading reaches approximately **80%** of the analogue range. During washing and rinsing, the drum alternates between clockwise and anticlockwise movement. The final spin rotates the drum for 20 turns.

## 📁 Repository Structure

```text
solar-powered-washing-machine-controller/
├── arduino/
│   ├── tinkercad_simulation/
│   │   └── tinkercad_simulation.ino
│   └── physical_hardware/
│       └── physical_hardware.ino
├── docs/
│   ├── circuit-simulation.md
│   └── circuit-physical.md
└── README.md
```

## 🛠️ Technology & Tools

- Arduino Uno
- C++ / Arduino programming
- Tinkercad
- Stepper motor control
- Sensor integration
- Embedded systems
- Serial communication
- Digital and analogue I/O

## 📚 Documentation

Detailed wiring information is available in:

- [`docs/circuit-simulation.md`](docs/circuit-simulation.md) - Tinkercad circuit and wiring
- [`docs/circuit-physical.md`](docs/circuit-physical.md) - physical hardware wiring and components

## 💡 What I Learned

This project gave me practical experience in **embedded systems, hardware interfacing, sensor integration, motor control, analogue and digital I/O, and designing control logic for a real-world system**.

Moving from the Tinkercad simulation to the physical hardware also helped me understand the differences between simulating a system and implementing it with real components.

## 👩🏽‍💻 Project Information

**Project:** Solar-Powered Washing Machine Controller  
**Field:** Embedded Systems  
**Platform:** Arduino Uno  
**Implementation:** Tinkercad Simulation + Physical Hardware

Built by **Siphokazi Veronica Mdze**.
