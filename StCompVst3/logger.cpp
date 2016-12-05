#include "logger.h"
#include <time.h>

namespace LogTool
{
	using namespace std;

	ofstream* Logger::logfile = 0;
	const char * Logger::filename = "C:\\tmp\\vstlog.csv";
	const Logger::LogLevel Logger::messaeThreshold = Logger::LogLevel::INFO;
	const int Logger::logmode = static_cast<int>(std::ios::app);

	void Logger::Log(LogLevel level, const char * file, const char* func, const int line, const char * message) {
		if (!isLogging(level)) { return; }	
		if (Logger::logfile == 0) {
			logfile = new ofstream(Logger::filename, Logger::logmode);
			time_t now = time(NULL);
			char timeString[128] = { 0 };
			ctime_s(timeString, sizeof(timeString), &now);
			*logfile << timeString;
		}
		*logfile << file << "," << line << "," << func << "," << message << endl;
	}

	void Logger::Log(LogLevel level, const char * file, const char* func, const int line, stringstream& sstream) {
		Logger::Log(level, file, func, line, sstream.str().c_str());
	}

	bool Logger::isLogging(Logger::LogLevel level) {
#ifdef _DEBUG
		return (level <= messaeThreshold);
#else
		return false;
#endif

	}

}
