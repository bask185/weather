#!/bin/bash
python.exe updateIO.py
arduino-cli compile -b arduino:avr:nano ~/Documents/software/weather
exit