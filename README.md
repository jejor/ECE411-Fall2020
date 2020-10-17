# ECE411-Fall2020
Practicum Project: Safety Bar


## Executive Summary / Concept of Operations

This is a LED light bar system which will display power information from a distance of 300 feet. This system will be installed on a liquid fuel engine test stand and will display which sub-systems are receiving power and will display data on which individual valves are receiving power during the different engine firing stages. 

## Brief Market Analysis

This project can be adapted for a system with up to 9 inputs to display the power states of the lines. This is a safety and data sub-system for ensuring that all lines are powered as they should be within the purposes of the whole system. The Safety Bar would cost less than $200 if brought to market; this includes a rough estimate of component cost and assembly.

## Requirements

- Must be visible from 300 feet (91.5 meters).
- Must be able to take at least 9 power line (valve) inputs.
- Must be compatible with a 24V input to power the system.
- Should display all possible combinations of power states.
- Should cost less than $200 total.
- Should compare State Machine to the current valve states.
- Should have a battery backup with 1 hour of backup power.
- Should be protected from the elements within an enclosure.
- Should have thermal distribution system.
- May have data backup on the micro-controller.
- May be enclosed in an explosion proof box.
- May have rechargeable battery.

## System Architecture

#### The full system architecture for the Test Stand Automation & Regulation system with the Safety Bar sub-project (green circle):
![TSAR System Architecture Practicum](https://github.com/jejor/ECE411-Fall2020/blob/main/images/sysArch-Practicum.png?raw=true)

#### Safety Bar Level 1 Block Diagram:
![Level 1 Block Diagram](https://github.com/jejor/ECE411-Fall2020/blob/main/images/level1.png?raw=true)

#### Safety Bar Level 2 Block Diagram:
![Level 2 Block Diagram](https://github.com/jejor/ECE411-Fall2020/blob/main/images/level2.png?raw=true)

## Design Specification

There are a total of 9 valves which states need to be compared to the software control state machine. This system will verify that the valves are responding to the software commands. To do this we will use sensors on each 9 valves, process the data on a micro-controller, compare the physical valve states to the expected valve states and display any relevant information to the user in the form of LED display codes. The LED bar will display the current state the system is in, if there is an error or if the physical state does not match the expected state, the LED bar will flash and display which valve is causing the error. 

- Sensor: Hall Effect
- Processor: STM32F0
- Actuator: LEDs
- Power: POE+ with backup battery

`Software Pseudocode:`

`LOOP:`

&nbsp;&nbsp;&nbsp;&nbsp; `READ: hall effect sensors`

&nbsp;&nbsp;&nbsp;&nbsp; `READ: STM32F0 state command`

&nbsp;&nbsp;&nbsp;&nbsp; `COMPARE: hall effect sensor data to state command`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; `IF: doesn’t match`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; `Sound Buzzer`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; `Display binary value for which valve is causing the error`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; `IF: does match`

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; `Display current state in binary`

&nbsp;&nbsp;&nbsp;&nbsp; `RECORD: data to SD card`

