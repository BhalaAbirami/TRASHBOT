# Working Mechanism – TRASHBOT

## Overview

TRASHBOT is an autonomous waste detection, collection, and segregation system designed using a dual-controller architecture. The system performs autonomous navigation, waste pickup, wet/dry segregation, and bin level monitoring using FSM-based Embedded C logic.

The system is divided into two controllers:

- Master Controller
  - Handles robot movement
  - Obstacle detection
  - Bin monitoring
  - Task coordination

- Slave Controller
  - Handles robotic arm operation
  - Waste pickup
  - Waste drop
  - Wet/Dry segregation

Communication between both controllers is performed using UART-based SoftwareSerial communication.

---

# System Operation

## 1. Autonomous Navigation

The master controller continuously drives the robot forward using differential drive DC motors controlled through the L298N motor driver.

An ultrasonic sensor mounted at the front detects obstacles during navigation.

If an obstacle is detected within the predefined threshold distance:
- The robot stops
- FSM transitions to obstacle stop state
- Movement resumes after the waiting period

---

## 2. Waste Detection and Pickup Trigger

During movement, the master controller sends a pickup command to the slave controller when the robotic arm is available.

The slave controller receives the command through UART communication and initiates the waste pickup sequence.

The master controller temporarily locks movement during arm operation to ensure stable pickup operation.

---

## 3. Robotic Arm Pickup Operation

The robotic arm uses multiple servo motors to perform synchronized movement.

Pickup sequence:
1. Arm moves to pickup position
2. Gripper closes to hold waste
3. Arm lifts the waste object
4. Arm moves to drop position

Smooth synchronized movement is implemented for stable arm operation.

---

## 4. Wet/Dry Waste Segregation

After pickup, the wet sensor detects the waste type.

Based on sensor status:
- Wet waste → Flipper rotates toward wet bin
- Dry waste → Flipper rotates toward dry bin

The flipper servo then returns to the center position after sorting operation completes.

---

## 5. Return to Home Position

After waste segregation:
- Arm returns to home position
- Slave controller sends ARM_DONE message to master controller
- Master controller resumes autonomous movement

---

## 6. Bin Level Monitoring

Separate ultrasonic sensors continuously monitor wet and dry bin levels.

If waste level reaches the threshold distance:
- FSM transitions to BIN_FULL state
- Robot movement stops
- Further operation is prevented until manual clearing

---

# FSM-Based Control Logic

The system uses FSM (Finite State Machine) based logic for structured and reliable operation.

Master controller states:
- MOVE
- OBSTACLE_STOP
- AUTO_WAIT
- BIN_FULL
- ARM_LOCK

Slave controller states:
- IDLE
- PICK
- DROP
- SORT
- RETURN

FSM architecture improves:
- Task sequencing
- Fault isolation
- Safe-state handling
- System readability
- Modular debugging

---

# Communication Architecture

The system uses UART-based communication between controllers.

Messages used:
- PICK
- ARM_BUSY
- ARM_DONE

This enables synchronized coordination between navigation and robotic arm subsystems.

---

# Safety and Reliability Features

- Motor safe-stop during arm operation
- Obstacle-based automatic stopping
- Bin full protection logic
- Modular controller separation
- Structured FSM execution
- Timeout handling for ultrasonic sensing
