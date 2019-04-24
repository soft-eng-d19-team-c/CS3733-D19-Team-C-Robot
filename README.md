# CS3733-D19-Team-C-Robot
Code for the robot

## Dependencies

In the Arduino IDE you will need to install [WiFiNINA](https://github.com/arduino-libraries/WiFiNINA) from the library manager and [Arduino megaAVR Boards](https://github.com/arduino/ArduinoCore-megaavr) from the board manager for use with the Uno Wifi Rev2 

## Hardware

* [Arduino Uno WiFi Rev 2](https://store.arduino.cc/usa/arduino-uno-wifi-rev2)
* [Arduino Motor Shield](https://store.arduino.cc/usa/arduino-motor-shield-rev3)
* [Dagu Rover 5 Tracked Chassis](https://www.pololu.com/product/1550)

## POST data format

Comma-seperated String: "turn, distance, turn, distance..."

turn: String containing either L, R, or S

distance: int (In string form) containing distance to drive straight in inches

### Example

If the following PathToText directions are generated:

* Starting at Center for International Medicine
* Take a right at Tower Hallway 5 distance: 9ft
* Take a left at Tower Hallway 2 distance: 49ft
* Take a right at Tower Hallway 1 distance: 30ft
* Continue straight past Tower Elevator Entrance distance: 10ft
* Take a right at Tower Hallway 10 distance: 14ft
* Take a right at Tower Hallway 9 distance: 9ft
* Finally, arrive at Multifaith Chapel


The data sent to the robot should look like:

```
"R,108,L,588,R,360,S,120,R,168,R,108";
```

The maximum number of moves that can be sent and run at once is currently 100. This can be changed by setting ```max_moves``` on line 6 to the desired value.