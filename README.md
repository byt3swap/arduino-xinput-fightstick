# arduino-xinput-fightstick
Basic Arcade/Fight Stick Controller using XInput Library

This is a dead simple implementation for creating single joystick, 8 button joystick.  Implemented using [dmadison's ArduinoXInput Library](https://github.com/dmadison/ArduinoXInput).

## Setup
Make sure you follow the guide at dmadison's XInput repo (linked about) for setting up XInput hardware for compatible arduino devices.  Select the appropriate XInput board and flash with manual reset (as described in dmadison's XInput repo).

## Usage
Wire up the buttons as follows.  Pin mappings are made to make wiring clean with the unoffical `Pro Micro` variant.

|Pin|Button|
|---|---|
|`2`|`A`|
|`3`|`B`|
|`4`|`X`|
|`5`|`Y`|
|`6`|`RT`|
|`7`|`LT`|
|`8`|`RB`|
|`9`|`LB`|
|`A0`|`Back`|
|`A1`|`Start`|
|`10`|`Joystick Up`|
|`16`|`Joystick Down`|
|`14`|`Joystick Right`|
|`15`|`Joystick Left`|

This implementation supports joystick mode switching, allowing the joystick to work as an Xbox `Right Stick`, `Left Stick`, or `DPAD`.  `DPAD` is the default mode.  To switch between modes, hold `LB` and `LT` for 3 seconds, release, and then use the joystick to select the mode, based on the table below.

|Direction|Mode|
|---------|----|
|`Down`|`DPAD`|
|`Right`|`Right Stick`|
|`Left`|`Left Sick`|
