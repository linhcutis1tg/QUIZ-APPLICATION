#pragma once
#include <iostream>
#include <string>
#include <vector>


/**
 * @brief Lớp trừu tượng (Abstract Class) đại diện cho một Câu hỏi tổng quát.
 * Cung cấp khuôn mẫu cơ bản và tính đa hình cho các loại câu hỏi cụ thể.
 */
class Question 
{
protected:
    int id;                             // Mã số / Thứ tự câu hỏi
    string content;                // Nội dung câu hỏi
    vector<string> options;   // Danh sách các lựa chọn (A, B, C, D...)
    char correctAnswer;                 // Đáp án đúng của câu hỏi (A/B/C/D)

public:
    // Hàm khởi tạo lưu trữ thông tin câu hỏi cơ bản
    Question(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer);
    
    // Hàm hủy ảo (Virtual Destructor) rất quan trọng để tránh rò rỉ bộ nhớ khi giải phóng lớp con qua con trỏ lớp cha
    virtual ~Question() = default;

    /**
     * @brief Hàm thuần ảo hiển thị câu hỏi. Lớp con bắt buộc phải triển khai lại (Override).
     */
    virtual void displayQuestion() const = 0;

    // Các hàm tiện ích lấy thông tin câu hỏi
    int getID() const;
    char getCorrectAnswer() const;
    int getOptionCount() const;
};

/**
 * @brief Lớp MultipleChoiceQuestion kế thừa từ Question.
 * Chuyên trách việc xử lý hiển thị câu hỏi dưới dạng trắc nghiệm lựa chọn đơn câu.
 */
class MultipleChoiceQuestion : public Question 
{
public:
    MultipleChoiceQuestion(int id, const std::string& content, const std::vector<std::string>& options, char correctAnswer);
    
    /**
     * @brief Triển khai chi tiết cách hiển thị câu hỏi trắc nghiệm kèm theo ký tự đầu dòng (A., B., C...)
     */
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
    cout << "\nQuestion " << id << ": " << content << "\n";
    char optionName = 'A'; // Ký tự đại diện bắt đầu từ 'A'
    for (const string& option : options) {
        cout << "  " << optionName << ". " << option << "\n";
        ++optionName; // Tăng dần ký tự lên 'B', 'C', 'D'...
    }
}