// tao them mot class MultipleChoiceQuestion la lop con cua class Question thua huong tat ca thuoc tinh cua Question va ham displayQuestion
// de ghi de cau hoi sau moi lan lam
#pragma once
#include <iostream>
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
class MultipleChoiceQuestion : public Question
{
public:
    MultipleChoiceQuestion(int id, const std::string &content,const std::vector<std::string> &options, char correctAnswer)
        : Question(id, content, options, correctAnswer)
    {
    }
    void displayQuestion() const override
    {
        std::cout << "Cau " << id << ": " << content << std::endl;

        char optionName = 'A';

        for (int i = 0; i < options.size(); i++)
        {
            std::cout << optionName << ". " << options[i] << std::endl;
            optionName++;
        }
    }
};
