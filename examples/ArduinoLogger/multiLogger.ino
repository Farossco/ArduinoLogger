/**
 * Copyright (c) 2020 Farès Chati
 * This file id part of the ArduinoLogger library.
 *
 * Example code for multi-output logger
 *
 * This example code demonstrate how to setup a multi-output Logger
 *
 * A multi-ouput logger will print the same message on all the provided outputs
 *
 * Please note that the internal date needs to be initialised somehow
 */

#include <Arduino.h>
#include <Logger.h>

void setup ()
{
	Serial.begin (9600);
	Serial1.begin (9600);
	Serial2.begin (9600);

	logger.add (Serial, LOG_LEVEL_VERBOSE); // This will log everything on Serial

	/* /!\ not available on Arduino Uno, Nano and Mini /!\ */
	logger.add (Serial1, LOG_LEVEL_VERBOSE); // This will log everything on Serial1

	// You can theoretically add as many outputs as you'd like

	inf << np << endl; // Displays an end of line without the prefix (Because of "np")

	err << "This is an error message" << endl;
	warn << "This is a warning message" << endl;
	inf << "This is an info message" << endl;
	trace << "This is a trace message" << endl;
	verb << "This is a verbose message" << dendl; // Double end of line

	logger.edit (Serial, LOG_LEVEL_INFO);   // This will log info messages and bellow
	logger.edit (Serial1, LOG_LEVEL_TRACE); // This will log trace messages and bellow

	err << "This is an error message" << endl;
	warn << "This is a warning message" << endl;
	inf << "This is an info message" << endl;
	trace << "I'm only showing up on Serial1" << endl; // This will only be displayed on Serial1
	verb << "I'm not displayed at all :'(" << dendl;   // This will not be displayed at all

	// By default, the date is displayed with the prefix,
	// it can be disabled respectively for every output :
	logger.disableDate (Serial);

	inf << "This is displayed with no date on Serial" << endl;

	logger.edit (Serial, LOG_LEVEL_INFO, true, true, true); // Enabling prefix, date and level name display

	inf << "This has a full prefix again" << endl;

	inf << np << "This has no prefix at all on both outputs" << endl;

	inf << "And prefix is back on this one" << endl;

	logger.disable (Serial1); // You can also disable an output

	inf << "This does not have output counter" << endl;

	logger.add (Serial2, LOG_LEVEL_VERBOSE); // You can add a third output

	inf << "This has an output counter of 1|2 on Serial and 2|2 on Serial2" << endl;

	logger.enable (Serial1);

	inf << "This has an output counter of 1|3 on Serial, 2|3 on Serial1 and 3|3 on Serial2" << endl;

	inf << npo (Serial) << "This has no prefix on Serial only" << endl;

	inf << dsb (Serial) << "This is disabled for Serial for this line" << endl;

	inf << "This is disabled on all outputs" << endl;
} // setup

void loop ()
{ }

/** Output on Serial :
 *
 * [00/00/1970 00:00:00::000] [1|2] [ ERROR ] This is an error message
 * [00/00/1970 00:00:00::006] [1|2] [WARNING] This is a warning message
 * [00/00/1970 00:00:00::078] [1|2] [ INFO  ] This is an info message
 * [00/00/1970 00:00:00::148] [1|2] [ TRACE ] This is a trace message
 * [00/00/1970 00:00:00::218] [1|2] [VERBOSE] This is a verbose message
 *
 * [00/00/1970 00:00:00::290] [1|2] [ ERROR ] This is an error message
 * [00/00/1970 00:00:00::361] [1|2] [WARNING] This is a warning message
 * [00/00/1970 00:00:00::433] [1|2] [ INFO  ] This is an info message
 * [1|2] [ INFO  ] This is displayed with no date on Serial
 * [00/00/1970 00:00:00::667] [1|2] [ INFO  ] This has a full prefix again
 * This has no prefix at all on both outputs
 * [00/00/1970 00:00:00::785] [1|2] [ INFO  ] And prefix is back on this one
 * [00/00/1970 00:00:00::863] [ INFO  ] This does not have output counter
 * [00/00/1970 00:00:00::870] [1|2] [ INFO  ] This has an output counter of 1|2 on Serial and 2|2 on Serial2
 * [01/01/1970 00:00:01::020] [1|3] [ INFO  ] This has an output counter of 1|3 on Serial, 2|3 on Serial1 and 3|3 on Serial2
 * This has no prefix on Serial only
 * [01/01/1970 00:00:01::391] [1|3] [ INFO  ] This is disabled on all outputs
 *
 *
 ** Output on Serial1 :
 *
 * [00/00/1970 00:00:00::003] [2|2] [ ERROR ] This is an error message
 * [00/00/1970 00:00:00::074] [2|2] [WARNING] This is a warning message
 * [00/00/1970 00:00:00::144] [2|2] [ INFO  ] This is an info message
 * [00/00/1970 00:00:00::214] [2|2] [ TRACE ] This is a trace message
 * [00/00/1970 00:00:00::285] [2|2] [VERBOSE] This is a verbose message
 *
 * [00/00/1970 00:00:00::357] [2|2] [ ERROR ] This is an error message
 * [00/00/1970 00:00:00::429] [2|2] [WARNING] This is a warning message
 * [00/00/1970 00:00:00::498] [2|2] [ INFO  ] This is an info message
 * [00/00/1970 00:00:00::503] [2|2] [ TRACE ] I'm only showing up on Serial1
 * [00/00/1970 00:00:00::580] [2|2] [ INFO  ] This is displayed with no date on Serial
 * [00/00/1970 00:00:00::673] [2|2] [ INFO  ] This has a full prefix again
 * This has no prefix at all on both outputs
 * [00/00/1970 00:00:00::794] [2|2] [ INFO  ] And prefix is back on this one
 * [01/01/1970 00:00:01::104] [2|3] [ INFO  ] This has an output counter of 1|3 on Serial, 2|3 on Serial1 and 3|3 on Serial2
 * [01/01/1970 00:00:01::222] [2|3] [ INFO  ] This has no prefix on Serial only
 * [01/01/1970 00:00:01::302] [2|3] [ INFO  ] This is disabled for Serial for this line
 * [01/01/1970 00:00:01::400] [2|3] [ INFO  ] This is disabled on all outputs
 *
 *
 ** Output on Serial2 :
 *
 * [00/00/1970 00:00:00::977] [2|2] [ INFO  ] This has an output counter of 1|2 on Serial and 2|2 on Serial2
 * [01/01/1970 00:00:01::163] [3|3] [ INFO  ] This has an output counter of 1|3 on Serial, 2|3 on Serial1 and 3|3 on Serial2
 * [01/01/1970 00:00:01::243] [3|3] [ INFO  ] This has no prefix on Serial only
 * [01/01/1970 00:00:01::332] [3|3] [ INFO  ] This is disabled for Serial for this line
 * [01/01/1970 00:00:01::410] [3|3] [ INFO  ] This is disabled on all outputs
 */