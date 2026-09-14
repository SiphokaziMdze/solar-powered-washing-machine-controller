# Circuit - Physical Hardware

Real-hardware build: a geared stepper motor for the drum, a DC motor for the pump, a valve, two potentiometers, and an IR receiver for solar power input, all sharing one breadboard with the Arduino.

## Components

| Qty | Component |
|---|---|
| 1 | Arduino Uno |
| 1 | Breadboard |
| 1 | 28BYJ-48 stepper motor + ULN2003 driver board (drum) |
| 1 | Small DC motor (pump) |
| 1 | NPN transistor (e.g. 2N2222/TIP120) + 1 kOhm base resistor + 1N4007 flyback diode, or a relay module, for switching the pump |
| 1 | Breadboard power module (set to 5V), or external 7-9V adapter |
| 1 | LED (blue) for the inlet valve, + 220 ohm resistor, or a real solenoid valve later |
| 1 | Push button (start/pause) + 10 kOhm pull-down resistor |
| 1 | Potentiometer (water level, on A5) |
| 1 | Potentiometer or IR receiver module (solar power level, on A1 / D3) |
| - | Jumper wires |

## Wiring

**1. Power first (stable and safe)**
- Plug the breadboard power module into the top rails, set to 5V, fed from USB or a 7-9V adapter.
- Arduino GND -> breadboard GND rail.
- **Common ground is mandatory** - every module's GND must tie into Arduino GND, whether or not you also share the +5V rail.
- Do not power the motors from the Arduino's own 5V pin. Use the power module's 5V rail for the stepper driver and pump, and share ground with the Arduino.

**2. Drum motor - 28BYJ-48 stepper + ULN2003 (pins 4-7)**
- ULN2003 `IN1` -> Arduino D4, `IN2` -> D5, `IN3` -> D6, `IN4` -> D7.
- ULN2003 `VCC` -> breadboard +5V rail (from the power module).
- ULN2003 `GND` -> breadboard GND rail.
- Plug the stepper's 5-wire JST connector into the ULN2003 header.
- No stepper library is used; `IN1`-`IN4` are stepped in sequence manually (see `stepMotor()` / `HALFSTEP[]` in the sketch).

**3. Pump - small DC motor (pin 9), via transistor switch**
- Motor `+` -> +5V (power module).
- Motor `-` -> transistor collector.
- Transistor emitter -> GND.
- Flyback diode across the motor terminals: banded end to +5V, other end to motor `-`.
- Arduino D9 -> 1 kOhm resistor -> transistor base.
- Optional: 10 kOhm from base -> GND for extra noise immunity.
- `digitalWrite(9, HIGH)` runs the pump, `LOW` stops it.
- **Relay alternative:** relay `VCC` -> +5V, `GND` -> GND, `IN` -> D9; motor wired through the relay's `COM`/`NO` contacts. Use one option, not both.

**4. Inlet valve (pin 8)**
- No solenoid valve yet -> use a blue LED: D8 -> LED anode; LED cathode -> 220 ohm -> GND.
- With a real solenoid valve later, drive it the same way as the pump (relay or transistor + flyback diode).

**5. Start/Pause button (pin 10, pull-down)**
- One leg -> +5V.
- Other leg -> Arduino D10.
- 10 kOhm resistor from D10 -> GND (pull-down, so the pin reads LOW when idle).

**6. Water level sensor - potentiometer (A5)**
- Left pin -> +5V, right pin -> GND, wiper (middle pin) -> A5.
- The code fills until the reading crosses ~818 (~80% of the 0-1023 range).

**7. Solar power input - potentiometer or IR receiver (A1 / D3)**
- Potentiometer version: same as the water sensor, wiper -> A1; `updatePowerLevel()` maps the raw 0-1023 reading down to 0-3.
- IR receiver version (VS1838/TSOP1838-style, 3-pin): `VCC` -> +5V, `GND` -> GND, `OUT` -> D3. Swap in the `IRremote` library to decode actual 0-3 values sent from a remote, replacing the `analogRead(A1)` call.

## Quick Wiring Checklist

- [ ] Arduino GND <-> breadboard GND <-> ULN2003 GND - common ground everywhere
- [ ] Power module feeds +5V to motors/driver and rails, not the Arduino 5V pin
- [ ] ULN2003 `IN1`-`IN4` -> D4-D7, `VCC` -> 5V, `GND` -> GND, stepper plugged in
- [ ] Pump wired via NPN + flyback diode (or relay), D9 -> base/`IN`
- [ ] Valve LED: D8 -> LED -> 220 ohm -> GND
- [ ] Button: +5V -[button]- D10; 10 kOhm from D10 -> GND
- [ ] Water-level pot: ends to 5V/GND, wiper -> A5
- [ ] Solar input: pot wiper -> A1, or IR receiver `OUT` -> D3

## Common Pitfalls

- Powering the stepper from the Arduino's own 5V pin can brown-out the board. Use the external power module.
- Forgetting the flyback diode on the DC motor when using a transistor can cause switching spikes and resets.
- Missing common ground between the Arduino and the motor supply prevents reliable operation.
- Swapping the ULN2003 input order can cause the motor to vibrate instead of rotating.
