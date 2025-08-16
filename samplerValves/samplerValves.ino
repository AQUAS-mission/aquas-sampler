// Pin definitions
const int CONTAINER_PINS[3] = {10, 11, 12};
const int OUTFLOW_SOLENOID_PIN = 13;   // Pin to control outflow solenoid
const int SENSOR_PINS[3] = {7, 8, 9};
const int PUMP_PIN = 2;               // Digital pin for pump control
const int SAMPLE_TRIGGER_PIN = 6;     // Digital pin used to trigger sampling
// const int FORCE_RESET_PIN = 3;    

// Container status struct
struct ContainerStatus {
  int container_pin;
  int sensor_pin;
  bool is_filled;
};

ContainerStatus containers[3];
int currentSampleContainer = 0;  // Tracks which container to fill

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
  // pinMode(FORCE_RESET_PIN, INPUT);
  // pinMode(TRIGGER_PIN_TESTER, OUTPUT);

  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(OUTFLOW_SOLENOID_PIN, LOW);
}

// Turn pump on/off
void setPump(bool on) {
  digitalWrite(PUMP_PIN, on ? HIGH : LOW);
}

// Purge system: flush lines for specified duration (ms)
void purge(unsigned long duration) {
  // Close all sample solenoids
  for (int i = 0; i < 3; i++) {
    digitalWrite(containers[i].container_pin, LOW);
  }

  Serial.println("Purging begins...");

  // Open outflow solenoid
  digitalWrite(OUTFLOW_SOLENOID_PIN, HIGH);
  setPump(true);

  delay(duration);

  // Stop pump and close outflow
  setPump(false);
  digitalWrite(OUTFLOW_SOLENOID_PIN, LOW);
  Serial.println("Purging finished.");
}

// Force reset all container status
void forceReset() {
  for (int i = 0; i < 3; i++) {
    containers[i].is_filled = false;
  }
  currentSampleContainer = 0;
  Serial.println("Force reset: All container status reset to false");
}

// Sample into the current container (guardrails included)
void sample() {
  if (currentSampleContainer >= 3) {
    Serial.println("All containers filled.");
    return;
  }

  ContainerStatus &csc = containers[currentSampleContainer];



  // Check if already filled
  if (digitalRead(csc.sensor_pin) == LOW) {
    Serial.print("Container "); Serial.print(currentSampleContainer + 1);
    Serial.println(" is already filled.");
    csc.is_filled = true;
    currentSampleContainer++;
    return;
  }

  // Clear to sample: purge first to clean pipeline.
  purge(10000);


  // Begin sampling
  digitalWrite(csc.container_pin, HIGH);  // Open solenoid
  setPump(true);

  Serial.print("Sampling container ");
  Serial.println(currentSampleContainer + 1);
  unsigned long startTime = millis();
  unsigned long timeout = 20000;  // 10 seconds safety timeout

  // Wait until the container is filled or timeout
  while (digitalRead(csc.sensor_pin) == HIGH) {
    if (millis() - startTime > timeout) {
      Serial.println("Sampling timeout: sensor did not trigger.");
      break;
    }
  }

  // Stop pump and close solenoid
  setPump(false);
  digitalWrite(csc.container_pin, LOW);
  Serial.print("Sampling for container ");
  Serial.print(currentSampleContainer + 1);
  Serial.println("COMPLETED.");

  // Mark container as filled and move to next
  if(digitalRead(csc.sensor_pin) == LOW){ // LOW = water detected, HIGH = no water
    csc.is_filled = true;
  }  
  currentSampleContainer++;

  Serial.print("Sample collected in container ");
  Serial.println(currentSampleContainer);
}

void loop() {
  // Sample when digital pin goes HIGH (rising edge logic could be added)
  Serial.print("Reading trigger pin: ");
  Serial.println(digitalRead(SAMPLE_TRIGGER_PIN));
  if (digitalRead(SAMPLE_TRIGGER_PIN) == HIGH) {
    sample();
    delay(1000); // Debounce delay
  }

  // Force reset when pin goes HIGH
  // if (digitalRead(FORCE_RESET_PIN) == HIGH) {
  //   forceReset();
  //   delay(1000); // Debounce delay
  // }

  // digitalWrite(TRIGGER_PIN_TESTER, HIGH);

} 
