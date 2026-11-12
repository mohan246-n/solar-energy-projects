# Solar Water Pump Controller

Runs a 12 V DC water pump from a solar-charged battery only
when it is safe: the battery is charged enough, the overhead
tank still needs water, and the well / source has not run
dry. Protects both the battery and the pump.

## How it works

Two voltage dividers scale the panel voltage and the battery
voltage down to the Arduino's 0–5 V ADC range, so the Arduino
always knows the battery state. A float switch at the top of
the overhead tank signals "tank full", and a float switch in
the well / source tank signals "well dry". The pump relay
turns on only when the battery is at or above the start
voltage, the tank is not full, and the well has water. It
turns off immediately if the tank fills, the well runs dry
(dry-run protection — a pump run without water burns out
quickly), or the battery falls to the low-voltage cutoff.
The gap between start and stop voltage is hysteresis, so the
relay does not chatter around one threshold.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Solar panel | 12 V, 20 W or larger | 1 |
| Battery | 12 V lead-acid | 1 |
| Water pump | 12 V DC submersible / diaphragm | 1 |
| Relay module | 5 V, 1-channel, rated above pump current | 1 |
| Float switch | Tank high level | 1 |
| Float switch | Well low level (dry-run) | 1 |
| Resistors | 30 kΩ and 7.5 kΩ per divider (panel + battery = 2 sets) | 4 |
| Resistor | 220 Ω (LED) | 1 |
| LED | 5 mm (status) | 1 |
| Fuse | Rated for the pump, in series with the battery | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
Battery + ── Fuse ── Relay COM     Relay NO ── Pump +
Battery - ───────────────────────── Pump - / Arduino GND (common)
Solar panel connects to the battery through your charge
controller / monitor (see solar-battery-charger-monitor).

Battery + ── 30k ──┬── A1 ── 7.5k ── GND   (battery divider, ~5:1)
Panel + ──── 30k ──┬── A0 ── 7.5k ── GND   (panel divider, ~5:1)

Arduino D2 ── Tank float switch ── GND  (closed = tank full)
Arduino D3 ── Well float switch ── GND  (open = well dry)
Arduino D7 ── Relay IN (pump relay, active LOW)
Arduino D13 ── 220Ω ── Status LED ── GND
```

Float switches use the Arduino's internal pull-ups, so no
external resistors are needed for them. The pump relay is
OFF on startup.

## Wiring steps

1. **Do the low-voltage divider and float-switch wiring
   first, with the panel, battery and pump disconnected.**
2. Build both voltage dividers as shown; check each with a
   multimeter before connecting to the Arduino.
3. Wire the tank float switch between D2 and GND near the
   top of the tank, and the well float switch between D3
   and GND so it opens when the water falls below it.
4. Connect the relay: IN → D7, VCC → 5 V, GND → GND.
   Battery + → fuse → relay COM; relay NO → pump +.
   Pump − → battery − / common GND.
5. Upload `solar-water-pump-controller.ino` and open the
   Serial Monitor (9600 baud).
6. Test the floats by hand first (lift / lower each float)
   and watch the Serial status before fitting the pump in
   water. Only then connect the pump.

## Calibration

Measure the real battery voltage with a multimeter and
compare it with the Serial reading; if it is off, adjust
`DIVIDER_RATIO` (default 5.0) until they agree. Defaults:
pump starts at 12.5 V and stops at 11.5 V (low-voltage
cutoff) — suitable for a 12 V lead-acid battery. Raise the
stop voltage if your pump draws a heavy current, so the
battery is not pulled down too far while pumping.

## Safety notes

- Reversed polarity will damage the Arduino, battery and
  pump — check +/− with a multimeter before connecting.
- Use a fuse rated for the pump in series with the battery.
  Size the relay above the pump's *starting* current, not
  just its running current.
- Keep all mains wiring out of this project: it is 12 V DC
  only. If you adapt it to an AC pump, get a qualified
  electrician to wire the AC side through an enclosed relay.
- Never let the pump run dry to "test" it — dry-run
  protection is a backup, not a substitute for checking
  the well / source level.
- Keep electrical connections above water level and in a
  dry, ventilated box. Water and bare wiring do not mix.
