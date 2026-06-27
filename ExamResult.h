#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Candidate.h"
#include "Question.h"

class ExamResult 
{
private:
    Candidate candidate;
    std::string startTime;
    int duration;
    int correctCount;
    double totalScore;
    std::string subject;
    int totalQuestions;

public:
    ExamResult();

    void setCandidate(const Candidate& c);
    void setStartTime(const std::string& time);
    void setDuration(int d);
    void setSubject(const std::string& s);
    void setTotalQuestions(int total);

    void calculateResult(const std::vector<Question*>& questions, const std::vector<char>& candidateAnswers);
    void displayFinalReport() const;
};

ExamResult::ExamResult() 
    : startTime(""), duration(0), correctCount(0), totalScore(0.0), subject(""), totalQuestions(0) {}

void ExamResult::setCandidate(const Candidate& c) { candidate = c; }
void ExamResult::setStartTime(const std::string& time) { startTime = time; }
void ExamResult::setDuration(int d) { duration = d; }
void ExamResult::setSubject(const std::string& s) { subject = s; }
void ExamResult::setTotalQuestions(int total) { totalQuestions = total; }

void ExamResult::calculateResult(const std::vector<Question*>& questions, const std::vector<char>& candidateAnswers) 
{
    correctCount = 0;
    for (size_t i = 0; i < questions.size(); ++i) {
        if (i < candidateAnswers.size() && candidateAnswers[i] == questions[i]->getCorrectAnswer()) {
            ++correctCount;
        }
    }
    if (totalQuestions > 0) {
        totalScore = static_cast<double>(correctCount) * 10.0 / totalQuestions;
    }
}

void ExamResult::displayFinalReport() const 
{
    std::cout << "\n==================== EXAM RESULT ====================\n";
    std::cout << " Student ID    : " << candidate.getId() << "\n";
    std::cout << " Full Name     : " << candidate.getName() << "\n";
    std::cout << " Subject       : " << subject << "\n";
    std::cout << " Start Time    : " << startTime << "\n";
    std::cout << " Duration      : " << duration << " seconds\n";
    std::cout << " Correct Answ. : " << correctCount << "/" << totalQuestions << "\n";
    std::cout << " Total Score   : " << totalScore << "/10.0\n";
    std::cout << "=====================================================\n";
}