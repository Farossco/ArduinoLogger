/**
 * Copyright (c) 2020 Farès Chati
 * This file id part of the ArduinoLogger library.
 *
 * Example code for a simple logger
 *
 * This example features most of the Logger's functionalities
 *
 * Please note that the internal date needs to be initialised somehow
 */

#include <Arduino.h>
#include <Logger.h>

void setup ()
{
	Serial.begin (9600);

	logger.add (Serial, LOG_LEVEL_VERBOSE); // This will log everything on Serial

	inf << np << endl; // Displays an end of line without the prefix (Because of "np")

	err << "This is an error message" << endl;
	warn << "This is a warning message" << endl;
	inf << "This is an info message" << endl;
	trace << "This is a trace message" << endl;
	verb << "This is a verbose message" << dendl; // Double end of line

	logger.edit (Serial, LOG_LEVEL_INFO); // This will log info messages and bellow

	err << "This is an error message" << endl;
	warn << "This is a warning message" << endl;
	inf << "This is an info message" << endl;
	trace << "Whatever, I'm not going to be printed" << endl; // This will not be displayed
	verb << "Me neither" << endl;                             // This will not be displayed either

	// By default, the date is displayed with the prefix,
	// it can be disabled respectively for every output :
	logger.disableDate (Serial);

	inf << "This is displayed with no date" << endl;

	logger.edit (Serial, LOG_LEVEL_INFO, true, true, true); // Enabling prefix, date and level name display

	inf << "This has a full prefix again" << endl;

	inf << np << "This one has no prefix at all" << endl;

	inf << "And prefix is back on this one" << endl;
} // setup

void loop ()
{ }

/** Output :
 *
 * [00/00/1970 00:00:00::000] [ ERROR ] This is an error message
 * [00/00/1970 00:00:00::001] [WARNING] This is a warning message
 * [00/00/1970 00:00:00::064] [ INFO  ] This is an info message
 * [00/00/1970 00:00:00::126] [ TRACE ] This is a trace message
 * [00/00/1970 00:00:00::190] [VERBOSE] This is a verbose message
 *
 * [00/00/1970 00:00:00::257] [ ERROR ] This is an error message
 * [00/00/1970 00:00:00::321] [WARNING] This is a warning message
 * [00/00/1970 00:00:00::387] [ INFO  ] This is an info message
 * [ INFO  ] This is displayed with no date
 * [00/00/1970 00:00:00::493] [ INFO  ] This has a full prefix again
 * This one has no prefix at all
 * [00/00/1970 00:00:00::592] [ INFO  ] And prefix is back on this one
 */