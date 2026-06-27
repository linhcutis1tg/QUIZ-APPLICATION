#pragma once
#include <string>
#include <ctime>

class TimeService 
{
public:
    TimeService() = delete; // Class tiện ích

    static std::string getCurrentDate();
    static std::string getCurrentTime();
    static int calculateElapsedSeconds(std::time_t start, std::time_t end);
};

#include "TimeService.h"
#include <sstream>

std::string TimeService::getCurrentDate() 
{
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    std::ostringstream oss;
    oss << ltm->tm_mday << "/" << (ltm->tm_mon + 1) << "/" << (ltm->tm_year + 1900);
    return oss.str();
}

std::string TimeService::getCurrentTime() 
{
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    std::ostringstream oss;
    oss << (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour << ":"
        << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << ":"
        << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec;
    return oss.str();
}

int TimeService::calculateElapsedSeconds(std::time_t start, std::time_t end) 
{
    return static_cast<int>(std::difftime(end, start));
}