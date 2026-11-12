/*
  Solar Street Light
  ------------------
  Solar panel charges a 12 V battery during the day. An LDR
  senses dusk; at night the relay turns the LED lamp ON, and
  at dawn it turns it OFF again. Relays are OFF on startup
  (safe default).

  Wiring:
    Panel divider  : panel+ - 30k - A0 - 7.5k - GND
    Battery divider: batt+  - 30k - A1 - 7.5k - GND
    LDR divider    : 5V - LDR - A2 - 10k resistor - GND
    Charge relay   : D7 (active LOW, panel to battery)
    Lamp relay     : D8 (active LOW, battery to LED lamp)
*/

// ---- Pin definitions ----
const int PANEL_PIN       = A0;  // Solar panel voltage divider
const int BATTERY_PIN     = A1;  // Battery voltage divider
const int LDR_PIN         = A2;  // LDR voltage divider
const int CHARGE_RELAY    = 7;   // Charge relay (active LOW)
const int LAMP_RELAY      = 8;   // Lamp relay (active LOW)
const int STATUS_LED      = 13;  // Built-in / status LED

// ---- Calibration / thresholds ----
const float DIVIDER_RATIO = 5.0;  // (30k + 7.5k) / 7.5k
const float ADC_REF       = 5.0;  // Arduino ADC reference (V)

const float FULL_VOLTAGE    = 14.4;  // Stop charging at this voltage
const float RESUME_VOLTAGE  = 13.0;  // Resume charging below this
const float LOW_BATT_CUTOFF = 11.0;  // Lamp OFF to protect battery

// LDR readings (0-1023). Light ON below DUSK, OFF above DAWN.
const int DUSK_THRESHOLD = 400;
const int DAWN_THRESHOLD = 500;

bool charging = false;
bool lampOn   = false;

float readVoltage(int pin) {
  int raw = analogRead(pin);
  return (raw * ADC_REF / 1023.0) * DIVIDER_RATIO;
}

void setup() {
  pinMode(CHARGE_RELAY, OUTPUT);
  pinMode(LAMP_RELAY, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Safe defaults: both relays OFF on startup.
  digitalWrite(CHARGE_RELAY, HIGH);  // HIGH = relay OFF (active LOW)
  digitalWrite(LAMP_RELAY, HIGH);
  digitalWrite(STATUS_LED, LOW);

  Serial.begin(9600);
  Serial.println("Solar Street Light ready.");
}

void loop() {
  float panelVoltage   = readVoltage(PANEL_PIN);
  float batteryVoltage = readVoltage(BATTERY_PIN);
  int   ldrValue       = analogRead(LDR_PIN);
  bool  isDay          = ldrValue > DAWN_THRESHOLD;
  bool  isNight        = ldrValue < DUSK_THRESHOLD;

  // Daytime charging, with full-charge hysteresis.
  if (isDay && !charging && batteryVoltage < RESUME_VOLTAGE &&
      panelVoltage > batteryVoltage) {
    charging = true;
    digitalWrite(CHARGE_RELAY, LOW);  // LOW = relay ON (charging)
    Serial.println("Daytime - charging ON");
  } else if (charging && (batteryVoltage >= FULL_VOLTAGE || isNight)) {
    charging = false;
    digitalWrite(CHARGE_RELAY, HIGH);
    Serial.println("Charging OFF");
  }

  // Night-time lamp, with low-battery protection.
  if (isNight && !lampOn && batteryVoltage > LOW_BATT_CUTOFF) {
    lampOn = true;
    digitalWrite(LAMP_RELAY, LOW);  // LOW = relay ON (lamp on)
    digitalWrite(STATUS_LED, HIGH);
    Serial.println("Dusk - lamp ON");
  } else if (lampOn && (isDay || batteryVoltage <= LOW_BATT_CUTOFF)) {
    lampOn = false;
    digitalWrite(LAMP_RELAY, HIGH);
    digitalWrite(STATUS_LED, LOW);
    Serial.println("Lamp OFF (dawn or low battery)");
  }

  Serial.print("Panel: ");
  Serial.print(panelVoltage, 2);
  Serial.print(" V | Battery: ");
  Serial.print(batteryVoltage, 2);
  Serial.print(" V | LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | Lamp: ");
  Serial.println(lampOn ? "ON" : "OFF");

  delay(1000);
}
