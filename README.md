# YAN Sumobot

DIY 1 kg Sumobot using Arduino Uno.

## Features

- RC Bluetooth control
- Autonomous mode
- OLED display
- Motor testing
- Adjustable motor speed
- HC-06 Bluetooth module

## Hardware

- Arduino Uno
- L298N Motor Driver
- 2x JGA25 280 RPM Motors
- HC-06 Bluetooth Module
- 0.96" I2C OLED
- Push Buttons
- 3S Battery

## Pin Configuration
SUMOBOT PIN CONFIGURATION

L298 MOTOR DRIVER
ENA - D9
ENB - D6
IN1 - D2
IN2 - D3
IN3 - D4
IN4 - D5

12V MOTOR TO L298N DRIVER
LEFT:
(+) - OUT4
(-) - OUT3
RIGHT:
(+) - OUT1
(-) - OUT2

HC-SR04
FRONT: TRIG D10, ECHO D11
LEFT:  TRIG D8,  ECHO D7
RIGHT: TRIG D12, ECHO D13

IR
RIGHT - A1
LEFT  - A0

BUTTONS
SELECT - A2
START  - A3

OLED
SDA - A4
SCL - A5

HC-06
RXD - TX
TXD - RX
