#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define VIBRATION_PIN A5
#define RELAY_PIN 3

Adafruit_MPU6050 mpu;

int vibrationSensorReading;
int vibrationThreshold = 200;

unsigned int confidenceCycles = 0;
unsigned long lastDetectionTime = 0;
unsigned long delayPeriod_ms = 5000;

bool isVibrationMoved = false;
bool isMPUMoved = false;

float accelX, accelY, accelZ;
float accelThreshold = 1.5;

void setup() {
  Serial.begin(9600);
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  if (!mpu.begin()) {
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
}

void loop() {
  vibrationSensorReading = analogRead(VIBRATION_PIN);
  Serial.print("Vibration Sensor Reading: ");
  Serial.println(vibrationSensorReading);

  if (vibrationSensorReading >= vibrationThreshold) {
    isVibrationMoved = true;
    lastDetectionTime = millis();
    Serial.println("Vibration Detected!");
  } else {
    isVibrationMoved = false;
  }

  sensors_event_t event;
  mpu.getAccelerometerSensor()->getEvent(&event);
  accelX = event.acceleration.x;
  accelY = event.acceleration.y;
  accelZ = event.acceleration.z;

  Serial.print("Accel X: ");
  Serial.print(accelX);
  Serial.print(" Accel Y: ");
  Serial.print(accelY);
  Serial.print(" Accel Z: ");
  Serial.println(accelZ);

  if (abs(accelX) > accelThreshold || abs(accelY) > accelThreshold || abs(accelZ) > accelThreshold) {
    isMPUMoved = true;
    Serial.println("Significant movement detected by MPU6050!");
  } else {
    isMPUMoved = false;
  }

  if (isVibrationMoved && isMPUMoved) {
    confidenceCycles++;
    Serial.print("Confidence Cycles: ");
    Serial.println(confidenceCycles);
  }

  if (confidenceCycles >= 5) {
    Serial.println("Triggering Relay...");
    digitalWrite(RELAY_PIN, HIGH);
    delay(2000);
    digitalWrite(RELAY_PIN, LOW);
    confidenceCycles = 0;
    lastDetectionTime = millis();
  }

  if (millis() - lastDetectionTime > delayPeriod_ms) {
    confidenceCycles = 0;
  }

  delay(100);
}
