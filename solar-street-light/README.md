# Solar Street Light

A solar panel charges a 12 V battery during the day, and an
LED street lamp turns on automatically at dusk and off at dawn,
using an LDR sensor and relays — no mains power or manual
switching needed.

## How it works

Two voltage dividers scale the panel voltage and the battery
voltage down to the Arduino's 0–5 V ADC range. By day, if the
panel voltage is higher than the battery voltage and the
battery is not full, the charge relay connects the panel to
the battery. The LDR divider reads ambient light: below the
dusk threshold it is night, so the lamp relay turns the LED
lamp on; above the dawn threshold it is day, so the lamp
turns off. If the battery falls to the low-battery cutoff at
night, the lamp turns off to protect the battery from deep
discharge. The two LDR thresholds give hysteresis, so the
lamp does not flicker at the switching point.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Solar panel | 12 V, 10–20 W | 1 |
| Battery | 12 V lead-acid | 1 |
| LED lamp | 12 V LED, 5–10 W | 1 |
| Relay module | 5 V, 2-channel (or 2× 1-channel) | 1 |
| LDR | Light-dependent resistor | 1 |
| Resistors | 30 kΩ and 7.5 kΩ per divider (panel + battery = 2 sets) | 4 |
| Resistor | 10 kΩ (LDR divider), 220 Ω (LED) | 2 |
| LED | 5 mm (status) | 1 |
| Fuse | 5 A, in series with the battery | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
Solar panel + ── Relay 1 COM     Relay 1 NO ── Battery + (through 5A fuse)
Solar panel - ─────────────────── Battery - / Arduino GND (common)

Battery + ── Relay 2 COM         Relay 2 NO ── LED lamp +
Battery - ────────────────────── LED lamp - / GND

Battery + ── 30k ──┬── A1 ── 7.5k ── GND   (battery divider, ~5:1)
Panel + ──── 30k ──┬── A0 ── 7.5k ── GND   (panel divider, ~5:1)
5V ── LDR ──┬── A2 ── 10k ── GND           (LDR divider)

Arduino D7 ── Relay 1 IN (charge, active LOW)
Arduino D8 ── Relay 2 IN (lamp, active LOW)
Arduino D13 ── 220Ω ── Status LED ── GND
```

Divider ratio: 30 kΩ + 7.5 kΩ gives a 5:1 ratio, so up to 25 V
reads as up to 5 V at the ADC. Both relays are OFF on startup.

## Wiring steps

1. **Do the low-voltage divider, LDR and LCD wiring first,
   with the panel and battery disconnected.**
2. Build both voltage dividers and the LDR divider as shown;
   check each with a multimeter before connecting to the
   Arduino.
3. Connect the relays: IN → D7 / D8, VCC → 5 V, GND → GND.
   Panel + → relay 1 COM, relay 1 NO → battery + through the
   5 A fuse. Battery + → relay 2 COM, relay 2 NO → lamp +.
4. Connect battery −, panel − and lamp − to Arduino GND
   (common ground).
5. Upload `solar-street-light.ino` and open the Serial
   Monitor (9600 baud). Reconnect the battery, then the panel.
6. Test in daylight: charging should start if the battery is
   not full. Cover the LDR with your hand — the lamp should
   turn on as if it were dusk.

## Calibration

Measure the real battery voltage with a multimeter and
compare it with the Serial reading; if it is off, adjust
`DIVIDER_RATIO` (default 5.0) until they agree. Tune
`DUSK_THRESHOLD` / `DAWN_THRESHOLD` from the Serial LDR
readings in your location. Defaults: full charge 14.4 V,
resume charging 13.0 V, lamp low-battery cutoff 11.0 V —
suitable for a 12 V lead-acid battery.

## Safety notes

- Reversed polarity will damage the Arduino and battery —
  check +/− with a multimeter before connecting.
- Use the 5 A fuse in series with the battery. Never short a
  12 V battery.
- This project is for 12 V DC only. Never connect it to AC
  mains, and do not use a mains lamp on the lamp relay.
- The charge cutoff is a simple relay cutoff, not a proper
  MPPT / charge controller — do not leave it unattended for
  long periods with a large panel.
