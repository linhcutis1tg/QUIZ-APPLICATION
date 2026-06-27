#pragma once
#include <iostream>
#include <string>

/**
 * Lớp quản lý thông tin và kiểm tra tính hợp lệ của Thí sinh (Candidate).
 * Chịu trách nhiệm nhập xuất dữ liệu và ràng buộc định dạng Tên / Mã sinh viên.
 */

class Candidate 
{
private:
    std::string id;     // Mã số sinh viên (Yêu cầu: 8-12 ký tự chữ và số)
    std::string name;   // Họ và tên thí sinh (Yêu cầu: chỉ chứa chữ cái và khoảng trắng)

    /**
     * @brief Kiểm tra định dạng Mã số sinh viên.
     * @param id Chuỗi mã số cần kiểm tra.
     * @return true nếu hợp lệ (độ dài 8-12, không chứa ký tự đặc biệt), ngược lại false.
     */
    bool isValidID(const std::string& id);

    /**
     * @brief Kiểm tra định dạng Tên sinh viên.
     * @param name Chuỗi họ tên cần kiểm tra.
     * @return true nếu hợp lệ (chỉ chứa chữ cái và dấu cách), ngược lại false.
     */
    bool isValidName(const std::string& name);

public:
    // Hàm khởi tạo mặc định và có tham số
    Candidate(std::string i = "", std::string n = "");

    // Các hàm Getter lấy giá trị thuộc tính ẩn (Encapsulation)
    std::string getId() const;
    std::string getName() const;

    /**
     * @brief Quá tải toán tử nhập (>>). Tự động ép buộc người dùng nhập đúng định dạng.
     */
    friend std::istream& operator>>(std::istream& in, Candidate& c);

    /**
     * @brief Quá tải toán tử xuất (<<) để in nhanh thông tin thí sinh ra màn hình.
     */
    friend std::ostream& operator<<(std::ostream& out, const Candidate& c);
};

Candidate::Candidate(std::string i, std::string n) : id(i), name(n) {}

bool Candidate::isValidID(const std::string& id)
{
    if (id.length() < 8 || id.length() > 12) return false;
    for (char c : id) {
        if (!std::isalnum(c)) return false; // Chỉ cho phép ký tự chữ hoặc số
    }
    return true;
}

bool Candidate::isValidName(const std::string& name)
{
    for (char c : name) {
        if (!std::isalpha(c) && c != ' ') return false; // Không cho phép số hay ký tự đặc biệt
    }
    return true;
}

std::string Candidate::getId() const { return id; }
std::string Candidate::getName() const { return name; }

std::istream& operator>>(std::istream& in, Candidate& c) 
{
    // Vòng lặp kiểm tra điều kiện mã sinh viên
    do {
        std::cout << "Enter student ID number (8-12 alphanumeric characters): ";
        in >> std::ws; // Xóa khoảng trắng và ký tự xuống dòng còn sót trong bộ đệm
        std::getline(in, c.id);
        if (!c.isValidID(c.id)) {
            std::cout << "Invalid student ID! Please try again.\n";
        }
    } while (!c.isValidID(c.id));

    // Vòng lặp kiểm tra điều kiện họ tên
    do {
        std::cout << "Enter full name (letters only): ";
        std::getline(in, c.name);
        if (!c.isValidName(c.name)) {
            std::cout << "Invalid full name! Please try again.\n";
        }
    } while (!c.isValidName(c.name));

    return in;
}

std::ostream& operator<<(std::ostream& out, const Candidate& c) 
{
    out << "------------- Candidate Information -------------\n";
    out << "Student ID: " << c.id << "\nFull name: " << c.name << "\n";
    return out;
}