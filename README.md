# ECE411-Fall2020
Practicum Project: Safety Bar


## Executive Summary / Concept of Operations
- A brief summary of your device. What does it do? Why? Who uses it? How do they use it? After reading this, the reader should have an understanding of your device and the context in which it's used.

This is a LED light bar system which will display power information from a distance of 300 feet. This system will be installed on a liquid fuel engine test stand and will display which sub-systems are recieving power and will display data on which individual valves are receiving power during the different engine firing stages. 

## Brief Market Analysis
- Who are the intended customers?
- What is the competition? Why is your product different?
- What price will do you think you can sell this for, and why? (keep it short, we're engineers, not marketing researchers)

This project can be adapted for a system with up to 9 inputs to display the power states of the lines. This is a safety and data sub-system for ensuring that all lines are powered as they should be within the purposes of the whole system. The Safety Bar would cost less than $200 if brought to market; this includes a rough estimate of component cost and assembly.

## Requirements
- Requirements are: Abstract, Verifiable, Unambiguous, Traceable, Realistic○Think "10,000 ft" view of what is required. E.g., "must be portable", "must measure voltagembut should measure current", must last 1 hour, etc.
- Remember, you're defining the requirements, not the solution.
- Feel free to use either "must" or "must, should, may"

.

.

.

- Must be visible from 300 feet (91.5 meters).
- Must be able to take at least 9 power line (valve) inputs.
- Must be compatible with a 24V input to power the system.
- Should display all possible combinations of power states.
- Should cost less than $200 total.
- May have data backup on the microcontroller.

## System Architecture
- This is your proposed design, from a high level.
- Create a Level 1 block diagram: one block per major section (e.g., processor, input, output,power supply, etc) with interconnections in between them. Label all interconnections. Label all blocks. If it makes sense to put the part number in the block, go ahead.

## Design Specification
- OK, now put in the technical details on the design.
- A list of bullet points is fine here: What is the sensor? Processor? Actuator? Power? Mechanical design? Firmware? Arduino or no? Development environment
