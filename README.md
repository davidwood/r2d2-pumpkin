# R2D2 Pumpkin

The R2D2 pumpkin is powered by an [Arduino UNO](https://www.arduino.cc/en/Main/ArduinoBoardUno) and this repository contains the Arduino sketch. 

## Power

The pumpkin can be powered using a 5V wall adapter or USB, but for portablility, a [6 slot AA battery](https://www.amazon.com/gp/product/B01BSLPE2I) adapter was used. A single-pole, single-throw (SPST) switch has been wired inline 

## Sound

The R2D2-like sound effects are mono, unsigned 8-bit WAVs, played from an SD card through a [SD card shield](https://www.amazon.com/gp/product/B006LRR0IQ/). The sounds being used can be found in the `sounds` directory.

The Arduino [SD](https://www.arduino.cc/en/Reference/SD) library is used to read the WAV files from the SD card. Sound playback requires the [TMRpcm](https://github.com/TMRh20/TMRpcm) library.

The speaker being used is a small [tablet buzzer](https://www.amazon.com/gp/product/B00WEBRRVI/), but any 8 Ohm 5W speaker can be used. 

Pin Connections:
* `red`: `9`
* `black`: `GND` (ground)

## Motion Detection

A passive infrared (PIR) sensor is mounted into the front of the pumpkin When the sensor detects motion, it triggers servo movement and sound effect playback. The [PIR being used](https://www.amazon.com/gp/product/B019F3OFBM) is based on the [HC-SR501](https://www.mpja.com/download/31227sc.pdf) and allows for easy adjustment of sensitivity, time delay, and trigger mode.

Pin Connections:
* `middle`: `2`
* `right`: `5V` (power)
* `left`: `GND` (ground)

## Movement

The R2D2 head is turned with a [TowerPro SG90 Micro Servo](https://www.amazon.com/gp/product/B01608II3Q). 

As the Arduino [Servo](https://www.arduino.cc/en/Reference/Servo) library uses the same 16-bit timer (`Timer1`) as the [TMRpcm](https://github.com/TMRh20/TMRpcm) library, the [ServoTimer2](https://github.com/nabontra/ServoTimer2) library is used instead. This library uses an 8-bit timer, but is only compatible with the [1.0.x](https://www.arduino.cc/en/Main/OldSoftwareReleases#1.0.x) version of the IDE and will not compile with 1.6.x.

Pin Connections:
* `orange`: `10`
* `red`: `5V` (power)
* `brown`: `GND` (ground)
