#pragma once
#include <fstream> // NOTE: (reductor) fstream is not used here, it's used in the cpp, should move this into there
#include <string>

#define LOGFILE	"log.txt" // NOTE: (reductor) Should prefer constexpr/const over defines
void Log(std::string message);    // logs a message to LOGFILE
void LogError(std::string message); // logs a message; execution is interrupted