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

#include "Logger.h"

LogOutput * Logger::_outputs = NULL;
uint8_t Logger::_nOutputs    = 0;
uint8_t Logger::_nDisplayed  = 0;

Logger::Logger() : _levelToOutput (LOG_LEVEL_SILENT)
{ }

Logger::Logger (uint8_t levelToOutput) : _levelToOutput (levelToOutput)
{
	setflags();
}

void Logger::add (Print & stream, uint8_t level, bool prefixEnabled, bool dateEnabled, bool levelNameEnabled) const
{
	LogOutput * oldOutputs = _outputs;

	// If the ouput is already present, just edit it
	if (getLogOutputFromStream (stream) != NULL)
		edit (stream, level, prefixEnabled, dateEnabled, levelNameEnabled);
	else
	{
		_outputs = new LogOutput[++_nOutputs]; // Incrementing the counter and creating a new array with one more item

		// Copying the existing array
		for (int i = 0; i < _nOutputs - 1; i++)
			_outputs[i] = oldOutputs[i];

		delete oldOutputs; // Then, removing the old array

		initLogOutput (&_outputs[_nOutputs - 1], stream, level, prefixEnabled, dateEnabled, levelNameEnabled); // Initializing the new item
	}
}

void Logger::edit (Print & stream, uint8_t level, bool prefixEnabled, bool dateEnabled, bool levelNameEnabled) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (output != NULL)
		initLogOutput (output, stream, level, prefixEnabled, dateEnabled, levelNameEnabled);
}

void Logger::enable (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (output != NULL)
		output->enabled = true;

	setAllDisplayIndex();
	setNDisplayedOutputs();
}

void Logger::disable (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (output != NULL)
		output->enabled = false;

	setAllDisplayIndex();
	setNDisplayedOutputs();
}

void Logger::enablePrefix (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (!output)
		return;

	output->prefixEnabled = true;
}

void Logger::enableDate (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (!output)
		return;

	output->dateEnabled = true;
}

void Logger::enableLevelName (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (!output)
		return;

	output->levelNameEnabled = true;
}

void Logger::disablePrefix (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (!output)
		return;

	output->prefixEnabled = false;
}

void Logger::disableDate (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (!output)
		return;

	output->dateEnabled = false;
}

void Logger::disableLevelName (Print & stream) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (!output)
		return;

	output->levelNameEnabled = false;
}

bool Logger::isEnabled (Print & stream, int level) const
{
	LogOutput * output = getLogOutputFromStream (stream);

	if (!output)
		return false;

	return output->enabled && output->level >= level;
}

LogOutput * Logger::getLogOutputFromStream (Print & stream) const
{
	for (uint8_t i = 0; i < _nOutputs; i++)
		if (_outputs[i].stream == &stream)
			return &_outputs[i];

	return NULL;
}

void Logger::initLogOutput (LogOutput * output, Print & stream, uint8_t level, bool prefixEnabled, bool dateEnabled, bool levelNameEnabled) const
{
	output->stream            = &stream;
	output->level             = constrain (level, LOG_LEVEL_SILENT, LOG_LEVEL_VERBOSE);
	output->prefixEnabled     = prefixEnabled;
	output->prefixOnNextPrint = prefixEnabled;
	output->dateEnabled       = true;
	output->levelNameEnabled  = true;
	output->enabled           = true;
	output->tempDisabled      = false;

	setAllDisplayIndex();
	setNDisplayedOutputs();
}

void Logger::setflags ()
{
	flags (dec | right | skipws | showbase | uppercase | boolalpha);
}

Logger &operator << (ostream & s, Logger& (*pf)(Logger & logger))
{
	return pf ((Logger&) s);
}

Logger &operator << (Logger &os, const npo &arg)
{
	LogOutput * output = os.getLogOutputFromStream (arg.output);

	if (output != NULL)
		output->prefixOnNextPrint = false;

	return os;
}

Logger &operator << (Logger &os, const dsb &arg)
{
	LogOutput * output = os.getLogOutputFromStream (arg.output);

	if (output != NULL)
		output->tempDisabled = true;

	return os;
}

