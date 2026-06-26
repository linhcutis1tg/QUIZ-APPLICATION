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

#include <iostream>
using namespace std;

class QuizManager 
{
    private:
        vector<Question*> questions;
        vector<char> candidateAnswer;
        int totalQuestions;
        Candidate currentcandidate;
        ExamResult finalResult;
    public:
        QuizManager() : totalQuestions(0) {}
        ~QuizManager();
        bool loadQuestionFromFile(string filename);
        void startExam();
        void reviewAndModifyAnswer();
        
};
QuizManager::~QuizManager()
{
    for (Question* q : questions)
    {
        delete q;
    }
    questions.clear();
}
bool QuizManager::loadQuestionFromFile(string filename) // code đọc lại file input
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Cannot open file!" << endl;
        return false;
    }
    int n;
    file >> n;
    file.ignore();
    for (int i = 0; i < n; i++)
    {
        int id;
        string line;
        getline(file, line);
        stringstream ss(line);
        ss >> id;
        string content;
        getline(ss, content);
        if (!content.empty() && content[0] == ' ')
            content.erase(0, 1);
        string optLine;
        getline(file, optLine);
        stringstream ss2(optLine);
        int count;
        ss2 >> count;
        vector<string> options;
        for (int j = 0; j < count; j++)
        {
            string opt;
            ss2 >> opt;
            options.push_back(opt);
        }
        char ans;
        file >> ans;
        file.ignore();
        ans = static_cast<char>(toupper(ans));

        questions.push_back(
            new MultipleChoiceQuestion(id, content, options, ans)
        );
    }
    totalQuestions =(int)questions.size();
    candidateAnswer.resize(totalQuestions,' ');
    return true;
}
void QuizManager::startExam() {

    // Vẽ khung giao diện trang trí bắt mắt lúc mở app giống video mẫu
    cout << "*********************************\n";
    cout << "* Quiz Program                  *\n";
    cout << "* Date: " << TimeService::getCurrentDate() << "              *\n"; // Gọi hàm lấy ngày tự động
    //cout << "* Number of questions: " << totalQuestions << "        *\n";
    cout << "*********************************\n";
    cout << "Please enter your information!\n";


    cin >> currentcandidate;
    cout << "Press enter to start...";
    cin.get();

    string startTime = TimeService::getCurrentTime();
    time_t startSecond = time(0);

    for (int i = 0; i < totalQuestions; i++) {
        questions[i]->displayQuestion();
        int optionCount = questions[i]->getOptionCount();
        candidateAnswer[i] = InputValidator::getValidatedAnswer(optionCount);
    }

    time_t endSecond = time(0);
    int duration = TimeService::calculateElapsedSeconds(startSecond, endSecond);

    finalResult.setCandidate(currentcandidate);
    finalResult.setStartTime(startTime);
    finalResult.setDuration(duration);
    finalResult.calculateResult(questions, candidateAnswer);

    cout << "You have finished your first attempt in " << duration << " seconds." << endl;
    reviewAndModifyAnswer();
}

void QuizManager::reviewAndModifyAnswer()
{
    while (true)
    {
        char choice;
        cout << "Do you want to change any answers? (Y/N): ";
        cin >> choice;
        choice = toupper(choice);

        if (choice != 'Y')
            break;

        while (true)
        {
            int choiceIndex = 0;
            cout << "Enter the question number you want to change:  ";
            while (!(cin >> choiceIndex))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid interger: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!InputValidator::validateQuestionIndex(choiceIndex, totalQuestions))
            {
                cout << "Invalid question number! Please enter again..." << endl;
                continue;
            }

            int index = choiceIndex - 1;
            cout << "Reviewing question no." << choiceIndex << endl;
            questions[index]->displayQuestion();

            cout << "Current answer  is: " << candidateAnswer[index] << endl;
            cout << "Re-select A/B/C/D (or S to skip): ";
            candidateAnswer[index] = InputValidator::getValidatedAnswer();
            cout << "Updating answer of question no." << choiceIndex << " successfully!" << endl;
            break;
        }
    }

    finalResult.calculateResult(questions, candidateAnswer);
    finalResult.displayFinalReport();
}