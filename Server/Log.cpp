#include "Log.h"
#include <time.h> // NOTE: (reductor) Should prefer the C++ headers instead (e.g. time.h -> ctime), and also use the std:: prefix for their types (e.g. std::time, etc)

void Log(std::string message) // NOTE: (reductor) This message is not consumed, should use const&
{
	time_t rawtime;
	struct tm tm;

	time(&rawtime);
	localtime_s(&tm, &rawtime);
	std::string time =  std::to_string(tm.tm_year+1900) + "/" +
						std::to_string(tm.tm_mon+1) + "/" +
						std::to_string(tm.tm_mday) + " " +
						std::to_string(tm.tm_hour) + ":" +
						std::to_string(tm.tm_min) + " "; // NOTE: (reductor) Could use std::put_time instead
	std::string log = time + message;

	std::ofstream fout(LOGFILE, std::ofstream::app);
	fout << log << "\n";
	fout.close();
}

void LogError(std::string message)
{
	Log(message);
	exit(1); // NOTE: (reductor) Should use std::exit and include cstdlib
}
