# Aquas-Sampler

Algae water sampling system with automated container management and diagnostic capabilities.

## Overview

This project contains two Arduino programs for a water sampling system:

1. **samplerValves.ino** - Main sampling program with automated container management
2. **samplerDiagnostic.ino** - Diagnostic tool for testing system components

## Hardware Requirements

### Components

-   Arduino Uno or compatible board
-   3x Solenoid valves (for sample containers)
-   1x Solenoid valve (for outflow/purge)
-   3x Water level sensors (FS-IR02B or similar)
-   1x DC pump (12V recommended)
-   1x Motor driver/relay module for pump control
-   1x Relay for triggering for sample sequence
-   1x (optional) relay for triggering force reset sequence

### Pin Connections

#### samplerValves.ino Pin Mapping

| Component            | Pin | Description                           |
| -------------------- | --- | ------------------------------------- |
| Container 1 Solenoid | 6   | Sample container 1 valve control      |
| Container 2 Solenoid | 7   | Sample container 2 valve control      |
| Container 3 Solenoid | 8   | Sample container 3 valve control      |
| Outflow Solenoid     | 9   | Purge/outflow valve control           |
| Container 1 Sensor   | 10  | Water level sensor for container 1    |
| Container 2 Sensor   | 11  | Water level sensor for container 2    |
| Container 3 Sensor   | 12  | Water level sensor for container 3    |
| Pump Control         | 5   | PWM control for pump speed            |
| Sample Trigger       | 2   | Digital input to trigger sampling     |
| Force Reset          | 3   | Digital input to reset all containers |

#### samplerDiagnostic.ino Pin Mapping

| Component            | Pin | Description                          |
| -------------------- | --- | ------------------------------------ |
| Container 1 Solenoid | 7   | Test solenoid 1                      |
| Container 2 Solenoid | 8   | Test solenoid 2                      |
| Container 3 Solenoid | 9   | Test solenoid 3                      |
| Container 1 Sensor   | 10  | Test water sensor 1                  |
| Container 2 Sensor   | 11  | Test water sensor 2                  |
| Container 3 Sensor   | 12  | Test water sensor 3                  |
| Outflow Solenoid     | 6   | Test purge solenoid                  |
| Pump Control         | 5   | Test pump control                    |
| Sample Trigger       | 2   | Digital input (unused in diagnostic) |
| Water Sensor         | 7   | Additional water sensor test         |

## Installation and Setup

### 1. Hardware Assembly

1. Connect all solenoid valves to their respective pins
2. Connect water level sensors to their designated pins
3. Connect the pump through a motor driver to pin 5
4. Connect the outflow solenoid to pin 9 (samplerValves) or pin 6 (samplerDiagnostic)
5. Connect push buttons to trigger and reset pins
6. Ensure proper power supply for all components

**⚠️ CRITICAL: Pin Alignment**
When connecting the hardware, ensure that each container's solenoid valve is paired with its corresponding water level sensor:

| Container Solenoid Pin | Water Sensor Pin | Container Number |
| ---------------------- | ---------------- | ---------------- |
| 6                      | 10               | Container 1      |
| 7                      | 11               | Container 2      |
| 8                      | 12               | Container 3      |

This alignment is essential for proper operation. When the system opens solenoid pin 6, it must read from sensor pin 10 to detect when that container is full. The same applies for containers 2 and 3.

**Assembly Tip**: Label your wires clearly during assembly to avoid confusion. For example, mark the wire going to solenoid pin 6 as "Container 1 Solenoid" and the wire going to sensor pin 10 as "Container 1 Sensor".

### 2. Software Installation

1. Open Arduino IDE
2. Load the desired program (`samplerValves.ino` or `samplerDiagnostic.ino`)
3. Verify the pin connections match your hardware setup
4. Upload the code to your Arduino board

## Usage Instructions

### samplerValves.ino - Main Sampling Program

This is the primary program for automated water sampling.

#### Features

-   **Sequential Sampling**: Automatically fills containers one by one
-   **Sensor Monitoring**: Uses water level sensors to detect when containers are full
-   **Safety Timeout**: 20-second timeout prevents infinite sampling
-   **Purge Function**: Clears lines between samples
-   **Force Reset**: Resets all container status

#### Operation

1. **Automatic Sampling**: Connect a trigger signal to pin 2 (HIGH = start sampling)
2. **Manual Reset**: Connect a button to pin 3 (HIGH = reset all containers)
3. **Serial Monitoring**: Open Serial Monitor at 9600 baud to view status messages

#### Sampling Process

1. System checks if current container is already filled
2. Opens appropriate solenoid valve
3. Activates pump at 200/255 speed
4. Monitors water level sensor until container fills
5. Closes valve and stops pump
6. Moves to next container
7. Repeats until all containers are filled

#### Serial Output Examples

```
Reading trigger pin: 0
Reading trigger pin: 1
Sampling container 1
Sampling for container 1 COMPLETED.
Sample collected in container 1
```

### samplerDiagnostic.ino - Diagnostic Tool

This program helps test and verify all system components.

#### Features

-   **Solenoid Testing**: Tests all solenoid valves individually
-   **Sensor Testing**: Reads all water level sensors
-   **Pump Testing**: Tests pump operation
-   **Component Verification**: Ensures all hardware is working correctly

#### Operation

1. Upload the diagnostic program
2. Open Serial Monitor at 9600 baud
3. The program will automatically run tests
4. Monitor output for component status

#### Test Sequence

1. **Sensor Tests**: Reads all water level sensors
2. **Solenoid Tests**: Activates each solenoid for 2 seconds
3. **Pump Test**: Runs pump at 150/255 speed for 2 seconds

#### Serial Output Examples

```
Starting diagnostic:
Testing pin 10 water level sensor...
1
Testing pin 11 water level sensor...
0
Testing pin 12 water level sensor...
1
```

## Troubleshooting

### Common Issues

1. **Solenoid Not Responding**

    - Check power supply voltage
    - Verify pin connections
    - Test with diagnostic program

2. **Water Sensor Not Working**

    - Ensure proper wiring (HIGH = empty, LOW = full)
    - Check sensor alignment
    - Verify sensor power supply

3. **Pump Not Running**

    - Check motor driver connections
    - Verify PWM pin output
    - Test with diagnostic program

4. **Sampling Timeout**
    - Check for blockages in tubing
    - Verify pump is working
    - Check solenoid valve operation

### Diagnostic Steps

1. Run `samplerDiagnostic.ino` first
2. Verify all components respond correctly
3. Check Serial Monitor for error messages
4. Test individual components if needed

## Configuration

### Adjustable Parameters

#### samplerValves.ino

-   `PUMP_SPEED`: Change from 200 to adjust pump power
-   `TIMEOUT`: Modify 20000ms timeout for sampling
-   `DEBOUNCE_DELAY`: Adjust 1000ms debounce timing

#### samplerDiagnostic.ino

-   `TEST_DURATION`: Modify 2000ms test duration
-   `PUMP_TEST_SPEED`: Change from 150 to adjust test pump speed

## Safety Notes

-   Always disconnect power before making wiring changes
-   Ensure proper water containment to prevent spills
-   Monitor system during operation
-   Use appropriate power supplies for all components
-   Test with diagnostic program before running main sampling

## File Structure

```
aquas-sampler/
├── README.md                    # This file
├── samplerValves/
│   └── samplerValves.ino       # Main sampling program
└── samplerDiagnostic/
    └── samplerDiagnostic.ino   # Diagnostic tool
```

## Support

For issues or questions:

1. Run the diagnostic program first
2. Check Serial Monitor output
3. Verify all pin connections
4. Test individual components
