//gom cac thuoc tinh: Candidate (goi class Candidate.h), startTime (string/time_t), duration(int), correctCount(int), totalScore(double)
//gom cac phuong thuc: CalculateResult (ham nay truyen tham chieu den vector<Questions*> de thuc hien doi chieu diem chuan da hinh,
//ham displayFinalReport() de hien thi ket qua
//goi ham calculateElapsedSeconds(int) de in ra thoi gian lam bai cua thi sinh
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
    // Constructor
    ExamResult()
    {
        startTime = "";
        duration = 0;
        correctCount = 0;
        totalScore = 0.0;
    }

    // Gán thông tin thí sinh
    void setCandidate(const Candidate& c) : candidate(c)
    {
    }

    // Gán thời gian bắt đầu
    void setStartTime(const string& time) : startTime(time)
    {
    }

    // Gán thời gian làm bài
    void setDuration(int d) : duration(d);
    {
    }

    // Tính kết quả bài thi
    void calculateResult(vector<Question*> questions,
        vector<char> candidateAnswers) : correctCount(0)
    {
        for (int i = 0; i < questions.size(); i++)
        {
            if (candidateAnswers[i] == questions[i]->getCorrectAnswer())
            {
                correctCount++;
            }
        }

        // Mỗi câu đúng được 1 điểm
        totalScore = correctCount;
    }

    // Hiển thị kết quả cuối cùng
    void displayFinalReport()
    {
        cout << "\n========== KET QUA BAI THI ==========\n";
        cout << candidate;
        cout << "Thoi gian bat dau : " << startTime << endl;
        cout << "Thoi gian lam bai : " << duration << " giay" << endl;
        cout << "So cau dung       : " << correctCount << endl;
        cout << "Tong diem         : " << totalScore << endl;
    }
};