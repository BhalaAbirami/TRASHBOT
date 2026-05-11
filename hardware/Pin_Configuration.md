# Pin Connections – TRASHBOT

---

# Master Controller (Arduino Uno)

## Motor Driver Connections (L298N)

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| D2 | IN1 | Left Motor Direction Control |
| D3 | IN2 | Left Motor Direction Control |
| D6 | ENA | Left Motor Speed Control (PWM) |
| D4 | IN3 | Right Motor Direction Control |
| D5 | IN4 | Right Motor Direction Control |
| D11 | ENB | Right Motor Speed Control (PWM) |

---

## Front Ultrasonic Sensor

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| D9 | TRIG | Obstacle Detection Trigger |
| D10 | ECHO | Obstacle Detection Echo |

---

## Dry Bin Ultrasonic Sensor

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| A0 | TRIG | Dry Bin Level Detection Trigger |
| A1 | ECHO | Dry Bin Level Detection Echo |

---

## Wet Bin Ultrasonic Sensor

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| A2 | TRIG | Wet Bin Level Detection Trigger |
| A3 | ECHO | Wet Bin Level Detection Echo |

---

## UART Communication with Slave Controller

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| D7 | Slave TX | SoftwareSerial RX |
| D8 | Slave RX | SoftwareSerial TX |

---

# Slave Controller (Arduino Uno)

## Servo Motor Connections

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| D5 | Base Servo | Base Rotation |
| D6 | Shoulder Servo | Arm Lifting Movement |
| D9 | Elbow Servo | Arm Extension Movement |
| D10 | Gripper Servo | Waste Gripping |
| D3 | Flipper Servo | Wet/Dry Waste Sorting |

---

## Wet Sensor Connection

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| D4 | Wet Sensor Output | Wet/Dry Waste Detection |

---

## UART Communication with Master Controller

| Arduino Pin | Connected To | Purpose |
|-------------|--------------|---------|
| D7 | Master TX | SoftwareSerial RX |
| D8 | Master RX | SoftwareSerial TX |

---


# Important Notes

- Common ground must be shared between both Arduino boards, motor driver, and external power supply.
- Servo motors should use external regulated power supply to avoid Arduino reset issues.
- UART communication requires TX ↔ RX cross connection between master and slave controllers.
- Disconnect serial communication pins during code upload if upload issues occur.
