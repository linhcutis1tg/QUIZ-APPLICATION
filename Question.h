#pragma once
#include <iostream>
#include <string>
#include <vector>

class Question 
{
protected:
    int id;
    std::string content;
    std::vector<std::string> options;
    char correctAnswer;

public:
    Question(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer);
    virtual ~Question() = default;

    virtual void displayQuestion() const = 0;
    int getID() const;
    char getCorrectAnswer() const;
    int getOptionCount() const;
};

class MultipleChoiceQuestion : public Question 
{
public:
    MultipleChoiceQuestion(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer);
    void displayQuestion() const override;
};

Question::Question(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer)
    : id(id), content(content), options(options), correctAnswer(correctAnswer) {}

int Question::getID() const { return id; }
char Question::getCorrectAnswer() const { return correctAnswer; }
int Question::getOptionCount() const { return static_cast<int>(options.size()); }

MultipleChoiceQuestion::MultipleChoiceQuestion(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer)
    : Question(id, content, options, correctAnswer) {}

void MultipleChoiceQuestion::displayQuestion() const 
{
    std::cout << "\nQuestion " << id << ": " << content << "\n";
    char optionName = 'A';
    for (const std::string& option : options) {
        std::cout << "  " << optionName << ". " << option << "\n";
        ++optionName;
    }
}