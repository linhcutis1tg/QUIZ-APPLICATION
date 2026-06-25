#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Question
{
protected:
    int id;                           // ma cau hoi
    std::string content;              // noi dung cau hoi
    std::vector<std::string> options; // cac lua chon
    char correctAnswer;               // dap an
public:
    Question(int id, const string &content, const vector<string> &options, char correctAnswer) 
        :id(id), content(content), options(options), correctAnswer(correctAnswer){}
    virtual ~Question() {}
    virtual void displayQuestion() const = 0;
    int getID() const { return id; }
    char getCorrectAnswer() const { return correctAnswer; }
};
class MultipleChoiceQuestion : public Question
{
public:
    MultipleChoiceQuestion(int id, const string &content,const vector<string> &options, char correctAnswer)
        : Question(id, content, options, correctAnswer)
    {
    }
    void displayQuestion() const override
    {
        cout << "Cau " << id << ": " << content << endl;

        char optionName = 'A';

        for (int i = 0; i < options.size(); i++)
        {
            std::cout << optionName << ". " << options[i] << std::endl;
            optionName++;
        }
    }
};
