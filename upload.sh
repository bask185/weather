#!/bin/bash
python.exe updateIO.py
echo "COMPILING"
arduino-cli compile -b arduino:avr:nano ~/Documents/software/weather
echo "UPLOADING"
arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p COM3 -i ~/Documents/software/weather/weather.arduino.avr.nano.hex
rm *.hex *.elf
exit