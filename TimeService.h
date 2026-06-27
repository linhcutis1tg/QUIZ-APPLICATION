#pragma once
#include <string>
#include <ctime>

/**
 * @brief Lớp cung cấp dịch vụ thời gian hệ thống (Time Utility).
 * Chịu trách nhiệm lấy ngày, giờ thực tế và đo khoảng thời gian chạy.
 */
class TimeService 
{
public:
    TimeService() = delete; // Khóa constructor

    /**
     * @brief Lấy ngày hiện tại từ đồng hồ hệ thống máy tính.
     * @return Chuỗi định dạng ngày (Ngày/Tháng/Năm).
     */
    static std::string getCurrentDate();

    /**
     * @brief Lấy giờ hiện tại từ đồng hồ hệ thống.
     * @return Chuỗi định dạng giờ (hh:mm:ss) kèm đệm số 0 nếu giá trị nhỏ hơn 10.
     */
    static std::string getCurrentTime();

    /**
     * @brief Tính hiệu số thời gian (Số giây thực hiện bài thi).
     */
    static int calculateElapsedSeconds(std::time_t start, std::time_t end);
};

#include "TimeService.h"
#include <sstream>

std::string TimeService::getCurrentDate() 
{
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);    // Chuyển đổi sang cấu trúc thời gian cục bộ
    std::ostringstream oss;
    // ltm->tm_mon tính từ 0 nên cần +1, ltm->tm_year tính từ 1900 nên cần +1900
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