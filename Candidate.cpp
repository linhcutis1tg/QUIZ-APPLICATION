#include "Candidate.h"

Candidate::Candidate(string id, string name) {
    this->id = id;
    this->name = name;
}

void Candidate::inputInfo() {
    cout << "Nhap ma sinh vien: ";
    getline(cin, id);

    cout << "Nhap ho ten: ";
    getline(cin, name);
}

string Candidate::getId() const {
    return id;
}

string Candidate::getName() const {
    return name;
}

istream& operator>>(istream& in, Candidate& c) {
    cout << "Nhap ma sinh vien: ";
    getline(in, c.id);

    cout << "Nhap ho ten: ";
    getline(in, c.name);

    return in;
}

ostream& operator<<(ostream& out, const Candidate& c) {
    out << "Ma sinh vien: " << c.id << endl;
    out << "Ho ten      : " << c.name << endl;

    return out;
}