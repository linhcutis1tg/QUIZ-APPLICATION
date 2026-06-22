#pragma once
#include <string>
#include <vector>

class Question
{
protected:
    int id;                           // ma cau hoi
    std::string content;              // noi dung cau hoi
    std::vector<std::string> options; // cac lua chon
    char correctAnswer;               // dap an
public:
    Question(int id, const std::string &content, const std::vector<std::string> &options, char correctAnswer);
    virtual ~Question() {}
    virtual void displayQuestion() const = 0;
    int getID() const { return id; }
    char getCorrectAnswer() const { return correctAnswer; }
};
    