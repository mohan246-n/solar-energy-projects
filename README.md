# Solar Energy Projects

A collection of 5 practical, beginner-friendly Arduino-based solar
energy projects, built for learning, home use, and portfolio / college work.
Each project lives in its own folder with its own README, working
Arduino code (`.ino`), and a text circuit diagram.

## Projects

| # | Project | What it does |
|---|---------|--------------|
| 1 | [solar-battery-charger-monitor](solar-battery-charger-monitor/) | Monitors a solar panel and a 12 V battery, shows voltage on LCD/serial, and cuts off charging when the battery is full. |
| 2 | [solar-street-light](solar-street-light/) | Solar panel charges the battery by day; an LDR turns the LED street lamp on at dusk and off at dawn. |
| 3 | [solar-water-pump-controller](solar-water-pump-controller/) | Runs a 12 V pump from the battery only when it is charged, the tank needs water, and the well is not dry. |
| 4 | [solar-panel-tracker](solar-panel-tracker/) | Two LDRs and a servo tilt the panel towards the brighter side, following the sun across the sky. |
| 5 | [battery-overcharge-protection](battery-overcharge-protection/) | Watches battery voltage while charging and cuts the charger off at full charge, with a buzzer/LED alarm. |

## How to use

1. Pick a project folder and read its `README.md` first — it lists the
   exact components, wiring, and safety notes for that project.
2. Open the `.ino` file in the Arduino IDE (or Arduino CLI / VS Code
   with the Arduino extension).
3. Select your board (Arduino Uno for all projects in this repo) and
   the correct port, then upload.
4. Wire the circuit exactly as described in the project README.
   **Do not power the circuit until you have double-checked the wiring.**
5. Open the Serial Monitor at 9600 baud (where the project uses it)
   to see readings and status messages.

## Common components and tools

- Arduino Uno (all 5 projects)
- Breadboard, jumper wires, USB cable
- Resistors: 30 kΩ, 10 kΩ, 7.5 kΩ, 220 Ω (see each project for exact values)
- 5 V relay module(s), LEDs, buzzer, LDRs, servo (project 4)
- Solar panel (12 V, 10–20 W) and 12 V lead-acid battery
- 16×2 LCD with I2C backpack (project 1)
- Float switches and a 12 V DC pump (project 3)
- Multimeter, wire stripper, screwdriver, soldering iron (optional)

## ⚠️ Safety warnings — read before building anything

- **Batteries store a lot of energy.** A shorted 12 V lead-acid
  battery can weld tools, melt wires and start a fire. Always use
  a fuse (e.g. 5 A) in series with the battery, never short its
  terminals, and keep metal tools away from them.
- Reversed polarity will damage the Arduino, the battery and any
  connected load. Check every +/− connection with a multimeter
  before first power-up.
- Charge lead-acid batteries in a ventilated place — they can
  vent hydrogen gas, especially if overcharged. Keep sparks and
  flames away.
- The voltage-divider circuits in this repo are for **DC only**
  (up to 25 V). Never connect them to AC mains.
- **Mains voltage (230 V AC in India / 110–240 V AC elsewhere)
  can kill.** These projects are 12 V DC projects. Do not switch
  mains loads with them; if a project is ever adapted for AC,
  get a qualified electrician to wire the AC side through an
  enclosed relay module, fully separate from the Arduino side.
- Solar panels produce voltage whenever light falls on them,
  even on a cloudy day and even when "switched off". Cover the
  panel or work in shade while wiring it.
- The relay charge cutoffs in these projects are simple cutoffs,
  not proper MPPT / charge controllers, and are set for 12 V
  lead-acid batteries. Do not use them as the only protection
  for lithium batteries, and do not leave large systems
  unattended for long periods.

## Repo layout

```
solar-energy-projects/
├── README.md
├── LICENSE
├── .gitignore
├── solar-battery-charger-monitor/
├── solar-street-light/
├── solar-water-pump-controller/
├── solar-panel-tracker/
└── battery-overcharge-protection/
```

## Pushing to GitHub

```bash
git init
git add .
git commit -m "Add 5 Arduino solar energy projects"
git branch -M main
git remote add origin https://github.com/<your-username>/solar-energy-projects.git
git push -u origin main
```

## License

MIT — see [LICENSE](LICENSE).
