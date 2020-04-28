/**
 * Copyright (c) 2020 Farès Chati
 * This file id part of the ArduinoLogger library.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <TimeLib.h>
#include <ostream>

// If you enable these features, you need to have SdCard library installed
#if defined(LOGGER_SD_FEATURES)
# include "SdCard.h"
#endif

#define LOG_LEVEL_SILENT  0
#define LOG_LEVEL_ERROR   1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_TRACE   4
#define LOG_LEVEL_VERBOSE 5

typedef struct LogOutput
{
	Print * stream;
	uint8_t level;
	uint8_t displayIndex;
	bool    prefixOnNextPrint;
	bool    prefixEnabled;
	bool    dateEnabled;
	bool    levelNameEnabled;
	bool    enabled;
	bool    tempDisabled;
} LogOutput;

// Do not display prefix for the current line and specified output
struct npo
{
	Print & output;

	explicit npo(Print & arg) : output (arg){ }
};

// Disable the provided output for this line
struct dsb
{
	Print & output;

	explicit dsb(Print & arg) : output (arg){ }
};

class ArduinoLogger : public ostream
{
public:
	ArduinoLogger();
	ArduinoLogger(uint8_t levelToOutput);

	// Add an output
	void add (Print & stream, uint8_t level,
	  bool prefixEnabled    = true,
	  bool dateEnabled      = true,
	  bool levelNameEnabled = true) const;

	// Modify the properties of an output
	void edit (Print & stream, uint8_t level,
	  bool prefixEnabled    = true,
	  bool dateEnabled      = true,
	  bool levelNameEnabled = true) const;

	void enable (Print & stream) const;           // Enable an output
	void enablePrefix (Print & stream) const;     // Enable prefix
	void enableDate (Print & stream) const;       // Enable date in prefix
	void enableLevelName (Print & stream) const;  // Enable level name in prefix
	void disable (Print & stream) const;          // Disable an output
	void disablePrefix (Print & stream) const;    // Disable prefix
	void disableDate (Print & stream) const;      // Disable date in prefix
	void disableLevelName (Print & stream) const; // Disable level name in prefix

	// Is the output enabled for the specified log level ?
	bool isEnabled (Print & stream, int level = LOG_LEVEL_SILENT) const;

	friend ArduinoLogger & operator << (ostream & s, ArduinoLogger & (*pf)(ArduinoLogger & logger));
	friend ArduinoLogger & operator << (ArduinoLogger & os, const npo & arg);
	friend ArduinoLogger & operator << (ArduinoLogger & os, const dsb & arg);
	friend ArduinoLogger & endl (ArduinoLogger & logger);  // End of line
	friend ArduinoLogger & dendl (ArduinoLogger & logger); // Double end of line
	friend ArduinoLogger & np (ArduinoLogger & logger);    // Do not display prefix for the current line
	friend ArduinoLogger & cr (ArduinoLogger & logger);    // Carriage return

private:
	void putch (char c);
	void putstr (const char * str);
	bool seekoff (off_type off, seekdir way);
	bool seekpos (pos_type pos);
	bool sync ();
	pos_type tellpos ();
	void setflags ();

	char * getClock ();
	LogOutput * getLogOutputFromStream (Print & stream) const;
	void initLogOutput (LogOutput * output, Print & stream, uint8_t level,
	  bool prefixEnabled,
	  bool dateEnabled,
	  bool levelNameEnabled) const;

	const char * debugLevelName (uint8_t debugLevel);
	void printPrefix (uint8_t index);
	void setPrefixOnNextPrint (bool prefixOnNextPrint) const;
	void setNDisplayedOutputs () const;
	void setAllDisplayIndex () const;
	void resetTempDisabled () const;

	const uint8_t _levelToOutput; // The level that needs to be output by the instance

	static LogOutput * _outputs; // Ouputs array
	static uint8_t _nOutputs;    // Outputs counter
	static uint8_t _nDisplayed;  // Enabled outputs counter
	char clock[30];              // 00/00/1970 00:00:00::000
};

ArduinoLogger & endl (ArduinoLogger & logger);  // End of line
ArduinoLogger & dendl (ArduinoLogger & logger); // Double end of line
ArduinoLogger & np (ArduinoLogger & logger);    // Do not display prefix for the current line
ArduinoLogger & cr (ArduinoLogger & logger);    // Carriage return

extern ArduinoLogger err;   // Error level logging
extern ArduinoLogger warn;  // Warning level logging
extern ArduinoLogger inf;   // Info level logging
extern ArduinoLogger trace; // Trace level logging
extern ArduinoLogger verb;  // Verbose level logging
extern const ArduinoLogger logger;

#endif // ifndef LOGGER_H