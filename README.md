List of Contents
1. Introduction
2. Node Overview
3. Components and Wiring
4. Libraries
5. License
 
# Introduction

Node repository is part of the RF scanner project. For a full project overview, please refer to the [RF_Scanner](https://github.com/MaybeAshLately/RF_Scanner) repository on GitHub account MaybeAshLately. 

# Node overview
The Node’s task is to measure RF spectrum occupancy and send the results to the server through the I2C bus.

Measurment has format of an 126 8-bits integers array, each containing the number of signals detected on the corresponding channels. The scanner operates in the frequency of 2,4 - 2,525 GHz. Each channel is 1 MHz wide. Scanner can detect eg. Wi-Fi and bluetooth signals. Due to I2C limitations each measurment is send in for 4 parts (32, 32, 32 and 30 bytes).

**Warning** in this example, the node adress is hardcoded as "8". To use multiple nodes (slaves), you must change their adresses. On I2C bus there can no be two devices with the same adress. You can modify the address simply by changing the following line:

```bash
const int slaveAddress=8;
```
Addresses can range from 8 to 126.

# Components and wiring 
The node part of RF scanner uses the following components:
- Arduino Uno R3
- nRF24L01 - the radio module

To connect nRF24 to Arduino: 
- CSN connects to pin 10, 
- CE connects to pin 9, 
- MOSI connects to pin 11, 
- SCK connects to pin 13, 
- MISO connects to pin 12
- VCC connects to 3.3V,
- GNF connects to GND.


The I2C bus uses pins SCL and SDA. The master device is also connected to pin 2, which wakes up the Arduino from sleep mode using an interrupt routine . 

# Libraries
The project uses following libraries:
- Wire.h - for I2C bus
- avr/sleep.h - to put the microcontroller to sleep and save power
- [RF24](https://github.com/nRF24/RF24) - driver for nRF24L01, version 1.4.10. The measurment logic is based on example from this library. 

# License 
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.
