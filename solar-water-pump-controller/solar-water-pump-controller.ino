/*
  Solar Water Pump Controller
  ----------------------------
  Runs a 12 V DC pump from a solar-charged battery only when
  it is safe to do so: the battery is charged enough, the
  overhead tank is not full, and the well / source is not dry.
  Pump relay is OFF on startup (safe default). Low battery
  or dry-run detection stops the pump immediately.

  Wiring:
    Panel divider  : panel+ - 30k - A0 - 7.5k - GND
    Battery divider: batt+  - 30k - A1 - 7.5k - GND
    Tank float     : D2 (float switch to GND, INPUT_PULLUP)
    Well float     : D3 (float switch to GND, INPUT_PULLUP)
    Pump relay     : D7 (active LOW)
    Status LED     : D13 (through a 220 ohm resistor to GND)
*/

// ---- Pin definitions ----
const int PANEL_PIN    = A0;  // Solar panel voltage divider
const int BATTERY_PIN  = A1;  // Battery voltage divider
const int TANK_FLOAT   = 2;   // HIGH level float: LOW = tank full
const int WELL_FLOAT   = 3;   // Low level float: HIGH = well dry
const int PUMP_RELAY   = 7;   // Pump relay (active LOW)
const int STATUS_LED   = 13;  // Built-in / status LED

// ---- Calibration / thresholds ----
const float DIVIDER_RATIO = 5.0;  // (30k + 7.5k) / 7.5k
const float ADC_REF       = 5.0;  // Arduino ADC reference (V)

const float PUMP_START_VOLTAGE = 12.5;  // Start pump above this
const float PUMP_STOP_VOLTAGE  = 11.5;  // Stop pump below this (cutoff)

bool pumpOn = false;

float readVoltage(int pin) {
  int raw = analogRead(pin);
  return (raw * ADC_REF / 1023.0) * DIVIDER_RATIO;
}

void setPump(bool on) {
  pumpOn = on;
  // Active-LOW relay: LOW = pump ON, HIGH = pump OFF.
  digitalWrite(PUMP_RELAY, on ? LOW : HIGH);
  digitalWrite(STATUS_LED, on ? HIGH : LOW);
}

void setup() {
  pinMode(PUMP_RELAY, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(TANK_FLOAT, INPUT_PULLUP);
  pinMode(WELL_FLOAT, INPUT_PULLUP);

  // Safe default: pump OFF on startup.
  setPump(false);

  Serial.begin(9600);
  Serial.println("Solar Water Pump Controller ready.");
}

void loop() {
  float panelVoltage   = readVoltage(PANEL_PIN);
  float batteryVoltage = readVoltage(BATTERY_PIN);
  bool tankFull = (digitalRead(TANK_FLOAT) == LOW);
  bool wellDry  = (digitalRead(WELL_FLOAT) == HIGH);

  // Stop conditions first — they always win.
  if (pumpOn && wellDry) {
    setPump(false);
    Serial.println("Well dry - pump OFF (dry-run protection)");
  } else if (pumpOn && tankFull) {
    setPump(false);
    Serial.println("Tank full - pump OFF");
  } else if (pumpOn && batteryVoltage < PUMP_STOP_VOLTAGE) {
    setPump(false);
    Serial.println("Battery low - pump OFF (low-voltage cutoff)");
  }

  // Start only when battery is charged, tank needs water,
  // and the well has water.
  if (!pumpOn && !tankFull && !wellDry &&
      batteryVoltage >= PUMP_START_VOLTAGE) {
    setPump(true);
    Serial.println("Pump ON");
  }

  Serial.print("Panel: ");
  Serial.print(panelVoltage, 2);
  Serial.print(" V | Battery: ");
  Serial.print(batteryVoltage, 2);
  Serial.print(" V | Tank: ");
  Serial.print(tankFull ? "FULL" : "not full");
  Serial.print(" | Well: ");
  Serial.print(wellDry ? "DRY" : "has water");
  Serial.print(" | Pump: ");
  Serial.println(pumpOn ? "ON" : "OFF");

  delay(1000);
}
