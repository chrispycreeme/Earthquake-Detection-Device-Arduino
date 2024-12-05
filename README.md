# Earthquake Detection and Relay Control System

## Overview
This project uses an MPU6050 accelerometer and a vibration sensor to detect potential earthquake-like movements, activating a relay when significant movement is detected.

## Features
- Vibration detection with analog sensor
- Movement tracking via MPU6050 accelerometer
- Configurable sensitivity thresholds
- Automated relay activation

## Code Modification Guide

### 1. Vibration Sensor Threshold
- **Variable**: `vibrationThreshold`
- **Default**: `200`
- **Modification Example**:
    ```cpp
    int vibrationThreshold = 250; // Increases sensitivity
    ```

### 2. MPU6050 Sensitivity
- **Variable**: `accelThreshold`
- **Default**: `1.5`
- **Modification Example**:
    ```cpp
    float accelThreshold = 2.0; // Reduces sensitivity
    ```

### 3. Confidence Cycles
- **Variable**: `confidenceCycles`
- **Default Trigger**: `5`
- **Modification Example**:
    ```cpp
    if (confidenceCycles >= 3) { // Faster relay triggering
    ```

### 4. Relay Activation Duration
- **Default Duration**:
    ```cpp
    delay(2000); // Milliseconds
    ```
- **Modification Example**:
    ```cpp
    delay(3000); // 3-second activation
    ```

### 5. Detection Delay Period
- **Variable**: `delayPeriod_ms`
- **Default**: `5000` milliseconds
- **Modification Example**:
    ```cpp
    unsigned long delayPeriod_ms = 10000; // 10-second delay
    ```

## System Requirements
- Microcontroller with I2C support
- MPU6050 accelerometer
- Analog vibration sensor
- Relay module

## Important Considerations
1. Verify I2C pin connections
2. Check relay module specifications
3. Use Serial Monitor for debugging
4. Calibrate sensor thresholds

## License
Open-source. Modify and distribute freely.
