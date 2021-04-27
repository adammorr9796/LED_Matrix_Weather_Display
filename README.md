# LED Matrix Weather Display Project

This repository contains source files for a motion-activated device that can retrieve and display weather data from the internet. Four main hardware modules are being used in this project: two 8x8 green LED matrices, an ESP-01 WiFi module, and a motion sensor module. The main microcontroller for the project is the Atmel ATtiny84-20PU being programmed using In System Programming via an AVRISP MKII clone.

**NOTE:** This project is currently on-going and as such, only some functionality has been implemented thus far. Mainly, the I2C driver library has been created, and I have been able to display data to two LED matrices. Future commits will contain code for interfacing with the WiFi and motion sensor modules, along with other enhancements and functionality.

## Goals & Project Motivation

 1. Create a motion-activated device that can retrieve and display weather data from the internet.
 2. Regain familiarity with utilizing microcontroller interrupts for fast detection and reaction to internal and external hardware events.
 3. Gain knowledge of the Inter-Integrated Circuit and UART communication protocols and implement both protocols in a project.
 4. Design flow-charts and timing diagrams to properly plan and architect software structure.
 5. Continue to gain familiarity with register access level microcontroller programming in C.
 6. Continue to practice reading through data sheets and reference documents to understand the architecture of microcontroller and LED matrix controller IC hardware used in the project.
 7. Continue to practice debugging by using step-through techniques and checking of register values at regular intervals.
 8. Continue to practice using an oscilloscope to analyze data packets being sent by serial communication protocols.

## Files & Codebase

 - i2c.h
 - i2c.c
 - main.c

## Future Enhancements & Fixes

 - TODO

## Technical Documents & References Utilized

- [ATtiny84 User Guide](https://ww1.microchip.com/downloads/en/DeviceDoc/doc8006.pdf)

- [ATtiny84 Reference Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/8006S.pdf)

- [HT16K33 Controller Driver Chip Reference Document](https://www.holtek.com/documents/10179/116711/HT16K33v120.pdf)

- [Sparkfun I2C Tutorial](https://learn.sparkfun.com/tutorials/i2c/all)

