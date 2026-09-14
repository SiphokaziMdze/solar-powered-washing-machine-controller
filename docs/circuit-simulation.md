# Circuit - Tinkercad Simulation

LED-based simulation of the washing machine, buildable entirely in Tinkercad before any physical hardware is involved.

## Components

| Qty | Component |
|---|---|
| 1 | Arduino Uno R3 |
| 1 | Breadboard |
| 4 | Green LEDs (drum motor) |
| 1 | Blue LED (water inlet valve) |
| 1 | Red LED (pump) |
| 6 | 220 ohm resistors (one per LED) |
| 1 | Push button (start/pause) |
| 1 | 10 kOhm resistor (button pull-down) |
| 1 | Potentiometer, 10 kOhm (water level sensor, on A5) |
| - | Jumper wires |

## Wiring

**1. Drum motor (4 green LEDs, pins 4-7)**
- Each LED's cathode (short leg) -> GND rail, through a 220 ohm resistor.
- Each LED's anode (long leg) -> Arduino pin: LED1->4, LED2->5, LED3->6, LED4->7.

**2. Water inlet valve (blue LED, pin 8)**
- Cathode -> GND rail via 220 ohm resistor.
- Anode -> Arduino pin 8.

**3. Pump (red LED, pin 9)**
- Cathode -> GND rail via 220 ohm resistor.
- Anode -> Arduino pin 9.

**4. Start/Pause button (pin 10, pull-down)**
- One leg -> Arduino pin 10.
- Other leg -> 5V.
- 10 kOhm resistor from the pin-10 side -> GND (so the pin reads LOW when the button is not pressed).

**5. Water level sensor (potentiometer, A5)**
- Left pin -> 5V.
- Right pin -> GND.
- Middle pin (wiper) -> Arduino A5. Turning the knob simulates the water level rising and falling.

**6. Power rails**
- Arduino GND -> breadboard GND (-) rail.
- Arduino 5V -> breadboard + rail.
- Every component's GND/power leg ties into these shared rails.

## Layout Summary

```text
Arduino          Breadboard
---------        ---------------------
Pin 4   -> LED1 -> 220 ohm -> GND
Pin 5   -> LED2 -> 220 ohm -> GND
Pin 6   -> LED3 -> 220 ohm -> GND
Pin 7   -> LED4 -> 220 ohm -> GND

Pin 8   -> Blue LED (valve) -> 220 ohm -> GND
Pin 9   -> Red LED (pump)   -> 220 ohm -> GND

Pin 10  -> Button leg A
5V      -> Button leg B
GND     -> 10 kOhm -> Pin 10 (pull-down)

A5      -> Potentiometer wiper
5V      -> Potentiometer left pin
GND     -> Potentiometer right pin

5V      -> Breadboard + rail
GND     -> Breadboard - rail
```

## How to Run It

1. Upload `arduino/tinkercad_simulation/tinkercad_simulation.ino`.
2. Open the Serial Monitor (9600 baud).
3. Type a number `0-3` and press Send to set the available simulated solar power.
4. Turn the potentiometer to raise the simulated water level. The valve LED turns off once it crosses approximately 80%.
5. Press the start button. The machine runs through fill -> wash -> drain -> fill -> rinse -> drain -> spin, pausing at each stage until `checkPower()` detects enough available power.
