# STM32F446RE Bare-Metal HC-SR04 Ultrasonic Distance Measurement

A low-level, bare-metal C implementation for measuring distance using the HC-SR04 ultrasonic sensor on an STM32F446RE (ARM Cortex-M4) development board, with real-time data visualization via UART.

## 🚀 Features
* *Bare-Metal Register Programming:* Direct manipulation of RCC, GPIO, and TIM2 registers without high-level hardware abstraction libraries (HAL/LL).
* *Timer Input Capture:* Utilizes Timer 2 (Channel 2) to capture precise pulse widths (Rising and Falling edges) of the HC-SR04 Echo pin.
* *Overflow Handling:* Robust logic to handle timer roll-overs (0xFFFF) during long-distance measurements.
* *UART Serial Communication:* Formats and transmits distance data (float) to a serial terminal using sprintf and custom UART drivers.

## 📌 Hardware Connections
| HC-SR04 Pin | STM32F446RE Pin | Description |
| :--- | :--- | :--- |
| *VCC* | 5V | Power Supply |
| *Trig* | PA0 | Trigger Pulse Input (GPIO Output) |
| *Echo* | PA1 (TIM2_CH2) | Echo Pulse Capture (Alternate Function) |
| *GND* | GND | Ground |

## 🛠️ Toolchain & Configuration
* *IDE:* STM32CubeIDE
* *Language:* C (GNU11)
* *Linker Flag:* -u _printf_float (Required for floating-point support in sprintf)

## 💻 Code Overview
* *PIN_Set()*: Configures GPIO modes and alternate functions for PA0 and PA1.
* *Timer2_InIt()*: Sets up Timer 2 prescaler, auto-reload register, and Input Capture mode.
* *Read_Distance()*: Triggers the sensor, captures start/end timestamps via Input Capture, handles timer overflow, and calculates the distance in centimeters.
*
