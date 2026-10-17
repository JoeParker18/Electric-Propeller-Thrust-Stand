# Electric Propeller Thrust Stand (in progress)

A self-directed project: an Arduino-based thrust stand to characterize a DC-motor-driven electric propeller.

## What's here
| Path | Contents |
|---|---|
| `firmware/thrust_stand/thrust_stand.ino` | Arduino sketch: sets motor PWM from serial input and streams raw HX711 load-cell counts, supply voltage, and ready flag at 10 Hz |
| `cad/` | Full stand assembly (STEP, STL) and a printable thruster bracket (STL, 3MF, render) |
| `docs/` | Parts list |

## Hardware
- Arduino + HX711 24-bit load-cell amplifier (DOUT pin 2, SCK pin 3)
- Motor PWM on pin 5
- Supply voltage sensed on A0

## Status
The firmware is in diagnostic mode: tare is disabled so raw counts can be checked. Next steps are calibrating the load cell (counts to newtons), adding RPM measurement, and post-processing thrust against throttle and RPM.

## Dependencies
- [HX711 Arduino library](https://github.com/RobTillaart/HX711), installed through the Arduino Library Manager
