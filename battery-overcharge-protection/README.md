# Battery Overcharge Protection

Protects a 12 V lead-acid battery from overcharging: it
watches the battery voltage while a solar panel or charger
is connected, cuts the charger off when the battery is
full, and sounds an alarm so you know charging is done.

## How it works

A voltage divider scales the battery voltage down to the
Arduino's 0–5 V ADC range. Each measurement averages several
ADC readings, so relay decisions are not made on a single
noisy sample. While the battery is below the resume voltage,
the charge relay is ON and the battery charges. When the
battery reaches the full-charge voltage, the relay turns
OFF, disconnecting the charger, and the buzzer and LED pulse
as a "battery full" alarm. Charging does not restart until
the battery falls back to the resume voltage — that gap
(hysteresis) stops the relay switching on and off rapidly
around the full point, which would wear it out and stress
the battery. Overcharging a lead-acid battery boils off its
electrolyte and shortens its life, so this cutoff matters.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Battery | 12 V lead-acid | 1 |
| Charger / solar panel | 12 V charging source | 1 |
| Relay module | 5 V, 1-channel, rated above charge current | 1 |
| Resistors | 30 kΩ and 7.5 kΩ (battery divider) | 2 |
| Resistor | 220 Ω (LED) | 1 |
| Buzzer | 5 V active buzzer | 1 |
| LED | 5 mm red | 1 |
| Fuse | 5 A, in series with the battery | 1 |
| Diode | 1N4007 (charger → relay, prevents back-feed) | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
Charger + ── 1N4007 diode ── Relay COM
Relay NO ──────────────────── Battery + (through 5A fuse)
Charger - ─────────────────── Battery - / Arduino GND (common)

Battery + ── 30k resistor ──┬── A0 (Arduino) ── 7.5k resistor ── GND
                            (battery voltage divider, ~5:1)

Arduino D7 ── Relay IN (charge relay, active LOW)
Arduino D8 ── Buzzer + (buzzer - to GND)
Arduino D13 ── 220Ω ── Red LED ── GND
```

Divider ratio: 30 kΩ + 7.5 kΩ gives a 5:1 ratio, so up to
25 V reads as up to 5 V at the ADC. The relay is OFF (not
charging) on startup.

## Wiring steps

1. **Do the low-voltage divider, buzzer and LED wiring
   first, with the charger and battery disconnected.**
2. Build the divider as shown; check it with a multimeter
   before connecting to the Arduino.
3. Connect the buzzer (+ → D8, − → GND) and the LED
   (D13 → 220 Ω → LED anode, cathode → GND).
4. Connect the relay: IN → D7, VCC → 5 V, GND → GND.
   Charger + → diode → relay COM; relay NO → battery +
   through the 5 A fuse.
5. Connect battery − and charger − to Arduino GND
   (common ground).
6. Upload `battery-overcharge-protection.ino` and open
   the Serial Monitor (9600 baud). Reconnect the battery,
   then the charger, and watch the voltage climb.

## Calibration

Measure the real battery voltage with a multimeter and
compare it with the Serial reading. If it is off, adjust
`DIVIDER_RATIO` (default 5.0) in the code until they agree —
an error here means the cutoff happens at the wrong real
voltage, so this step is important. Default cutoffs: full
charge 14.4 V, resume charging at 13.0 V — suitable for a
12 V lead-acid battery. Use lower values (e.g. 13.8 V float)
for a battery kept on charge permanently, and different
values entirely for lithium batteries (see safety notes).

## Safety notes

- **Lead-acid values only.** The 14.4 V / 13.0 V defaults
  are for 12 V lead-acid batteries. Lithium batteries need
  a proper BMS and charger — do not use this simple relay
  cutoff as their only protection.
- Reversed polarity will damage the Arduino and battery —
  check +/− with a multimeter before connecting.
- Use the 5 A fuse in series with the battery. Never short
  a 12 V battery, and charge lead-acid batteries in a
  ventilated place (they can vent hydrogen gas).
- This project is for DC only. Never connect it to AC
  mains. If your charger plugs into mains, only its low-
  voltage DC output side belongs in this circuit.
- This is a simple protection device, not a certified
  charge controller — check on the battery regularly and
  do not leave a large charger unattended.
