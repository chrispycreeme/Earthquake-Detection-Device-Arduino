# Earthquake Detection Device (EDD) for Arduino

A reliable Arduino-based earthquake detection system using MPU6050 accelerometer and vibration sensors for early warning.

## Features

- Dual-sensor validation using accelerometer and vibration detection
- Configurable sensitivity thresholds for X, Y, and Z axes
- Confidence-based triggering system to prevent false alarms
- Relay output for connecting warning systems or emergency controls
- Serial monitoring for real-time movement data

## Hardware Requirements

- Arduino board
- MPU6050 accelerometer
- Vibration sensor
- Relay module
- Jumper wires

## Pin Configuration

- Vibration Sensor: Pin 2
- Relay Output: Pin 3
- MPU6050: I2C connection (SDA/SCL)

## Installation

1. Install required libraries:
   ```
   #include <Wire.h>
   #include <MPU6050.h>
   ```

2. Connect components according to pin configuration
3. Upload code to Arduino board

## Configuration

Adjust detection sensitivity via `EarthquakeSettings` struct:

```cpp
float accelXThreshold = 2.0;  // Side-to-side sensitivity
float accelYThreshold = 2.0;  // Front-to-back sensitivity
float accelZThreshold = 2.0;  // Up-down sensitivity
int requiredConfidenceCycles = 3;  // Detection cycles needed
int cooldownPeriod = 1000;  // Milliseconds between checks
```

## How It Works

1. System monitors both accelerometer and vibration sensor data
2. Movement must exceed thresholds on both sensors
3. Required number of confidence cycles must be met
4. Alarm triggers through relay when earthquake is confirmed
5. System enters cooldown period before next detection cycle

## Serial Output

Monitor earthquake detection in real-time through Serial Monitor (9600 baud):
- Initialization status
- Movement detection data
- Alarm triggers
- Setting updates
