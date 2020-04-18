# ArduinoLogger

*An Arduino library for simple and clean-looking logging*

This library allows you to easily add logging to your program. It is mainly made for long-term projects or projects that need constant logging (into an SD card for example) as it does not provide a way to easily compile out all the logging codes.

## Features
* Intuitive usage
* Several log levels (Error, Warning, Info, Trace, Verbose)
* Multi-output: Logs the message on several outputs at once
* Logging using the iostream style (`<<`)
* Supports all kind of Print output
* Displays a prefix with a clock and the logging level (Configurable)

## Tested for
* Arduino Mega
* ESP8266

## Installation
### PlatformIO
This library is available on the PlatformIO package manager.

To install it, simply add it to your `platformio.ini` file under `lib_deps`

Example :

```ini
[env:Arduino]
platform = atmelavr
board = megaatmega2560
framework = arduino
lib_deps = ArduinoLogger
```

### Arduino IDE
If you're using the Arduino IDE, you need to manually install this library

* First, head over to https://github.com/Farossco/ArduinoLogger
* Click on `Clone or Download` then `Download ZIP`
* Extract the folder into the Arduino Library folder
* Rename the folder to ArduinoLogger

The library folder is usually under :
* `C:\users\[you]\documents\Arduino` on Windows
* `/home/[you]/Arduino` on Linux

## Usage

This library is made to be very simple to use. To initialize the logger for an output, you need to provide the output and the log level.

If you're using a Serial as your output, you still need to call `Serial.begin()`

```c++
logger.add (Serial, LOG_LEVEL_VERBOSE); 

err << "This is an error message" << endl;
warn << "This is a warning message" << endl;
inf << "This is an info message" << endl;
trace << "This is a trace message" << endl;
verb << "This is a verbose message" << endl;
```

## Dependencies
* [`Time`](https://github.com/PaulStoffregen/Time) library for clock prefix display