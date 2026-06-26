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

public:
    ExamResult() : startTime(""), duration(0), correctCount(0), totalScore(0.0) {}

    void setCandidate(const Candidate& c) { candidate = c; }
    void setStartTime(const string& time) { startTime = time; }
    void setDuration(int d) { duration = d; }

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
        totalScore = static_cast<double>(correctCount);
    }

    void displayFinalReport() const
    {
        cout << "\n========== KET QUA BAI THI ==========" << endl;
        cout << candidate.getId() << " - " << candidate.getName() << endl;
        cout << "Thoi gian bat dau : " << startTime << endl;
        cout << "Thoi gian lam bai : " << duration << " giay" << endl;
        //// chủ đề 1 2 3 
        // cin 
        cout << "So cau dung       : " << correctCount << endl; // số câu đùng/tổng số câu hỏi = n của chủ đề 
        cout << "Tong diem         : " << totalScore << endl;
    }
};