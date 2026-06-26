#pragma once
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class Candidate
{
private:
    string id;
    string name;

    // Kiểm tra mã sinh viên
    bool isValidID(string id)
    {
        if (id.length() < 8 || id.length() > 12)
        {
            return false;
        }

        for (char c : id)
        {
            if (!isalnum(c))  //isalnum() hàm có sẵn của C++ kiểm tra các Ký tự có phải là số ko
            {
                return false;
            }
        }

        return true;
    }

    // Kiểm tra họ tên
    bool isValidName(string name)
    {
        for (char c : name)
        {
            if (!isalpha(c) && c != ' ') //isalpha() hàm có sẵn của C++ kiểm tra các Ký tự có phải là chữ ko
            {
                return false;
            }
        }

        return true;
    }

public:
    Candidate(string i = "", string n = "")
        : id(i), name(n)
    {
    }

    friend istream& operator>>(istream& in, Candidate& c)
    {
        // Nhập mã sinh viên
        do
        {
            cout << "Enter student ID number: ";
            in >> ws;
            getline(in, c.id);

            if (!c.isValidID(c.id))
            {
                cout << "Invalid student ID! Please enter it again.\n";
            }

        } while (!c.isValidID(c.id));

        // Nhập họ tên
        do
        {
            cout << "Enter full name: ";
            getline(in, c.name);

            if (!c.isValidName(c.name))
            {
                cout << "Invalid full name! Please enter it again..\n";
            }

        } while (!c.isValidName(c.name));

        return in;
    }

    friend ostream& operator<<(ostream& out, const Candidate& c)
    {
        out << "Student ID: " << c.id << endl;
        out << "Full name: " << c.name << endl;

        return out;
    }

    string getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }
};