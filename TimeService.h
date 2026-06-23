//ko co thuoc tinh
//gom cac static method: getCurrentDate(string) ham nay se lay  ngay thuc te khi bat dau lam bai theo dinh dang DD/MM/YYYY

//getCurrentTime(string) lay thoi gan thuc te theo dinh dang hour:minute:secondd,
//calculateElapsedSeconds(int) ham nay gom 2tham so time_t start va time_t end de tinh thoi gian lam bai cua thi sinh ham nay se duoc goi trong ExamResult
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