// ======================================================
// TRASHBOT ARM CONTROLLER
// Waste Pickup + Sorting Controller
//
// Responsibilities:
// 1. Robotic arm control
// 2. Waste pickup operation
// 3. Waste drop operation
// 4. Wet/Dry segregation
// 5. Servo synchronization
//
// Communication:
// Receives PICK command from master
// Sends ARM_BUSY and ARM_DONE status
// ======================================================

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial link(7, 8);

// ======================================================
// SERVO OBJECTS
// ======================================================

Servo base;
Servo shoulder;
Servo elbow;
Servo gripper;
Servo flipper;

// ======================================================
// SERVO PIN CONFIGURATION
// ======================================================

#define BASE_PIN 5
#define SHOULDER_PIN 6
#define ELBOW_PIN 9
#define GRIPPER_PIN 10
#define FLIPPER_PIN 3

#define WET_SENSOR 4

// ======================================================
// ARM POSITION DEFINITIONS
// Predefined stable servo positions
// ======================================================

#define BASE_HOME 90

#define SHOULDER_PICK 20
#define SHOULDER_DROP 90
#define SHOULDER_HOME 90

#define ELBOW_PICK 100
#define ELBOW_DROP 0
#define ELBOW_HOME 100

#define GRIP_OPEN 180
#define GRIP_CLOSE 20

#define FLIP_CENTER 110
#define FLIP_LEFT 60
#define FLIP_RIGHT 180

// ======================================================
// ARM FSM STATES
//
// IDLE   -> Waiting for command
// PICK   -> Pickup operation
// DROP   -> Drop waste
// SORT   -> Wet/Dry segregation
// RETURN -> Return to home position
// ======================================================

enum ArmState {
  IDLE,
  PICK,
  DROP,
  SORT,
  RETURN
};

ArmState state = IDLE;

// ======================================================
// SYNCHRONIZED ARM MOVEMENT
// Moves shoulder and elbow together smoothly
// ======================================================

void moveBoth(int sTarget, int eTarget) {

  int s = shoulder.read();
  int e = elbow.read();

  while (s != sTarget || e != eTarget) {

    if (s < sTarget)
      s++;
    else if (s > sTarget)
      s--;

    if (e < eTarget)
      e++;
    else if (e > eTarget)
      e--;

    shoulder.write(s);
    elbow.write(e);

    delay(15);
  }
}

// ======================================================
// SYSTEM INITIALIZATION
// ======================================================

void setup() {

  Serial.begin(9600);
  link.begin(9600);

  // Servo Attach
  base.attach(BASE_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  gripper.attach(GRIPPER_PIN);
  flipper.attach(FLIPPER_PIN);

  // Sensor Pin
  pinMode(WET_SENSOR, INPUT);

  // Initial Servo Positions
  base.write(BASE_HOME);

  shoulder.write(SHOULDER_HOME);
  elbow.write(ELBOW_HOME);

  gripper.write(GRIP_OPEN);

  flipper.write(FLIP_CENTER);

  Serial.println("ARM READY");
}

// ======================================================
// MAIN ARM FSM
// Controls complete pickup sequence
// ======================================================

void loop() {

  // ======================================================
  // RECEIVE COMMAND FROM MASTER
  // ======================================================

  if (link.available()) {

    String cmd = link.readStringUntil('\n');

    cmd.trim();

    if (cmd == "PICK" && state == IDLE) {

      state = PICK;
    }
  }

  switch (state) {

    // ======================================================
    // IDLE STATE
    // ======================================================

    case IDLE:

      break;

    // ======================================================
    // PICK STATE
    // ======================================================

    case PICK:

      link.println("ARM_BUSY");

      moveBoth(SHOULDER_PICK, ELBOW_PICK);

      gripper.write(GRIP_CLOSE);

      delay(800);

      state = DROP;

      break;

    // ======================================================
    // DROP STATE
    // ======================================================

    case DROP:

      moveBoth(SHOULDER_DROP, ELBOW_DROP);

      gripper.write(GRIP_OPEN);

      delay(800);

      state = SORT;

      break;

    // ======================================================
    // SORT STATE
    // ======================================================

    case SORT:

      if (digitalRead(WET_SENSOR)) {

        flipper.write(FLIP_RIGHT);
      }

      else {

        flipper.write(FLIP_LEFT);
      }

      delay(1000);

      flipper.write(FLIP_CENTER);

      state = RETURN;

      break;

    // ======================================================
    // RETURN STATE
    // ======================================================

    case RETURN:

      moveBoth(SHOULDER_HOME, ELBOW_HOME);

      link.println("ARM_DONE");

      state = IDLE;

      break;
  }
}
