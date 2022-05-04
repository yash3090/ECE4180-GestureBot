# ECE4180-GestureBot
ECE 4180 Spring'22 </br>
Team: Yash Saraiya and Siddhanth Vashista </br>

## Introduction
GestureBot is a pi based robot, that is controlled through an accelerometer and mbed attached to a glove. The glove allows for additional features such as speed and headlights control. (add)

## Hardware
 - Mbed
 - Button x2
 - MMA8452 Accelerometer
 - Raspberry Pi 4
 - Huzzah ESP8266 WiFi module (Adafruit)
 - DC Motor ROB-13302 x2
 - Dual H-Bridge TB6612FNG
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
Pin Connection for Raspberry Pi 4, Hbridge Motor Driver and external barrel jack.



## Communication between Glove and Pi
The Huzzah ESP8266 WiFi module on the Mbed was used to set up a webpage on a local IP address using hotspot. The Pi was then also connected to the same WiFi hotspot. The value on the webpage was the command, which is a number ranging from 0-29. This number corresponded to a particular instruction for the Pi on what action to perform such as move forward, backward, left, right, speed control, and light control. A button on the webpage had to be constantly pressed in order to update the command value on the page. The code on the Pi used selenium to scrape the page, read the command, and then press the button every to refresh the command value. This was done every 0.05s.

![Screenshot 2022-05-03 at 1 03 39 PM](https://user-images.githubusercontent.com/60718509/166503509-346c42ac-1b97-444a-aa96-0cd3432ce20e.png)

The image shown above is the webpage created by the Huzzah WiFi module on the Mbed and the one that is scraped by the Pi for command information


