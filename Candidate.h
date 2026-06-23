// //viet class candidate gom cac thuoc tinh: id (string), name(string) 
// cac phuong thuc: constructor, ham inputinfo() de nhap thong tin thi sinh
#pragma once
#include <iostream>
#include <string>
using namespace std;

class Candidate {
private:
    string id;
    string name;

public:
    // Constructor
    Candidate(string i = "", string n = "")
        : id(i), name(n)
    {
    }

    // Hàm nhập thông tin
    void inputInfo() 
    {
        cout << "Nhap ma sinh vien: ";
        cin >> ws;
        getline(cin, id);

        cout << "Nhap ho ten: ";
        getline(cin, name);
    }

    // Getter
    string getId() const 
    {
        return id;
    }

    string getName() const 
    {
        return name;
    }

    // Nạp chồng toán tử
    friend istream& operator>>(istream& in, Candidate& c) 
    {
        cout << "Nhap ma sinh vien: ";
        in >> ws;
        getline(in, c.id);

        cout << "Nhap ho ten: ";
        getline(in, c.name);

        return in;
    }

    friend ostream& operator<<(ostream& out, const Candidate& c) 
    {
        out << "Ma sinh vien: " << c.id << '\n';
        out << "Ho ten       : " << c.name << '\n';
        return out;
    }
};