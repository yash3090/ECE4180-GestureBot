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
Pin Connections for Mbed, Accelerometer, Huzzah Wifi Module, and Barrel Jack

| Mbed  | Accelerometer | Huzzah Wifi module | Barrel Jack |
| ------------- | ------------- | ------------- | ------------- |
| VOUT | 3.3V |   |    |
| PIN 9  | SDA  |   |    |
| PIN 10 | SCL  |   |  |
| GND  | GND  | GND  | - |
|   |  | V+ | + |
| PIN 27 |   | TX  | |
| PIN 28  |   | RX  |  |

_Note: An external 5v power supply was connected to the barrel jack to support the HUzzah wifi module as it drained too much current from the Mbed_

The buttons were used as Pull-Ups and were connected to PIN 16 and 17 for light and speed control respectively
## Pi Robot SetUp

## Communication between Glove and Pi
The Huzzah ESP8266 WiFi module on the Mbed was used to set up a webpage on a local IP address using hotspot. The Pi was then also connected to the same WiFi hotspot. The value on the webpage was the command, which is a number ranging from 0-29. This number corresponded to a particular instruction for the Pi on what action to perform such as move forward, backward, left, right, speed control, and light control. A button on the webpage had to be constantly pressed in order to update the command value on the page. The code on the Pi used selenium to scrape the page, read the command, and then press the button every to refresh the command value. This was done every 0.05s.



