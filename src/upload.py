#!/usr/bin/env python
import os
os.system("python src/build.py")
print("UPLOADING")
os.system("arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p COM4 -i ./build/arduino.avr.nano/weather.ino.hex")
exit