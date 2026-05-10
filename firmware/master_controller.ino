// ======================================================
// TRASHBOT MASTER CONTROLLER
// Autonomous Navigation + Bin Monitoring Controller
//
// Responsibilities:
// 1. Robot movement control
// 2. Obstacle detection
// 3. Bin full monitoring
// 4. Communication with robotic arm controller
// 5. FSM based navigation logic
//
// Communication:
// Master <-> Slave using SoftwareSerial
 ======================================================

#include <SoftwareSerial.h>

SoftwareSerial link(7, 8); // RX, TX

// ======================================================
// MOTOR DRIVER PINS
// Controls differential drive DC motors
// ======================================================

#define IN1 2
#define IN2 3
#define ENA 6

#define IN3 4
#define IN4 5
#define ENB 11

// ======================================================
// FRONT ULTRASONIC SENSOR
// Used for obstacle detection
// ======================================================

#define TRIG 9
#define ECHO 10

// ======================================================
// BIN LEVEL SENSORS
// Detects dry/wet bin fill status
// ======================================================

#define DRY_TRIG A0
#define DRY_ECHO A1

#define WET_TRIG A2
#define WET_ECHO A3

// ======================================================
// SYSTEM THRESHOLDS
// ======================================================

#define STOP_DIST 7
#define BIN_FULL_DIST 3

// ======================================================
// FSM STATES
//
// MOVE           -> Normal navigation
// OBSTACLE_STOP  -> Wait when obstacle detected
// AUTO_WAIT      -> Periodic automatic pause
// BIN_FULL       -> Stop system if bin is full
// ARM_LOCK       -> Wait until arm operation completes
// ======================================================

enum State {
  MOVE,
  OBSTACLE_STOP,
  AUTO_WAIT,
  BIN_FULL,
  ARM_LOCK
};

State state = MOVE;

// ======================================================
// SYSTEM FLAGS
// ======================================================

bool armBusy = false;
bool pickSent = false;

// ======================================================
// SYSTEM TIMERS
// ======================================================

unsigned long stateStart = 0;
unsigned long moveStart = 0;

// ======================================================
// ULTRASONIC DISTANCE FUNCTION
// Returns distance in cm
// Returns -1 if timeout occurs
// ======================================================

int readUS(int trig, int echo) {

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);

  long t = pulseIn(echo, HIGH, 30000);

  if (t == 0)
    return -1;

  return t * 0.034 / 2;
}

// ======================================================
// BIN FULL CHECK
// Returns true when waste reaches threshold distance
// ======================================================

bool binFull(int trig, int echo) {

  int d = readUS(trig, echo);

  return (d > 0 && d <= BIN_FULL_DIST);
}

// ======================================================
// MOTOR CONTROL FUNCTIONS
// moveForward() -> Robot forward movement
// stopMotors()  -> Emergency/safe stop
// ======================================================

void moveForward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 180);
  analogWrite(ENB, 180);
}

void stopMotors() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ======================================================
// SYSTEM INITIALIZATION
// ======================================================

void setup() {

  Serial.begin(9600);
  link.begin(9600);

  // Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Ultrasonic Pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Bin Sensor Pins
  pinMode(DRY_TRIG, OUTPUT);
  pinMode(DRY_ECHO, INPUT);

  pinMode(WET_TRIG, OUTPUT);
  pinMode(WET_ECHO, INPUT);

  moveStart = millis();

  Serial.println("MASTER READY");
}

// ======================================================
// MAIN FSM LOGIC
// Handles autonomous robot behavior
// ======================================================

void loop() {

  // ======================================================
  // SERIAL COMMUNICATION HANDLING
  //
  // ARM_BUSY -> Slave started pickup operation
  // ARM_DONE -> Slave completed operation
  // ======================================================

  if (link.available()) {

    String msg = link.readStringUntil('\n');
    msg.trim();

    if (msg == "ARM_BUSY") {

      armBusy = true;

      state = ARM_LOCK;

      stopMotors();
    }

    if (msg == "ARM_DONE") {

      armBusy = false;

      pickSent = false;

      state = MOVE;

      moveStart = millis();
    }
  }

  // ======================================================
  // SENSOR READINGS
  // ======================================================

  int obs = readUS(TRIG, ECHO);

  bool dry = binFull(DRY_TRIG, DRY_ECHO);
  bool wet = binFull(WET_TRIG, WET_ECHO);

  unsigned long now = millis();

  // ======================================================
  // FSM EXECUTION
  // ======================================================

  switch (state) {

    // ======================================================
    // MOVE STATE
    // ======================================================

    case MOVE:

      moveForward();

      if (!pickSent && !armBusy) {

        link.println("PICK");

        pickSent = true;
      }

      if (armBusy) {

        state = ARM_LOCK;

        stopMotors();
      }

      else if (obs > 0 && obs <= STOP_DIST) {

        state = OBSTACLE_STOP;

        stateStart = now;

        stopMotors();
      }

      else if (dry || wet) {

        state = BIN_FULL;

        stopMotors();
      }

      else if (now - moveStart > 20000) {

        state = AUTO_WAIT;

        stateStart = now;

        stopMotors();
      }

      break;

    // ======================================================
    // OBSTACLE STOP STATE
    // ======================================================

    case OBSTACLE_STOP:

      if (now - stateStart > 10000) {

        state = MOVE;
      }

      break;

    //  ======================================================
    // AUTO WAIT STATE
    // ======================================================

    case AUTO_WAIT:

      if (now - stateStart > 10000) {

        moveStart = millis();

        state = MOVE;
      }

      break;

    // ======================================================
    // BIN FULL STATE
    // ======================================================

    case BIN_FULL:

      stopMotors();

      Serial.println("BIN FULL");

      break;

    // ======================================================
    // ARM LOCK STATE
    // ======================================================

    case ARM_LOCK:

      stopMotors();

      Serial.println("WAIT ARM");

      break;
  }

  delay(100);
}
