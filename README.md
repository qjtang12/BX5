# A building block-based beam-break (B5) locomotor activity monitoring system and its use in circadian biology research
## Introduction
This is a system that uses infrared LEDs and receivers to monitor the locomotor activity of small rodents in biologocal researches.
The manuscript is now published online at:https://www.future-science.com/doi/10.2144/btn-2022-0036

I will keep building this project, bare with me please the things you want to check out is not uploaded yet. Highly appreciate any feedbacks on optimizing the system. I am a biology major, so this system is not optimized in engineer's eyes for sure. However, this also means you don't need engineer background to get it going at least.

## Getting Started
B5_circuit.png is the diagram showing the connection of the core part. To make it less messy, I only drew two ethernet jacks, just use the same idea, to add two more when making your own one.

4BeamBreak_RTC.ino is the Arduino script operating the system.

You will need the Arduino IDE to edit/upload the script to your Arduino.

A real time clock (RTC) module is used to track the time of data entry. You will need an extra Arduino library. DS3231.zip.

Follow the instruction here to add the library to your Arduino IDE.https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries






## Acknowledgement
For the real time clock (RTC) module in this project, I used the Library made by Henning Karlsen. The library is included here to download but can also be found and downloaded at http://www.rinkydinkelectronics.com/library.php?id=73
