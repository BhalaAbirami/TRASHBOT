# TRASHBOT
Autonomous Waste Detection, Collection and Segregation System

## Overview
TRASHBOT is an embedded robotic system designed for autonomous waste detection, collection, and segregation using distributed microcontroller based control architecture.

## Key Features
- State machine based task execution
- Master-Slave control architecture
- Dual microcontroller coordination
- Autonomous navigation and object detection
- Ultrasonic sensor based waste detection
- 4 DOF robotic arm control
- Servo driven waste segregation mechanism
- Embedded C based firmware implementation
- Real-time actuator coordination

## System Architecture
- State machine based control logic used for operation sequencing
- Master controller handles movement, waste detection, and task decision making
- Master controller sends commands to slave controller for pickup and segregation operations
- Slave controller controls robotic arm and executes pickup sequence
- Servo motors used for robotic arm movement, gripping operations for pick-drop and flipping mechanism for waste sorting 
- Ultrasonic sensor used for waste object detection and bin level indication 
- Embedded C based logic used for sensor and actuator control
- Dual microcontroller architecture used for separating Master and Slave

## Working Flow
1. Autonomous navigation initiated
2. Waste object detected using ultrasonic sensing
3. Pickup sequence triggered through robotic arm controller
4. Object transferred to segregation mechanism
5. Waste sorted into designated sections(waste type detected and servo based sorting)
6. The Ultrasonic sensors in the bin detects the bin fill if full stops the bot

## Repository Structure
- `firmware/` → Embedded control logic
- `hardware/` → BOM and architecture diagrams
- `documentation/` → System operation details
- `images/` → operational sequence representation 
 
