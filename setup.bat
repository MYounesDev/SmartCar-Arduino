@echo off
echo Setting up Arduino project...

:: Update Arduino CLI index
arduino-cli lib update-index

:: Install required libraries
arduino-cli lib install LiquidCrystal

:: Install Arduino Mega 2560 core
arduino-cli core install arduino:avr

echo Setup complete.
