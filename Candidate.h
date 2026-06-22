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
    Candidate(string id = "", string name = "");

    // Hàm nhập thông tin
    void inputInfo();

    // Getter
    string getId() const;
    string getName() const;

    // Nạp chồng toán tử
    friend istream& operator>>(istream& in, Candidate& c);
    friend ostream& operator<<(ostream& out, const Candidate& c);
};