Logger& endl (Logger& logger)
{
	logger.put ('\n');
	logger.setPrefixOnNextPrint (true);
	logger.setflags();
	logger.resetTempDisabled();

	return logger;
}

Logger& dendl (Logger& logger)
{
	logger.put ('\n');

	return endl (logger);
}

Logger& np (Logger& logger)
{
	logger.setPrefixOnNextPrint (false);

	logger << setfill (1);
	return logger;
}

void Logger::putch (char c)
{
	char str[2];

	str[0] = c;
	str[1] = '\0';

	putstr (str);
}

void Logger::putstr (const char * str)
{
	for (uint8_t i = 0; i < _nOutputs; i++)
	{
		if (!_outputs[i].tempDisabled && _outputs[i].enabled && _outputs[i].level >= _levelToOutput)
		{
			printPrefix (i);
			_outputs[i].stream->write (str);
		}
	}
}

bool Logger::seekoff (off_type off, seekdir way)
{
	(void) off;
	(void) way;
	return false;
}

bool Logger::seekpos (pos_type pos)
{
	(void) pos;
	return false;
}

bool Logger::Logger::sync ()
{
	return true;
}

Logger::pos_type Logger::tellpos ()
{
	return 0;
}

void Logger::printPrefix (uint8_t index)
{
	LogOutput * output = &_outputs[index];

	if (output->prefixEnabled && output->prefixOnNextPrint)
	{
		Print * stream = output->stream;

		if (output->dateEnabled)
		{
			stream->print (F ("["));
			stream->print (clock());
			stream->print (F ("] "));
		}

		if (_nDisplayed > 1)
		{
			stream->print (F ("["));
			stream->print (_outputs[index].displayIndex);
			stream->print (F ("|"));
			stream->print (_nDisplayed);
			stream->print (F ("] "));
		}

		if (output->levelNameEnabled)
		{
			stream->print (F ("["));
			stream->print (debugLevelName (_levelToOutput));
			stream->print (F ("] "));
		}

		output->prefixOnNextPrint = false;
	}
} // Logger::printPrefix

void Logger::setPrefixOnNextPrint (bool prefixOnNextPrint) const
{
	for (uint8_t i = 0; i < _nOutputs; i++)
		_outputs[i].prefixOnNextPrint = prefixOnNextPrint;
}

void Logger::setNDisplayedOutputs () const
{
	_nDisplayed = 0;

	for (uint8_t i = 0; i < _nOutputs; i++)
		if (_outputs[i].enabled)
			_nDisplayed++;
}

void Logger::setAllDisplayIndex () const
{
	uint8_t n = 0;

	for (uint8_t i = 0; i <= _nOutputs; i++)
		if (_outputs[i].enabled)
			_outputs[i].displayIndex = ++n;
}

void Logger::resetTempDisabled () const
{
	for (uint8_t i = 0; i < _nOutputs; i++)
		_outputs[i].tempDisabled = false;
}

const char * Logger::debugLevelName (uint8_t debugLevel)
{
	switch (debugLevel)
	{
		case LOG_LEVEL_ERROR:
			return " ERROR ";

		case LOG_LEVEL_WARNING:
			return "WARNING";

		case LOG_LEVEL_INFO:
			return " INFO  ";

		case LOG_LEVEL_TRACE:
			return " TRACE ";

		case LOG_LEVEL_VERBOSE:
			return "VERBOSE";

		default:
			return "???????";
	}
}

String Logger::clock ()
{
	char buf[25];

	sprintf (buf, "%.2d/%.2d/%.4d %.2d:%.2d:%.2d::%.3ld", day(), month(), year(), hour(), minute(), second(), (millis() % 1000));

	return buf;
}

Logger err   (LOG_LEVEL_ERROR);
Logger warn  (LOG_LEVEL_WARNING);
Logger inf   (LOG_LEVEL_INFO);
Logger trace (LOG_LEVEL_TRACE);
Logger verb  (LOG_LEVEL_VERBOSE);
const Logger logger;