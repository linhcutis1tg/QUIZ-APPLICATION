#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Candidate.h"
#include "Question.h"

using namespace std;

class ExamResult
{
private:
    Candidate candidate;   // Thông tin thí sinh
    string startTime;      // Thời gian bắt đầu
    int duration;          // Thời gian làm bài (giây)
    int correctCount;      // Số câu trả lời đúng
    double totalScore;     // Tổng điểm
    string subject;        // Tên môn
    int totalQuestions;    // Tổng số câu của môn

public:
    ExamResult()
        : startTime(""),
        duration(0),
        correctCount(0),
        totalScore(0.0),
        subject(""),
        totalQuestions(0)
    {
    }

    void setCandidate(const Candidate& c) { candidate = c; }
    void setStartTime(const string& time) { startTime = time; }
    void setDuration(int d) { duration = d; }
    void setSubject(const string& s) { subject = s; }
    void setTotalQuestions(int total) { totalQuestions = total; }

    void calculateResult(const vector<Question*>& questions, const vector<char>& candidateAnswers)
    {
        correctCount = 0;
        for (size_t i = 0; i < questions.size(); ++i)
        {
            if (i < candidateAnswers.size() && candidateAnswers[i] == questions[i]->getCorrectAnswer())
            {
                ++correctCount;
            }
        }

        totalScore = (double)correctCount * 10 / totalQuestions;
    }

    void displayFinalReport() const
    {
        cout << "\n========== KET QUA BAI THI ==========\n";
        cout << "Ma sinh vien      : " << candidate.getId() << endl;
        cout << "Ho ten            : " << candidate.getName() << endl;
        cout << "Mon thi           : " << subject << endl;
        cout << "Thoi gian bat dau : " << startTime << endl;
        cout << "Thoi gian lam bai : " << duration << " giay" << endl;
        cout << "So cau dung       : " << correctCount << "/" << totalQuestions << endl;
        cout << "Tong diem         : " << totalScore << "/10" << endl;
    }
};