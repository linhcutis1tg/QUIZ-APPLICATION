#pragma once
#include <iostream>
#include <string>
using namespace std;
/**
 * Lớp quản lý thông tin và kiểm tra tính hợp lệ của Thí sinh (Candidate).
 * Chịu trách nhiệm nhập xuất dữ liệu và ràng buộc định dạng Tên / Mã sinh viên.
 */
class Candidate 
{
private:
    string id;     // Mã số sinh viên (Yêu cầu: 8-12 ký tự chữ và số)
    string name;   // Họ và tên thí sinh (Yêu cầu: chỉ chứa chữ cái và khoảng trắng)

    /**
     * @brief Kiểm tra định dạng Mã số sinh viên.
     * @param id Chuỗi mã số cần kiểm tra.
     * @return true nếu hợp lệ (độ dài 8-12, không chứa ký tự đặc biệt), ngược lại false.
     */
    bool isValidID(const string& id);

    /**
     * @brief Kiểm tra định dạng Tên sinh viên.
     * @param name Chuỗi họ tên cần kiểm tra.
     * @return true nếu hợp lệ (chỉ chứa chữ cái và dấu cách), ngược lại false.
     */
    bool isValidName(const string& name);

public:
    // Hàm khởi tạo mặc định và có tham số
    Candidate(string i = "", string n = "");

    // Các hàm Getter lấy giá trị thuộc tính ẩn (Encapsulation)
    string getId() const;
    string getName() const;

    /**
     * @brief Quá tải toán tử nhập (>>). Tự động ép buộc người dùng nhập đúng định dạng.
     */
    friend istream& operator>>(istream& in, Candidate& c);

    /**
     * @brief Quá tải toán tử xuất (<<) để in nhanh thông tin thí sinh ra màn hình.
     */
    friend ostream& operator<<(ostream& out, const Candidate& c);
};

Candidate::Candidate(string i, string n) : id(i), name(n) {}

bool Candidate::isValidID(const string& id)
{
    if (id.length() < 8 || id.length() > 12) return false;
    for (char c : id) {
        if (!isalnum(c)) return false; // Chỉ cho phép ký tự chữ hoặc số
    }
    return true;
}

bool Candidate::isValidName(const string& name)
{
    for (char c : name) {
        if (!isalpha(c) && c != ' ') return false; // Không cho phép số hay ký tự đặc biệt
    }
    return true;
}

string Candidate::getId() const { return id; }
string Candidate::getName() const { return name; }

istream& operator>>(istream& in, Candidate& c) 
{
    // Vòng lặp kiểm tra điều kiện mã sinh viên
    do {
        cout << "\n Student ID : ";
        in >> ws; // Xóa khoảng trắng và ký tự xuống dòng còn sót trong bộ đệm
        getline(in, c.id);
        if (!c.isValidID(c.id)) {
            cout << "Invalid student ID! Please try again.\n";
        }
    } while (!c.isValidID(c.id));

    // Vòng lặp kiểm tra điều kiện họ tên
    do {
        cout << " Full Name  : ";
        getline(in, c.name);
        if (!c.isValidName(c.name)) {
            cout << "Invalid full name! Please try again.\n";
        }
    } while (!c.isValidName(c.name));

    return in;
}

ostream& operator<<(std::ostream& out, const Candidate& c) 
{
    out << "------------- Candidate Information -------------\n";
    out << "Student ID: " << c.id << "\nFull name: " << c.name << "\n";
    return out;
}