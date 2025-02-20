/*
 * CLogger.hpp
 *
 *  Created on: Feb 22, 2021
 *      Author: andy
 */
#pragma once
#ifndef EXTERN_CARBON_SRC_CLOGGER_HPP_
#define EXTERN_CARBON_SRC_CLOGGER_HPP_

#include <c_port.h>
#include <sstream>

#ifdef C_POCO
namespace Poco {
class Logger;
}
#endif


/**
 * Poco LogStream dumps to the log when a newline i.e. std::endl is encountered,
 * howeve the old proprietary logging system dumps basically when the stream object
 * goes out of scope.
 *
 * This object allows us to to use the new Poco logging system and maintain
 * compatability with all existing code.
 *
 * This object is returne from the rr::Logger, exposes a ostream interface, and
 * and dumps to the log when it goes out of scope.
 */
class CAPI_EXPORT CLoggingBuffer
{
public:
    CLoggingBuffer(int level, const char* func, const char* file, int line);

    /**
     * dump the contents of the stringstream to the log.
     */
    ~CLoggingBuffer();

    /**
     * get the stream this buffer holds.
     */
    std::ostream &stream();

private:
    std::stringstream buffer;
    int level;
    const char* func;
    const char* file;
    int line;
};

/**
 * same as Poco level, repeat here to avoid including any Poco files
 * as Poco is usually linked statically so third parties would not need
 * to have Poco installed.
 */
enum CLogLevel
{
    LOG_CURRENT = 0, /// Use the current level -- don't change the level from what it is.
    LOG_FATAL = 1,   /// A fatal error. The application will most likely terminate. This is the highest priority.
    LOG_CRITICAL,    /// A critical error. The application might not be able to continue running successfully.
    LOG_ERROR,       /// An error. An operation did not complete successfully, but the application as a whole is not affected.
    LOG_WARNING,     /// A warning. An operation completed with an unexpected result.
    LOG_NOTICE,      /// A notice, which is an information with just a higher priority.
    LOG_INFORMATION, /// An informational message, usually denoting the successful completion of an operation.
    LOG_DEBUG,       /// A debugging message.
    LOG_TRACE        /// A tracing message. This is the lowest priority.
};

enum CLogEvent
{
    LOG_OUTPUTSTREAM_CHANGED,
    LOG_LEVEL_CHANGED,
    LOG_CALLBACK_SET
};

typedef HRESULT (*CLoggerCallback)(CLogEvent, std::ostream *);

/**
 * The Carbon logger.
 *
 * A set of static method for setting the logging level.
 */
class CAPI_EXPORT CLogger : PyObject
{
public:




    /**
     * sets the logging level to one a value from Logger::Level
     */
    static void setLevel(int level = LOG_CURRENT);

    /**
     * get the current logging level.
     */
    static int getLevel();

    /**
     * Suppresses all logging output
     */
    static void disableLogging();

    /**
     * stops logging to the console, but file logging may continue.
     */
    static void disableConsoleLogging();

    /**
     * turns on console logging at the given level.
     */
    static void enableConsoleLogging(int level = LOG_CURRENT);

    /**
     * turns on file logging to the given file as the given level.
     *
     * If fileName is an empty string, then this will use the file
     * specified in the Config::LOGGER_LOG_FILE_PATH. If this is empty,
     * a file called "roadrunner.log" will be created in the
     * temp directory.
     *
     * The temp directory may be specified via Config::TEMP_DIR_PATH.
     * If this is empty, then the system spefified temp dir is
     * used.
     */
    static void enableFileLogging(const std::string& fileName = "",
            int level = LOG_CURRENT);

    /**
     * turns off file logging, but has no effect on console logging.
     */
    static void disableFileLogging();

    /**
     * get the textural form of the current logging level.
     */
    static std::string getCurrentLevelAsString();

    /**
     * get the name of the currently used log file.
     */
    static std::string getFileName();

