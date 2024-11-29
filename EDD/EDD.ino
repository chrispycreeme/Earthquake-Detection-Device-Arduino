#include <Wire.h>
#include <MPU6050.h>

// Vibration sensor pin - defines input location for detecting ground movement
#define VIBRATION_SENSOR_PIN 2  

// Relay pin - output for triggering alarm/emergency system
#define RELAY_PIN 3             

// Earthquake detection configuration structure
// Correlates sensitivity settings and detection parameters
struct EarthquakeSettings {
  // Movement thresholds: define how much acceleration constitutes significant movement
  // Lower values = more sensitive, higher values = less sensitive
  // These correlate to determine what qualifies as potential earthquake motion
  float accelXThreshold = 2.0;  // Side-to-side movement sensitivity
  float accelYThreshold = 2.0;  // Front-to-back movement sensitivity
  float accelZThreshold = 2.0;  // Up-and-down movement sensitivity

  // Confidence cycle correlation:
  // Requires multiple detection cycles to confirm earthquake
  // Prevents false alarms by demanding consistent movement detection
  int requiredConfidenceCycles = 3;  

  // Cooldown period prevents system from continuously retriggering
  // Correlates with detection timing to manage system responsiveness
  int cooldownPeriod = 1000;  // 1 second between detection attempts
};

// Global variables that correlate sensor data and detection logic
MPU6050 mpu;                    // Motion sensor for precise movement tracking
EarthquakeSettings settings;    // Detection configuration settings
unsigned long lastDetectionTime = 0;  // Tracks time since last earthquake detection

// Confidence tracking variables:
// Correlate to provide multi-sensor validation before alarm
// Must independently reach threshold to confirm earthquake
int accelerometerConfidence = 0;  // Tracks continuous accelerometer movement
int vibrationConfidence = 0;      // Tracks continuous vibration sensor activation

void setup() {
  // This runs once when the Arduino first starts up

  // Start talking to the computer so we can see what's happening
  Serial.begin(9600);

  // Tell the Arduino to start listening to our motion sensor
  Wire.begin();
  mpu.initialize();
  
  // Check if our motion sensor is working correctly
  if (!mpu.testConnection()) {
    Serial.println("Oops! Motion sensor is not working.");
    while (1); // Stop everything if sensor is broken
  }

  // Tell Arduino which pins do what
  pinMode(VIBRATION_SENSOR_PIN, INPUT);   // Listen for vibrations
  pinMode(RELAY_PIN, OUTPUT);             // Send out an alarm signal

  Serial.println("Earthquake Detector is Ready!");
}

// This function checks if we're moving too much
bool detectAccelerometerMovement() {
  // Get the current movement from our sensor
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert raw numbers to actual movement measurements
  float gx = ax / 16384.0;  // Side-to-side movement
  float gy = ay / 16384.0;  // Front-to-back movement
  float gz = az / 16384.0;  // Up-and-down movement

  // Are we moving more than our "too much" settings?
  bool movementDetected = 
    (abs(gx) > settings.accelXThreshold) ||
    (abs(gy) > settings.accelYThreshold) ||
    (abs(gz) > settings.accelZThreshold);

  // If we're moving a lot, print out how we're moving
  if (movementDetected) {
    Serial.print("Big Movement! X=");
    Serial.print(gx);
    Serial.print(" Y=");
    Serial.print(gy);
    Serial.print(" Z=");
    Serial.println(gz);
  }

  return movementDetected;
}

// This function checks if we feel any shaking
bool detectVibration() {
  // Read if the vibration sensor is feeling anything
  int vibrationValue = digitalRead(VIBRATION_SENSOR_PIN);

  if (vibrationValue == HIGH) {
    Serial.println("Something is shaking!");
  }

  return (vibrationValue == HIGH);
}

// What do we do when we think an earthquake is happening?
void triggerEarthquakeAlarm() {
  Serial.println("EARTHQUAKE CONFIRMED! TAKING ACTION!");
  digitalWrite(RELAY_PIN, HIGH);  // Turn on the alarm or emergency system
}

void loop() {
  // This runs over and over while the Arduino is on

  // Have enough seconds passed since our last earthquake check?
  if (millis() - lastDetectionTime > settings.cooldownPeriod) {
    // Check if we're moving and shaking
    bool accelerometerTriggered = detectAccelerometerMovement();
    bool vibrationTriggered = detectVibration();

    // Count how many times we've seen movement and vibration
    if (accelerometerTriggered) accelerometerConfidence++;
    if (vibrationTriggered) vibrationConfidence++;

    // Have we seen enough movement to be sure it's an earthquake?
    if (accelerometerConfidence >= settings.requiredConfidenceCycles &&
        vibrationConfidence >= settings.requiredConfidenceCycles) {
      triggerEarthquakeAlarm();

      // Reset our counters and remember when this happened
      lastDetectionTime = millis();
      accelerometerConfidence = 0;
      vibrationConfidence = 0;
    }

    // Make sure our confidence doesn't get too high
    accelerometerConfidence = min(accelerometerConfidence, 5);
    vibrationConfidence = min(vibrationConfidence, 5);
  }

  delay(100);  // Small pause to not overwork the system
}

// Optional: A way to change how sensitive our detector is
void updateEarthquakeSettings(
    float newXThreshold = -1,
    float newYThreshold = -1,
    float newZThreshold = -1,
    int newConfidenceCycles = -1) {
  
  // Update settings if new values are provided
  if (newXThreshold > 0) settings.accelXThreshold = newXThreshold;
  if (newYThreshold > 0) settings.accelYThreshold = newYThreshold;
  if (newZThreshold > 0) settings.accelZThreshold = newZThreshold;
  if (newConfidenceCycles > 0) settings.requiredConfidenceCycles = newConfidenceCycles;

  Serial.println("Earthquake Detector Settings Updated!");
}