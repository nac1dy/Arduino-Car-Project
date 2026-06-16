# Arduino-Car Project

## Description

This repo contains the Code for the Communication between two Arduino Uno's, one mounted on a Eleego Smart Car and one Arduino with a Joystick Shield.
The Sender Arduino, with the Joystick, is sending signals to the Arduino on the Car, which then contains a "Logic-Handler" that does x when signal y is detected.

The Sender has 2 different main modes, one with another 2 modes. 
Either the Car drives with the signals the joystick sends, with either the Joycon or the Buttons. 
The other mode is a selfdriving mode where the car drives forward and takes in the distance. When the distance is lower then 20 cm's, it stopps, checks the distance to the left and the distance to the right, which ever distance is lower, this will be the direction it will take. Meaning it drives until it sensors a obsticle and will then decide based on the distance to objects on left and right which direction it takes.

## Getting Started

### Controller.ino

Controller.ino contains all the logic for sending signales to the other arduino.

It takes the input from A0 and A1 for the Joystick and from 2-7 for Button Inputs.
Then the State gets defined to determine in which mode the controller is in to switch the modes between joystcon mode and analog button mode.

Then in the joystick mode the angle is always read and based on the value of the angle the wire sends a signal to the Car which will then result in a motion

In the Analog button mode it detects if the button is pressed to send a signal to the car which will again result in a motion.

There are also the K and E Button which will result in a Mode switch (from Joystick driving to Autodriving or other way around)
While in Joystick driving one can switch the mode between buttons and joycon with the joycon button (pressing the joycon down)

All these signals (except the signals to switch between Button mode and Joycon mode) are send to the Car which will then move according to the Signal send.

### Car.ino

### Dependencies

Library Wire.h for communication between Arduinos

### Installing

Get yourself 2 Arduinos and a drivable car and connect both and start both.

## Authors

Noel

Niels

