# Solar Panel Tracker

A single-axis solar tracker: two LDRs compare the light on
either side of the panel, and a servo tilts the panel
towards the brighter side so it follows the sun and produces
more power over the day than a fixed panel.

## How it works

Two LDR voltage dividers are mounted on the panel, one on
the east edge and one on the west edge, separated by a small
vertical fin that shades one side when the sun is off-centre.
The Arduino reads both LDRs and compares them. If the east
side is brighter by more than the tolerance (deadband), the
servo steps the panel towards the east; if the west side is
brighter, it steps the other way. When the readings are
close, the panel is aimed at the sun and the servo holds
still — the deadband stops constant jittering, which wastes
power and wears the servo out. The same comparison works
with four LDRs and a second servo for dual-axis tracking.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| LDR | Light-dependent resistor | 2 (4 for dual-axis) |
| Resistors | 10 kΩ, one per LDR divider | 2 (4 for dual-axis) |
| Servo motor | MG996R / high-torque for a real panel; SG90 only for a demo model | 1 |
| Solar panel | Small panel to mount on the tracker | 1 |
| Divider fin | Card / acrylic strip between the LDRs | 1 |
| Frame / bracket | Wood, acrylic or aluminium to tilt the panel | 1 |
| Power supply | 5–6 V, 2 A for the servo (separate from Arduino USB) | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
5V ── LDR (east) ──┬── A0 ── 10k ── GND   (east LDR divider)
5V ── LDR (west) ──┬── A1 ── 10k ── GND   (west LDR divider)

Arduino D9 ── Servo signal (orange / yellow wire)
Servo + (red) ── 5-6V servo supply +  (NOT the Arduino 5V pin)
Servo - (brown/black) ── Servo supply - AND Arduino GND (common ground)

Divider fin: a vertical strip fixed to the panel between the
two LDRs, so one LDR is shaded when the sun is off-centre.
```

Mount the LDRs flat on the panel face, on either side of
the fin, so both see the same light only when the panel
faces the sun directly.

## Wiring steps

1. Build the two LDR dividers on the breadboard and check
   with the Serial Monitor that each reading rises when you
   shine a torch on that LDR.
2. Connect the servo signal wire to D9. Power the servo
   from its own 5–6 V supply and join that supply's ground
   to Arduino GND — servos under load pull too much current
   for the Arduino's 5 V pin and will reset it.
3. Fix the panel, LDRs and fin to the tilting frame and
   couple the frame to the servo horn. Start with a light
   demo panel before mounting anything heavy.
4. Upload `solar-panel-tracker.ino` and open the Serial
   Monitor (9600 baud). The servo starts at 90°.
5. Shine a torch from one side, then the other — the panel
   should step towards the torch each time. If it turns
   *away* from the light, swap the two LDR connections.

## Calibration

Cover both LDRs equally in steady light and check the two
Serial readings are close. If one LDR always reads higher,
its pair is mismatched — raise `TOLERANCE` (default 50) so
the deadband covers the mismatch, rather than chasing exact
matching. Use `STEP_SIZE` and the loop delay to trade speed
against smoothness, and keep `MIN_ANGLE` / `MAX_ANGLE`
inside the range where your frame can actually move without
binding or tipping over.

## Safety notes

- Power the servo from a separate supply with a common
  ground, never from the Arduino 5 V pin.
- Check the frame is balanced and cannot tip or fall,
  especially outdoors in wind. Weatherproof all wiring and
  connections if the tracker lives outside.
- A tracked panel still produces full voltage in sunlight —
  treat its output wiring like any other solar panel wiring
  (DC only, correct polarity, fuse if it charges a battery).
- Start small: learn the movement with a phone torch and a
  demo panel indoors before building a full-size version.
