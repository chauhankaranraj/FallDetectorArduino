# FallDetectorArduino

Introduction
------------

This app was created (example app in Android project, modified by Karanraj Chauhan) for the Engineering Design Project of the class ENG EK210 - Introdcution to Engineering Design. The project was to create a wearable device that detects when the wearer has fallen, sends a text message with GPS location, and make a phone call to the target number.

It has been modified to work in tandem with a BLE central (an Android device in this case). The sketch has a custom BLE Service with a BLE Characteristic (which has BLENotfiy property) that functions as an indicator of the device wearer's status. The Android device functions as the central, and once connected to the peripheral (Arduino 101), listens for changes in the BLE Characteristic.

Summary
-------

Currently, the sketch uses the detects the fall through the CURIE_IMU_SHOCK detection with the threshold (experimentally determined for our purposes) to be 4g. However, there is also a custom function (which is currently commented out) which measures the accelerometer reading every 100ms and for the change in acceleration. This is just for added safety. In our experimation the CURIE_IMU_SHOCK detection worked fine so this custom function was not needed. But it can work as an added safety measure, just in case the CURIE_IMU_SHOCK detection fails.

When a change in acceleration above the threshold is detected, the value of a boolean and the BLE Characteristic is changed, and the interrupts are removed so that the execution of the loop can continue without any problems (since there were some errors in communication with the Android device when the characteristic changed value and the interrupt was still attached).

The Android app then performs action based on the value of the BLE characteristic received
