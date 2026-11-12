/*
  Battery Overcharge Protection
  -------------------------------
  Watches a 12 V battery through a voltage divider while it
  charges. When the battery reaches the full-charge voltage
  the charge relay cuts the charger off, and the buzzer /
  LED alarm warns that the battery is full. Charging can
  only resume once the battery drops to the resume voltage
  (hysteresis). Relay is OFF on startup (safe default).

  Wiring:
    Battery divider: batt+ - 30k - A0 - 7.5k - GND
    Charge relay   : D7 (active LOW, charger to battery)
    Buzzer         : D8 (active buzzer, + to D8, - to GND)
    Alarm LED      : D13 (through a 220 ohm resistor to GND)
*/

// ---- Pin definitions ----
const int BATTERY_PIN = A0;  // Battery voltage divider
const int RELAY_PIN   = 7;   // Charge relay (active LOW)
const int BUZZER_PIN  = 8;   // Buzzer
const int ALARM_LED   = 13;  // Alarm LED

// ---- Calibration / thresholds ----
const float DIVIDER_RATIO = 5.0;  // (30k + 7.5k) / 7.5k
const float ADC_REF       = 5.0;  // Arduino ADC reference (V)

const float FULL_VOLTAGE   = 14.4;  // Cut off charging at this voltage
const float RESUME_VOLTAGE = 13.0;  // Resume charging below this voltage
const float SAMPLES        = 10;    // Readings to average per measurement

bool charging = false;
bool fullAlarm = false;

float readBatteryVoltage() {
  long total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += analogRead(BATTERY_PIN);
    delay(5);
  }
  float average = total / (float)SAMPLES;
  return (average * ADC_REF / 1023.0) * DIVIDER_RATIO;
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ALARM_LED, OUTPUT);

  // Safe defaults: relay OFF, buzzer and LED OFF on startup.
  digitalWrite(RELAY_PIN, HIGH);  // HIGH = relay OFF (active LOW)
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(ALARM_LED, LOW);

  Serial.begin(9600);
  Serial.println("Battery Overcharge Protection ready.");
}

void loop() {
  float batteryVoltage = readBatteryVoltage();

  // Charge control with hysteresis.
  if (!charging && batteryVoltage < RESUME_VOLTAGE) {
    charging = true;
    fullAlarm = false;
    digitalWrite(RELAY_PIN, LOW);  // LOW = relay ON (charging)
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(ALARM_LED, LOW);
    Serial.println("Charging ON");
  } else if (charging && batteryVoltage >= FULL_VOLTAGE) {
    charging = false;
    fullAlarm = true;
    digitalWrite(RELAY_PIN, HIGH);  // HIGH = relay OFF (cutoff)
    Serial.println("WARNING: Battery full - charging cut off!");
  }

  // While full, pulse the buzzer and LED as a reminder.
  if (fullAlarm) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(ALARM_LED, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(ALARM_LED, LOW);
    delay(800);
  } else {
    delay(1000);
  }

  Serial.print("Battery: ");
  Serial.print(batteryVoltage, 2);
  Serial.print(" V | ");
  Serial.println(charging ? "CHARGING" : "CUTOFF");
}