    /**
     * Internally, RoadRunner uses the Poco logging framework, so we
     * can custom format logging output based on a formatting pattern
     * string.
     *
     * The format pattern is used as a template to format the message and
     * is copied character by character except for the following special characters,
     * which are replaced by the corresponding value.
     *
     * An example pattern of "%Y-%m-%d %H:%M:%S %p: %t"
     *
     * would produce the following output:
     *
     * 2013-10-25 14:12:45 Fatal: console and file: A fatal error
     * 2013-10-25 14:12:45 Critical: console and file: A critical error
     * 2013-10-25 14:12:45 Error: console and file: An error
     * 2013-10-25 14:12:45 Warning: console and file: A warning.
     * 2013-10-25 14:12:45 Notice: console and file: A notice.
     *
     * The following formatting pattern descriptions is copied from the
     * Poco documentation:
     *
     *   * %s - message source
     *   * %t - message text
     *   * %l - message priority level (1 .. 7)
     *   * %p - message priority (Fatal, Critical, Error, Warning, Notice, Information, Debug, Trace)
     *   * %q - abbreviated message priority (F, C, E, W, N, I, D, T)
     *   * %P - message process identifier
     *   * %T - message thread name
     *   * %I - message thread identifier (numeric)
     *   * %N - node or host name
     *   * %U - message source file path (empty string if not set)
     *   * %u - message source line number (0 if not set)
     *   * %w - message date/time abbreviated weekday (Mon, Tue, ...)
     *   * %W - message date/time full weekday (Monday, Tuesday, ...)
     *   * %b - message date/time abbreviated month (Jan, Feb, ...)
     *   * %B - message date/time full month (January, February, ...)
     *   * %d - message date/time zero-padded day of month (01 .. 31)
     *   * %e - message date/time day of month (1 .. 31)
     *   * %f - message date/time space-padded day of month ( 1 .. 31)
     *   * %m - message date/time zero-padded month (01 .. 12)
     *   * %n - message date/time month (1 .. 12)
     *   * %o - message date/time space-padded month ( 1 .. 12)
     *   * %y - message date/time year without century (70)
     *   * %Y - message date/time year with century (1970)
     *   * %H - message date/time hour (00 .. 23)
     *   * %h - message date/time hour (00 .. 12)
     *   * %a - message date/time am/pm
     *   * %A - message date/time AM/PM
     *   * %M - message date/time minute (00 .. 59)
     *   * %S - message date/time second (00 .. 59)
     *   * %i - message date/time millisecond (000 .. 999)
     *   * %c - message date/time centisecond (0 .. 9)
     *   * %F - message date/time fractional seconds/microseconds (000000 - 999999)
     *   * %z - time zone differential in ISO 8601 format (Z or +NN.NN)
     *   * %Z - time zone differential in RFC format (GMT or +NNNN)
     *   * %E - epoch time (UTC, seconds since midnight, January 1, 1970)
     *   * %[name] - the value of the message parameter with the given name
     *   * %% - percent sign
     */
    static void setFormattingPattern(const std::string &format);

    /**
     * get the currently set formatting pattern.
     */
    static std::string getFormattingPattern();

    /**
     * gets the textual form of a logging level Enum for a given value.
     */
    static std::string levelToString(int level);

    /**
     * parses a string and returns a Logger::Level
     */
    static CLogLevel stringToLevel(const std::string& str);

    /**
     * check if we have colored logging enabled.
     */
    static bool getColoredOutput();

    static std::ostream *getOutputStream();

    /**
     * enable / disable colored output
     */
    static void setColoredOutput(bool);


    /**
     * Set the color of the output logging messages.
     *
     * In the future, we may add additional properties her.
     *
     * The following properties are supported:
     * enableColors:      Enable or disable colors.
     * traceColor:        Specify color for trace messages.
     * debugColor:        Specify color for debug messages.
     * informationColor:  Specify color for information messages.
     * noticeColor:       Specify color for notice messages.
     * warningColor:      Specify color for warning messages.
     * errorColor:        Specify color for error messages.
     * criticalColor:     Specify color for critical messages.
     * fatalColor:        Specify color for fatal messages.
     *
     *
     * The following color values are supported:
     *
     * default
     * black
     * red
     * green
     * brown
     * blue
     * magenta
     * cyan
     * gray
     * darkgray
     * lightRed
     * lightGreen
     * yellow
     * lightBlue
     * lightMagenta
     * lightCyan
     * white
     */
    static void setProperty(const std::string& name, const std::string& value);


    /**
     * logs a message to the log.
     *
     * This is most useful from Python as C++ code will typically call the
     * LoggingBuffer via the Log macro.
     */
    static void log(CLogLevel level, const std::string& msg);


    /**
     * Set a pointer to an ostream object where the console logger should
     * log to.
     *
     * Normally, this points to std::clog.
     *
     * This is here so that the Logger can properly re-direct to the
     * Python sys.stderr object as the QT IPython console only
     * reads output from the python sys.stdout and sys.stderr
     * file objects and not the C++ file streams.
     */
    static void setConsoleStream(std::ostream *os);


    static void setCallback(CLoggerCallback);

};

#if C_POCO
/**
 * the real logger is actually a Poco::Logger named "RoadRunner", i.e.
 * Poco::Logger::get("RoadRunner").
 *
 * This returns that logger.
 */
RR_DECLSPEC Poco::Logger &getLogger();
#endif



#ifndef NO_LOGGER
#define Log(level) \
    if (level > CLogger::getLevel()) { ; } \
    else CLoggingBuffer(level, MX_FUNCTION, __FILE__, __LINE__).stream()
#else
#define Log(level) \
    if (true) {  }\
    else \
    CLoggingBuffer(level, MX_FUNCTION, __FILE__, __LINE__)
#endif



CAPI_DATA(PyTypeObject) CLogger_Type;

HRESULT _CLogger_Init(PyObject *m);



#endif /* EXTERN_CARBON_SRC_CLOGGER_HPP_ */
