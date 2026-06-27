#pragma once
#include <string>
#include <ctime>
using namespace std;
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
    static string getCurrentDate();

    /**
     * @brief Lấy giờ hiện tại từ đồng hồ hệ thống.
     * @return Chuỗi định dạng giờ (hh:mm:ss) kèm đệm số 0 nếu giá trị nhỏ hơn 10.
     */
    static string getCurrentTime();

    /**
     * @brief Tính hiệu số thời gian (Số giây thực hiện bài thi).
     */
    static int calculateElapsedSeconds(time_t start, time_t end);
};

string TimeService::getCurrentDate() 
{
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);    // Chuyển đổi sang cấu trúc thời gian cục bộ
    ostringstream oss;
    // ltm->tm_mon tính từ 0 nên cần +1, ltm->tm_year tính từ 1900 nên cần +1900
    oss << ltm->tm_mday << "/" << (ltm->tm_mon + 1) << "/" << (ltm->tm_year + 1900);
    return oss.str();
}

string TimeService::getCurrentTime() 
{
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    ostringstream oss;
    oss << (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour << ":"
        << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << ":"
        << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec;
    return oss.str();
}

int TimeService::calculateElapsedSeconds(time_t start, time_t end) 
{
    return static_cast<int>(difftime(end, start));
}