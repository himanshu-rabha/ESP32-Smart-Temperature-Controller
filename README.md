# ESP32 Smart Temperature Controller

## Overview

An IoT-based smart temperature control system using ESP32, DS18B20 temperature sensor, relay control, and a 500W immersion heater.

The system automatically controls heating based on temperature thresholds and provides monitoring through both Blynk IoT and a custom web dashboard.

---

## Features

- DS18B20 Temperature Sensing
- Automatic Heater Control
- Adjustable Setpoint
- Relay-Based Heater Switching
- Emergency Stop
- Overtemperature Protection
- Blynk IoT Dashboard
- Local Web Dashboard
- WiFi Connectivity

---

## Hardware Used

- ESP32 DevKit V1
- DS18B20 Waterproof Temperature Sensor
- 2N2222 Transistor
- 1kΩ Resistor
- 4.7kΩ Pull-up Resistor
- 5V Relay Module
- 500W Immersion Heater
- Breadboard
- Jumper Wires

---

## Software Used

- Arduino IDE
- Blynk IoT
- HTML/CSS/JavaScript
- ESP32 WebServer

---

## System Architecture

DS18B20
↓
ESP32
↓
2N2222 Driver
↓
Relay Module
↓
500W Heater

---

## Control Logic

Temperature < Setpoint - 2°C
→ Heater ON

Temperature > Setpoint
→ Heater OFF

Sensor Failure
→ Heater OFF

Emergency Stop
→ Heater OFF

Temperature > 70°C
→ Heater OFF

---

## Project Versions

### V1.0
- DS18B20 Temperature Reading

### V1.1
- Relay Control

### V1.2
- Automatic Heater Controller

### V2.0
- Blynk IoT Integration

### V2.5
- Local Web Dashboard

---

## Future Improvements

- OLED Display
- Data Logging
- MQTT Integration
- Node-RED Dashboard
- Grafana Analytics
- PID Temperature Control

---

## Author

B.Tech Instrumentation Engineering

ESP32 Smart Temperature Controller Project
