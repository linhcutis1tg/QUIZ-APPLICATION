#include "Candidate.h"
#include <limits>

Candidate::Candidate(string i, string n)
    : id(i), name(n)
{
}

void Candidate::inputInfo()
{
    cout << "Nhap ma sinh vien: ";
    cin >> ws;              // Bỏ qua ký tự xuống dòng còn sót lại
    getline(cin, id);

    cout << "Nhap ho ten: ";
    getline(cin, name);
}

string Candidate::getId() const
{
    return id;
}

string Candidate::getName() const
{
    return name;
}

istream& operator>>(istream& in, Candidate& c)
{
    cout << "Nhap ma sinh vien: ";
    in >> ws;               // Bỏ qua khoảng trắng và '\n'
    getline(in, c.id);

    cout << "Nhap ho ten: ";
    getline(in, c.name);

    return in;
}

ostream& operator<<(ostream& out, const Candidate& c)
{
    out << "Ma sinh vien: " << c.id << '\n';
    out << "Ho ten       : " << c.name << '\n';

    return out;
}