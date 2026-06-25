#pragma once
#include <iostream>
#include <string>
using namespace std;

class Candidate {
private:
    string id;
    string name;

public:
    Candidate(string i = "", string n = "") : id(i), name(n) {}

    void inputInfo()
    {
        cout << "Nhap ma sinh vien: ";
        cin >> ws;
        getline(cin, id);

        cout << "Nhap ho ten: ";
        getline(cin, name);
    }

    string getId() const { return id; }
    string getName() const { return name; }
};