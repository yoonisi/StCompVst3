#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

namespace LogTool
{
	using namespace std;
	class Logger
	{
	public:
		enum LogLevel {
			NONE = 0,
			FATAL,
			ERROR,
			WARN,
			INFO
		};

		static void Log(LogLevel level, const char* file, const char* func, const int line, const char* message);
		static void Log(LogLevel level, const char* file, const char* func, const int line, std::stringstream& message);
		static bool isLogging(LogLevel level);
		
	private:
		static ofstream* logfile;
		static const int logmode;
		static const LogLevel messaeThreshold;
		static const char * filename;
	};
}

#ifdef _DEBUG
#define LOG(l,x) { LogTool::Logger::Log((l),__FILE__,__FUNCSIG__,__LINE__,(x)); }
#else
#define LOG(l,x)
#endif

