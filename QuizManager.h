#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <limits>
#include "Question.h"
#include "Candidate.h" 
#include "ExamResult.h" 
#include "InputValidator.h"
#include "TimeService.h"

class QuizManager 
{
private:
    std::vector<Question*> questions;
    std::vector<char> candidateAnswers;
    int totalQuestions;
    std::string currentSubject;
    Candidate currentCandidate;
    ExamResult finalResult;

public:
    QuizManager();
    ~QuizManager();

    bool loadQuestionFromFile(const std::string& filename);
    void startExam();
    void reviewAndModifyAnswer();
};

QuizManager::QuizManager() : totalQuestions(0) {}

QuizManager::~QuizManager() {
    for (Question* q : questions) {
        delete q;
    }
    questions.clear();
}

bool QuizManager::loadQuestionFromFile(const std::string& filename) {
    std::cout << "*************************************************\n";
    std::cout << "* QUIZ PROGRAM                  *\n";
    std::cout << "* Date: " << TimeService::getCurrentDate() << "                             *\n";
    std::cout << "*************************************************\n";
    std::cout << "Please enter your information:\n";
    std::cin >> currentCandidate;

    for (Question* q : questions) {
        delete q;
    }
    questions.clear();
    candidateAnswers.clear();
    totalQuestions = 0;
    currentSubject.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Cannot open file " << filename << "!\n";
        return false;
    }

    std::vector<std::string> subjects;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (std::isdigit(line[0])) break;
        subjects.push_back(line);
    }

    if (subjects.empty()) {
        std::cout << "No subjects found in file!\n";
        file.close();
        return false;
    }

    std::cout << "\n================ CHOOSE SUBJECT ================\n";
    for (size_t i = 0; i < subjects.size(); i++) {
        std::cout << " " << i + 1 << ". " << subjects[i] << "\n";
    }
    std::cout << "================================================\n";

    int choice;
    while (true) {
        std::cout << "Select subject (1-" << subjects.size() << "): ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        if (choice >= 1 && choice <= static_cast<int>(subjects.size())) break;
        std::cout << "Choice out of range! Select again.\n";
    }

    std::cin.ignore(1000, '\n');
    file.clear();
    file.seekg(0);

    currentSubject = subjects[choice - 1];
    std::string target = std::to_string(choice) + ". ";
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find(target) == 0) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Subject section not found in file!\n";
        file.close();
        return false;
    }

    int numQuestions = 0;
    if (!(file >> numQuestions)) {
        file.close();
        return false;
    }
    file.ignore(1000, '\n');

    int questionLoaded = 0;
    while (questionLoaded < numQuestions && std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        int id;
        ss >> id;
        std::string content;
        std::getline(ss, content);
        if (!content.empty() && content[0] == ' ') content.erase(0, 1);
            
        std::string optLine;
        if (!std::getline(file, optLine)) break; 
        std::stringstream ss2(optLine);
        int count;
        ss2 >> count;

        std::vector<std::string> options;
        for (int j = 0; j < count; j++) {
            std::string opt;
            if (!(ss2 >> opt)) break;
            options.push_back(opt);
        }

        std::string ansLine;
        if (!std::getline(file, ansLine)) break;
        std::stringstream ss3(ansLine);
        char ans;
        if (!(ss3 >> ans)) break;

        questions.push_back(new MultipleChoiceQuestion(id, content, options, static_cast<char>(std::toupper(ans))));
        questionLoaded++;
    }
    file.close();
    
    totalQuestions = static_cast<int>(questions.size());
    candidateAnswers.assign(totalQuestions, 'S'); 
    return totalQuestions > 0;
}

void QuizManager::startExam() {
    std::cout << "\nPress Enter to start the exam...";
    std::cin.get();

    std::string startTime = TimeService::getCurrentTime();
    std::time_t startSecond = std::time(0);

    for (int i = 0; i < totalQuestions; i++) {
        questions[i]->displayQuestion();
        int optionCount = questions[i]->getOptionCount();
        candidateAnswers[i] = InputValidator::getValidatedAnswer(optionCount);
    }

    std::time_t endSecond = std::time(0);
    int duration = TimeService::calculateElapsedSeconds(startSecond, endSecond);

    finalResult.setCandidate(currentCandidate);
    finalResult.setStartTime(startTime);
    finalResult.setDuration(duration);
    finalResult.setSubject(currentSubject);          
    finalResult.setTotalQuestions(totalQuestions);
    finalResult.calculateResult(questions, candidateAnswers);

    std::cout << "\nYou have finished your exam in " << duration << " seconds.\n";
    reviewAndModifyAnswer();
}

void QuizManager::reviewAndModifyAnswer() {
    while (true) {
        char choice;
        std::cout << "Do you want to review or change any answers? (Y/N): ";
        std::cin >> choice;
        choice = static_cast<char>(std::toupper(choice));

        if (choice != 'Y') break;

        while (true) {
            int choiceIndex = 0;
            std::cout << "Enter the question number you want to change (1-" << totalQuestions << "): ";
            while (!(std::cin >> choiceIndex)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please enter a valid integer: ";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (!InputValidator::validateQuestionIndex(choiceIndex, totalQuestions)) {
                std::cout << "Question number out of range! Please try again.\n";
                continue;
            }

            int index = choiceIndex - 1;
            std::cout << "\n--- Reviewing Question " << choiceIndex << " ---";
            questions[index]->displayQuestion();

            std::cout << "Current saved answer: " << candidateAnswers[index] << "\n";
            std::cout << "Enter new choice ";
            candidateAnswers[index] = InputValidator::getValidatedAnswer(questions[index]->getOptionCount());
            std::cout << "Answer for question " << choiceIndex << " updated successfully!\n\n";
            break;
        }
    }
    
    finalResult.calculateResult(questions, candidateAnswers);
    finalResult.displayFinalReport();
}