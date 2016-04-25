# TIG-YobiJula - *You can feel it*
Sensing glove project for **Space Apps Challenge 2016** in **Space Station** category!

This project consists in a autonomous glove equipped with pressure sensors on the fingertips that allows to transfer the pressure while grabbing an object to vibration motors or other actuators on the back of the hand.

## Why?

Three-time Hubble repair mission astronaut, John Grunsfield, described the repair proccess as:

> "doing brain surgery in the dark while wearing oven mitts"

This because the astronauts can't feel when they grab an object or use a tool during the ExtraVehicular Activities.

## How it works?

While grabbing an object, the pressure sensors on the fingertips measures how much pressure is being done and transfer it to the glove controller. The glove controller based on the pressure readings calculates how much vibration is needed to transmit the *"feeling"* of grabbing an object. The glove is equipped with a gyroscope to readjust the vibration applied based on the position of the hand.

## Components

* Arduino Mega 2560
* Force Sensitive Resistor
* Pressure Conductive Sheet (Velostat)
* Conductive Thread
* Vibration Motor (Precision Microdrives 310-101)
* Gyroscope MPU-6050
