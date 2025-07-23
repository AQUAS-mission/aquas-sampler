// Pin definitions
const int CONTAINER_PINS[3] = {7, 8, 9};
const int SENSOR_PINS[3] = {10, 11, 12};
const int OUTFLOW_SOLENOID_PIN = 6;   // Pin to control outflow solenoid
const int PUMP_PIN = 5;               // PWM pin for pump speed
const int SAMPLE_TRIGGER_PIN = 2;     // Digital pin used to trigger sampling
const int WATER_SENSOR_PIN = 7;       // Pin for the water sensor FS-IR02B

// Container status struct
struct ContainerStatus {
  int container_pin;
  int sensor_pin;
  bool is_filled;
};

ContainerStatus containers[3];
int currentSampleContainer = 0;

void setPumpSpeed(int value) {
  value = constrain(value, 0, 255);
  analogWrite(PUMP_PIN, value);
}

void testSolenoid(int pinNum) {
    Serial.print("Testing pin ");
    Serial.print(pinNum);
    Serial.println(" solenoid...");
    digitalWrite(pinNum, HIGH);
    delay(2000);
    digitalWrite(pinNum, LOW);
    delay(2000);
}

void runAllSolenoids() {
  // First, run tests on all solenoids. 
  for (int containerPin : CONTAINER_PINS) {
    testSolenoid(containerPin);
  }

  // Then, run purge solenoid
  testSolenoid(OUTFLOW_SOLENOID_PIN);
}

void testSensors() {
  // Then, run sensor readings test
  for (int sensorPin : SENSOR_PINS) {
    Serial.print("Testing pin ");
    Serial.print(sensorPin);
    Serial.println(" water level sensor...");
    Serial.println(digitalRead(sensorPin));
  }
}

void testPump() {
// Then, test pump
  Serial.println("Testing pump..."); 
  setPumpSpeed(150);
  delay(2000);
  setPumpSpeed(0);
  
}

void setup() {
  Serial.begin(9600);
  
  // Initialize container and sensor pins
  for (int i = 0; i < 3; i++) {
    containers[i].container_pin = CONTAINER_PINS[i];
    containers[i].sensor_pin = SENSOR_PINS[i];
    containers[i].is_filled = false;
    pinMode(containers[i].container_pin, OUTPUT);
    pinMode(containers[i].sensor_pin, INPUT);
  }

  // Initialize solenoid and pump pins
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(OUTFLOW_SOLENOID_PIN, OUTPUT);
  pinMode(SAMPLE_TRIGGER_PIN, INPUT);
  pinMode(WATER_SENSOR_PIN, INPUT);

  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(OUTFLOW_SOLENOID_PIN, LOW);
}

void loop() {
  Serial.println("Starting diagnostic: ");
  delay(2000);

  testSensors();

}
