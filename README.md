# ECE4180-GestureBot
ECE 4180 Spring'22 </br>
Team: Yash Saraiya and Siddhanth Vashista </br>

## Introduction
GestureBot is a pi based robot, that is controlled through an accelerometer and mbed attached to a glove. The glove allows for additional features such as speed and headlights control. (add) The GestureBot is a Pi based smart robot, that is controlled through a “Smart Glove”, containing an MBED, accelerometer, and Huzzafruit WiFi module. The MBED takes in reading from the accelerometer then publishes the readings via a webpage (a number from 0-24) set up by the Huzzahfruit. This web server is then read by the Raspberry Pi on the car, which uses the number taken in to determine the command. The glove also contains 2 buttons for speed control and light control. 

#### Block Diagram of system
![Screenshot 2022-05-04 at 9 42 44 AM](https://user-images.githubusercontent.com/60718509/166694523-ae5a418f-7000-4c14-ae5e-04864e634a31.png)


## Hardware
 - Mbed
 - Button x2
 - MMA8452 Accelerometer
 - Raspberry Pi 4
 - Huzzah ESP8266 WiFi module (Adafruit)
 - Sparkfun DC Motor ROB-13302 x2
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

_Note: An external 5v power supply was connected to the barrel jack to support the Huzzah wifi module as it drained too much current from the Mbed_

![Screenshot 2022-05-04 at 9 54 27 AM](https://user-images.githubusercontent.com/60718509/166696278-1db492cc-2959-4105-999f-245e807798ea.png)

The buttons were used as Pull-Ups and were connected to PIN 16 and 17 for light and speed control respectively

## Pi Robot SetUp

Pin connections for Raspberry Pi 4, Dual Hbridge and Motors. 

|Raspberry Pi 4| Dual Hbridge TB6612FNG | Motors|Barrel Jack Power|
|-----|---------------------|-------|----------------|
||VM||+|
|1|VCC||
|9|GND||-|
||A01|+|
||A02|-|
||B02|-|
||B01|+|
|12|PWMA||
|6|AI2||
|5|AI1||
||STBY||+|
|23|BI1||
|24|BI2||
|18|PWMB||

![IMG-3042](https://user-images.githubusercontent.com/55064067/166702682-a21bafad-8e32-477b-a039-9f003d378ad2.jpg)
_Note: An external 5v battery power supply was connnected via barrel jack to support H-Bridge and motor drive._

## Software

### Setting up the Pi
Libraries needed:
- Rpi.GPIO
- Selenium
- webdrive_manager
- time

```Python
pip install selenium
pip install webdriver-manager
pip install RPi.GPIO
```

### Software State Diagram

![Untitled Diagram](https://user-images.githubusercontent.com/55064067/166749677-1c7c8d0d-549e-48f7-9694-245ffe9050ab.jpg)


### Communication between Glove and Pi
The Huzzah ESP8266 WiFi module on the Mbed was used to set up a webpage on a local IP address using hotspot. The Pi was then also connected to the same WiFi hotspot. The value on the webpage was the command, which is a number ranging from 0-29. This number corresponded to a particular instruction for the Pi on what action to perform such as move forward, backward, left, right, speed control, and light control. A button on the webpage has to be pressed in order to update the command value on the page. The code on the Pi used selenium to scrape the page, read the command, and then press the button to refresh the command value. This was done every 0.05s.

![Screenshot 2022-05-03 at 1 03 39 PM](https://user-images.githubusercontent.com/60718509/166503509-346c42ac-1b97-444a-aa96-0cd3432ce20e.png)

The image shown above is the webpage created by the Huzzah WiFi module on the Mbed and the one that is scraped by the Pi for command information

## Further Work
 - Add a raspberry pi camera to live stream what the robot is seeing onto a webserver
 - Add an uLCD screen to the glove to keep track of statistics such as battery, speed, and distance traveled
 - Use radio frequency chips to reduce latency

