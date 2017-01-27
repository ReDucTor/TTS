#include "Log.h"
#include <time.h>

void Log(std::string message)
{
	time_t rawtime;
	struct tm tm;

	time(&rawtime);
	localtime_s(&tm, &rawtime);
	std::string time =  std::to_string(tm.tm_year+1900) + "/" +
						std::to_string(tm.tm_mon+1) + "/" +
						std::to_string(tm.tm_mday) + " " +
						std::to_string(tm.tm_hour) + ":" +
						std::to_string(tm.tm_min) + " ";
	std::string log = time + message;

	std::ofstream fout(LOGFILE, std::ofstream::app);
	fout << log << "\n";
	fout.close();
}

void LogError(std::string message)
{
	Log(message);
	exit(1);
}
