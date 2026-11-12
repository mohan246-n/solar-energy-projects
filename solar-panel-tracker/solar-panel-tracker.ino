/*
  Solar Panel Tracker (Single-Axis)
  ----------------------------------
  Two LDRs on either side of a divider fin compare the light
  falling on each side of the panel. A servo tilts the panel
  towards the brighter side, so it follows the sun across
  the sky. A deadband (tolerance) stops the servo jittering
  when both sides are almost equal.

  Wiring:
    LDR divider (east): 5V - LDR - A0 - 10k resistor - GND
    LDR divider (west): 5V - LDR - A1 - 10k resistor - GND
    Servo signal      : D9
    Servo power       : 5V (separate supply recommended), GND common
*/

#include <Servo.h>

// ---- Pin definitions ----
const int LDR_EAST_PIN = A0;  // East-side LDR divider
const int LDR_WEST_PIN = A1;  // West-side LDR divider
const int SERVO_PIN    = 9;   // Servo signal

// ---- Calibration / thresholds ----
const int TOLERANCE   = 50;   // Deadband: ignore differences below this
const int STEP_SIZE   = 2;    // Degrees to move per step
const int MIN_ANGLE   = 10;   // Servo travel limits (degrees)
const int MAX_ANGLE   = 170;
const int START_ANGLE = 90;   // Panel faces up / midday at startup

Servo tracker;
int panelAngle = START_ANGLE;

void setup() {
  tracker.attach(SERVO_PIN);
  tracker.write(panelAngle);  // Start in the middle (safe position)

  Serial.begin(9600);
  Serial.println("Solar Panel Tracker ready.");
}

void loop() {
  int eastLight = analogRead(LDR_EAST_PIN);
  int westLight = analogRead(LDR_WEST_PIN);
  int difference = eastLight - westLight;

  if (difference > TOLERANCE) {
    // Brighter in the east: tilt that way.
    panelAngle = min(panelAngle + STEP_SIZE, MAX_ANGLE);
    tracker.write(panelAngle);
    Serial.println("Tilting towards the east (brighter side)");
  } else if (difference < -TOLERANCE) {
    // Brighter in the west: tilt that way.
    panelAngle = max(panelAngle - STEP_SIZE, MIN_ANGLE);
    tracker.write(panelAngle);
    Serial.println("Tilting towards the west (brighter side)");
  }
  // If |difference| <= TOLERANCE the panel is well aimed:
  // hold position so the servo does not jitter.

  Serial.print("East LDR: ");
  Serial.print(eastLight);
  Serial.print(" | West LDR: ");
  Serial.print(westLight);
  Serial.print(" | Angle: ");
  Serial.print(panelAngle);
  Serial.println(" deg");

  delay(500);
}

/*
  Notes:
  - Which direction is "east" depends on how you mount the
    servo and LDRs. If the panel turns away from the sun,
    swap the two LDR connections (A0 / A1).
  - For a 4-LDR dual-axis version, add a second pair of
    LDRs and a second servo for up / down movement, using
    the same difference + deadband logic.
  - A servo moving a real panel must be a high-torque
    (e.g. MG996R) servo or a geared motor; a small SG90 is
    only suitable for a light demonstration panel.
*/
