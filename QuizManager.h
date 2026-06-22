#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Question.h"
#include "Candidate.h" 
#include "ExamResult.h" 
#include "InputValidator.h"

#include <iostream>
using namespace std;

class QuizManager 
{
    private:
        vector<Question*>question;
        vector<char>candidateAnswer;
        int totalQuestions;
        Candidate currentcandidate;
        ExamResult finalResult;
    public:
        QuizManager();
        ~QuizManager();
        void loadQuestionFromFile(string filename);
        void startExam();
        void reviewAndmodifyanswer();

};
void QuizManager::loadQuestionFromFile(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Cannot open file!" << endl;
        return;
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

        question.push_back(
            new MultipleChoiceQuestion(id, content, options, ans)
        );
    }
    totalQuestions = question.size();
    candidateAnswer.resize(totalQuestions);
}
