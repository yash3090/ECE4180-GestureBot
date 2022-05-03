# ECE4180-GestureBot
ECE 4180 Spring'22 </br>
Team: Yash Saraiya and Siddhanth Vashista </br>

## Introduction
GestureBot is a pi based robot, that is controlled through an accelerometer and mbed attached to a glove. The glove allows for additional features such as speed and headlights control. (add)

## Hardware
 - Mbed
 - Button x2
 - MMA8452 Accelerometer
 - Raspberry Pi (add model)
 - Huzzah ESP8266 WiFi module (Adafruit)
 - Motor x2
 - Dual H-Bridge (add model)
 - Barrel Jack

## Glove SetUp
Pin Connections for Mbed, Accelerometer, Huzzah Wifi Module, Buttons and Barrel Jack

| Mbed  | Accelerometer | Huzzah Wifi module | Barrel Jack |
| ------------- | ------------- | ------------- | ------------- |
| VOUT | 3.3V |   |    |
| PIN 9  | SDA  |   |    |
| PIN 10 | SCL  |   |  |
| GND  | GND  | GND  | - |
|   |  | V+ | + |
| PIN 27 |   | TX  | |
| PIN 28  |   | RX  |  |


## Pi Robot SetUp

## Communication between Glove and Pi
