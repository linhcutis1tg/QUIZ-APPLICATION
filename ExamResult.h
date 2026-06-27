#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Candidate.h"
#include "Question.h"

/**
 * @brief Lớp chứa dữ liệu và tính toán Kết quả bài thi (Exam Report).
 * Thực hiện chấm điểm dựa trên danh sách câu hỏi gốc và đáp án của thí sinh.
 */
class ExamResult 
{
private:
    Candidate candidate;    // Đối tượng thông tin thí sinh tham gia thi
    string startTime;  // Thời điểm bắt đầu làm bài (hh:mm:ss)
    int duration;           // Thời điểm bắt đầu làm bài (hh:mm:ss)
    int correctCount;       // Số lượng câu trả lời đúng
    double totalScore;      // Điểm số quy đổi về thang điểm 10
    string subject;    // Tên môn học thi hiện tại
    int totalQuestions;     // Tổng số câu hỏi của bài thi

public:
    // Khởi tạo các giá trị mặc định ban đầu cho kết quả
    ExamResult();

    // Các hàm Setter cập nhật thông tin bài thi từ tiến trình thi ngoài
    void setCandidate(const Candidate& c);
    void setStartTime(const string& time);
    void setDuration(int d);
    void setSubject(const string& s);
    void setTotalQuestions(int total);

    /**
     * @brief Thuật toán so sánh đáp án và tính toán điểm số.
     * @param questions Danh sách con trỏ lưu câu hỏi gốc của đề thi.
     * @param candidateAnswers Danh sách các ký tự lựa chọn của thí sinh.
     */
    void calculateResult(const vector<Question*>& questions, const vector<char>& candidateAnswers);
    
    /**
     * @brief Xuất báo cáo tổng quan kết quả cuối cùng theo biểu mẫu bảng căn lề cố định.
     */
    void displayFinalReport() const;
};

ExamResult::ExamResult() 
    : startTime(""), duration(0), correctCount(0), totalScore(0.0), subject(""), totalQuestions(0) {}

void ExamResult::setCandidate(const Candidate& c) { candidate = c; }
void ExamResult::setStartTime(const string& time) { startTime = time; }
void ExamResult::setDuration(int d) { duration = d; }
void ExamResult::setSubject(const string& s) { subject = s; }
void ExamResult::setTotalQuestions(int total) { totalQuestions = total; }

void ExamResult::calculateResult(const vector<Question*>& questions, const vector<char>& candidateAnswers) 
{
    correctCount = 0;
    for (size_t i = 0; i < questions.size(); ++i) {
        // Kiểm tra chỉ mục an toàn và so khớp ký tự đáp án (đã đồng bộ viết hoa)
        if (i < candidateAnswers.size() && candidateAnswers[i] == questions[i]->getCorrectAnswer()) {
            ++correctCount;
        }
    }
    // Tính điểm dựa trên công thức tỉ lệ thuận hệ thang điểm 10
    if (totalQuestions > 0) {
        totalScore = static_cast<double>(correctCount) * 10.0 / totalQuestions;
    }
}

void ExamResult::displayFinalReport() const 
{
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                          EXAM RESULT                         ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";

    std::cout << "Student ID      : " << candidate.getId() << std::endl;
    std::cout << "Full Name       : " << candidate.getName() << std::endl;
    std::cout << "Subject         : " << subject << std::endl;
    std::cout << std::endl;

    std::cout << "Start Time      : " << startTime << std::endl;
    std::cout << "Duration        : " << duration << " seconds" << std::endl;
    std::cout << std::endl;

    std::cout << "Correct Answer  : "
        << correctCount << "/" << totalQuestions << std::endl;

    std::cout << "Score           : "
        << totalScore << "/10" << std::endl;

    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}