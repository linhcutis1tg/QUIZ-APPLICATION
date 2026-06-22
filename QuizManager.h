#pragma once
#include <vector>
#include <string>
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