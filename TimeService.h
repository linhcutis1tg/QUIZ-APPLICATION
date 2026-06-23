#pragma once
#include <string>
#include <ctime>
#include <sstream>

class TimeService {
public:
    TimeService() = delete;

    //Truy cập thời gian hiện tại của máy tính, 
    //bóc tách lấy Ngày/Tháng/Năm để trả về chuỗi định dạng 
    static std::string getCurrentDate() {
        std::time_t now = std::time(nullptr);
        std::tm* ltm = std::localtime(&now);

        std::ostringstream oss;
        oss << ltm->tm_mday << "/"
            << (ltm->tm_mon + 1) << "/"
            << (ltm->tm_year + 1900);
        return oss.str();
    }

    //bóc tách giờ, phút, giây của hệ thống
    static std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        std::tm* ltm = std::localtime(&now);

        std::ostringstream oss;
        oss << ltm->tm_hour << ":"
            << ltm->tm_min << ":"
            << ltm->tm_sec;
        return oss.str();
    }

    //sử dụng difftime để trẻ về chính xác thời gian lúc bắt đầu và kết thúc
    static int calculateElapsedSeconds(std::time_t start, std::time_t end) {
        return static_cast<int>(std::difftime(end, start));
    }
};