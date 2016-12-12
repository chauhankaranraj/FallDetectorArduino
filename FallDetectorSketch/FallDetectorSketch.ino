/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include <CurieBLE.h>
#include "CurieIMU.h"

/* BACKUP FUNCTION
// variables that will hold current and previous measurements of accelerometer
float prevax, prevay, prevaz;
float currentax, currentay, currentaz;

// variables that store value of change in acceleration in x, y, z
float deltax = 0, deltay = 0, deltaz = 0;

// value of g in mm/s^2
float G = 9800;
*/

const int transistorBasePin = 13; // set transistorBasePin on pin 13
BLEPeripheral blePeripheral; // create peripheral instance

long previousMillis = 0;
bool didDetectFall = false;
bool ledBlinkState = LOW;

BLEService ledService("19B10002-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
// BLEIndicate: "here you have a packet. Is this something you want to acknowledge?"
// BLENotify: "you have received a packet, here it is"
BLECharCharacteristic switchChar("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);

void setup() {
  
  // set up LED on pin 13 as an output
  pinMode(transistorBasePin, OUTPUT);  // flashes once every 200ms to indicate fall
  digitalWrite(transistorBasePin, LOW);

  // Set up measurements using IMU
  
  CurieIMU.begin();
  CurieIMU.attachInterrupt(eventCallback);

  // enable Shock Detection
  CurieIMU.setDetectionThreshold(CURIE_IMU_SHOCK, 4500); // 4.5g = 4500 mg
  CurieIMU.setDetectionDuration(CURIE_IMU_SHOCK, 50);   // 50ms
  CurieIMU.interrupts(CURIE_IMU_SHOCK);

//  // Set the accelerometer range to 4G
//  CurieIMU.setAccelerometerRange(4);
  
  // Set up BLE for transmitting data
  
  // set the local name peripheral advertises
  blePeripheral.setLocalName("FallDD");
  // set the UUID for the service this peripheral advertises
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchChar);

  // set an initial value for the characteristic
  switchChar.setValue(0);

  // advertise the service
  blePeripheral.begin();

//  delay(1000);
//
//  // iniital reading of accelerometer
//  CurieIMU.readAccelerometerScaled(prevax, prevay, prevaz);
}

void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central)
  {
    // while the central is still connected to peripheral:
    while (central.connected())
    {
      digitalWrite(transistorBasePin, HIGH);

      // if a fall is detected and switchChar does not already have value then set it to 1
      if (didDetectFall && switchChar.value() != 1)
      {
        switchChar.setValue(1);
      }
            
      // if a fall has been detected, sound the buzzer every 200ms
      if (didDetectFall)
      {
        long currentMillis = millis();
        // if 200ms have passed, change led blink status
        if (currentMillis - previousMillis >= 200)
        {
          ledBlinkState = !ledBlinkState;
          digitalWrite(transistorBasePin, ledBlinkState);
          previousMillis = currentMillis;
        }
      }
      /* BACKUP FUNCTION */
//      else
//      {
//        // measure x,y,z accelerations every 100ms
//        if (currentMillis - previousMillis >= 100)
//        {
//          CurieIMU.readAccelerometerScaled(currentax, currentay, currentaz);
//          deltax = (currentax > prevax) ? (currentax - prevax) : (prevax - currentax);
//          deltay = (currentay > prevay) ? (currentay - prevay) : (prevay - currentay);
//          deltaz = (currentaz > prevaz) ? (currentaz - prevaz) : (prevaz - currentaz);
//
//          if ((deltax > 2.75*G) || (deltay > 3.5*G) || (deltaz > 3*G))
//          {
//            onFallDetected();            
//          }
//          
//        }
//      }
    }
    digitalWrite(transistorBasePin, LOW); // central has been disconnected
  }
}

void onFallDetected()
{
  CurieIMU.noInterrupts(CURIE_IMU_SHOCK);
  CurieIMU.detachInterrupt();
  noInterrupts();
  didDetectFall = true;
}

static void eventCallback(void)
{
  if (CurieIMU.getInterruptStatus(CURIE_IMU_SHOCK)) {
    onFallDetected();
  }
}


/*
  Copyright (c) 2016 Intel Corporation. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-
  1301 USA
*/
