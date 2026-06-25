#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Question
{
protected:
    int id;
    std::string content;
    std::vector<std::string> options;
    char correctAnswer;

public:
    Question(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer)
        : id(id), content(content), options(options), correctAnswer(correctAnswer) {}

    virtual ~Question() = default;
    virtual void displayQuestion() const = 0;

    int getID() const { return id; }
    char getCorrectAnswer() const { return correctAnswer; }
    int getOptionCount() const { return static_cast<int>(options.size()); }
};

class MultipleChoiceQuestion : public Question
{
public:
    MultipleChoiceQuestion(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer)
        : Question(id, content, options, correctAnswer) {}

    void displayQuestion() const override
    {
        cout << "Cau " << id << ": " << content << endl;

        char optionName = 'A';
        for (const std::string& option : options)
        {
            std::cout << optionName << ". " << option << std::endl;
            ++optionName;
        }
    }
};
//hi