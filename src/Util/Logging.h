#ifndef Logging_h__
#define Logging_h__

#ifdef _WIN32
// http://stackoverflow.com/a/2282433
#define __func__ __FUNCTION__
// http://stackoverflow.com/a/8488201
#define __BASE_FILE__ \
	(strrchr(__FILE__, '/') \
	? strrchr(__FILE__, '/') + 1 \
	: (strrchr(__FILE__, '\\') \
	? strrchr(__FILE__, '\\') + 1 \
	: __FILE__))
#endif

#include <iostream>
#include <limits>
#include <string>
#include <stdio.h>
#include <stdarg.h>

enum _LOG_LEVEL
{	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
};

#ifdef DEBUG
static _LOG_LEVEL LOG_LEVEL = LOG_LEVEL_DEBUG;
#else
static _LOG_LEVEL LOG_LEVEL = LOG_LEVEL_INFO;
#endif

const static char* _LOG_LEVEL_PREFIX[] =
{	"E: ",
	"W: ",
	"",
	"D: "
};

static void _LOG(_LOG_LEVEL logLevel, char* file, char* func, unsigned int line, const char* format, ...)
{	if (logLevel > LOG_LEVEL)
		return;

	va_list args;
	va_start(args, format);
	char* message = nullptr;
	size_t size = vsnprintf(message, 0, format, args);
	message = new char[size + 1];
	message[size] = '\0';
	vsnprintf(message, size, format, args);
	va_end(args);

	if (logLevel == LOG_LEVEL_ERROR)
	{	std::cerr << file << ":" << line << " " << func << std::endl;
		std::cerr << _LOG_LEVEL_PREFIX[logLevel] << message << std::endl;
	}
	else
	{	std::cout << _LOG_LEVEL_PREFIX[logLevel] << message << std::endl;
	}

	delete[] message;
}

#define LOG(logLevel, format, ...) \
	_LOG(logLevel, __BASE_FILE__, __func__, __LINE__, format, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
	LOG(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#define LOG_WARNING(format, ...) \
	LOG(LOG_LEVEL_WARNING, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...) \
	LOG(LOG_LEVEL_INFO, format, ##__VA_ARGS__)

#define LOG_DEBUG(format, ...) \
	LOG(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)

#endif // Logging_h__