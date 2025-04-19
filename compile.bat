@echo off
echo Compiling Arduino project...

:: Compile the Arduino sketch for Arduino Mega 2560
arduino-cli compile --fqbn arduino:avr:mega --output-dir ./build SmartCar-Arduino.ino

:: Check if the compile was successful
if %ERRORLEVEL% equ 0 (
    cls
    echo Compilation successful!
) else (
    echo Compilation failed!
)

exit
