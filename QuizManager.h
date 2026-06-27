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

/**
 * @brief Lớp điều phối trung tâm (Controller Class) điều khiển toàn bộ ứng dụng thi trắc nghiệm.
 * Quản lý vòng đời dữ liệu câu hỏi, giao tiếp file tệp tin, điều khiển luồng thi, cho phép xem lại và xuất kết quả.
 */
class QuizManager 
{
private:
    std::vector<Question*> questions;   // Danh sách các con trỏ quản lý câu hỏi (Đa hình Polymorphism)
    std::vector<char> candidateAnswers; // Mảng lưu trữ các đáp án do thí sinh lựa chọn ứng với từng câu hỏi
    int totalQuestions;                 // Tổng số lượng câu hỏi đã tải thành công của môn học
    std::string currentSubject;         // Môn học thi hiện tại được chọn
    Candidate currentCandidate;         // Đối tượng thông tin thí sinh hiện tại
    ExamResult finalResult;             // Đối tượng tổng hợp kết quả bài thi cuối cùng

public:
    // Khởi tạo trình quản lý thi ban đầu chưa có câu hỏi
    QuizManager();

    // Giải phóng vùng nhớ heap của các con trỏ câu hỏi để tránh rò rỉ bộ nhớ (Memory Leak)
    ~QuizManager();

    /**
     * @brief Tải cấu trúc danh sách môn học và phân tích bộ câu hỏi tương ứng từ tệp tin dữ liệu.
     * @param filename Tên file dữ liệu (Ví dụ: "input.txt").
     * @return true nếu tải dữ liệu môn học thành công, ngược lại false.
     */
    bool loadQuestionFromFile(const std::string& filename);

    /**
     * @brief Kích hoạt quy trình làm bài thi, đếm ngược thời gian và thu nhận đáp án tuần tự.
     */
    void startExam();

    /**
     * @brief Cho phép thí sinh xem lại đề thi, sửa đáp án tùy ý trước khi xuất kết quả cuối cùng.
     */
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
    // Hiển thị màn hình chào mừng ứng dụng và lấy thông tin thí sinh đầu vào
    std::cout << "*************************************************\n";
    std::cout << "* QUIZ PROGRAM                  *\n";
    std::cout << "* Date: " << TimeService::getCurrentDate() << "                             *\n";
    std::cout << "* Time: " << TimeService::getCurrentTime() << "                              *\n";
    std::cout << "*************************************************\n";
    std::cout << "Please enter your information:\n";
    std::cin >> currentCandidate;

    // Dọn dẹp dữ liệu cũ (nếu có) trước khi tải bài thi mới
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

    // BƯỚC 1: Đọc tiêu đề danh sách các môn học ở đầu tệp tin cho đến khi gặp dòng bắt đầu bằng chữ số
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (std::isdigit(line[0])) break;   // Gặp dòng chỉ mục câu hỏi -> dừng đọc danh sách môn
        subjects.push_back(line);
    }

    if (subjects.empty()) {
        std::cout << "No subjects found in file!\n";
        file.close();
        return false;
    }

    // BƯỚC 2: Hiển thị Menu chọn môn học trực quan cho người dùng
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

    std::cin.ignore(1000, '\n');    // Loại bỏ ký tự xuống dòng thừa
    file.clear();
    file.seekg(0);      // Di chuyển con trỏ đọc file về lại đầu tệp tin để tìm phân đoạn câu hỏi môn học

    currentSubject = subjects[choice - 1];
    std::string target = std::to_string(choice) + ". "; // Định dạng chuỗi tìm kiếm mục tiêu (Ví dụ: "1. ")
    bool found = false;

    // Tìm kiếm phân đoạn bắt đầu của môn học được chọn trong file
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

    // BƯỚC 3: Đọc tổng số câu hỏi cần tải cấu hình của phân đoạn môn học đó
    int numQuestions = 0;
    if (!(file >> numQuestions)) {
        file.close();
        return false;
    }
    file.ignore(1000, '\n');

    // Vòng lặp phân tích cú pháp chi tiết từng câu hỏi (ID, nội dung, số tùy chọn, các đáp án)
    int questionLoaded = 0;
    while (questionLoaded < numQuestions && std::getline(file, line)) {
        if (line.empty()) continue;

        // Đọc dòng chứa ID và Nội dung câu hỏi
        std::stringstream ss(line);
        int id;
        ss >> id;
        std::string content;
        std::getline(ss, content);
        if (!content.empty() && content[0] == ' ') content.erase(0, 1); // Xóa dấu cách thừa ở đầu nội dung câu hỏi
            
        // Đọc dòng chứa Số lượng phương án lựa chọn và chi tiết các phương án
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

        // Đọc dòng chứa Ký tự đáp án đúng cuối cùng câu hỏi
        std::string ansLine;
        if (!std::getline(file, ansLine)) break;
        std::stringstream ss3(ansLine);
        char ans;
        if (!(ss3 >> ans)) break;

        // Khởi tạo đối tượng câu hỏi cụ thể đưa vào danh sách quản lý đa hình đa lớp
        questions.push_back(new MultipleChoiceQuestion(id, content, options, static_cast<char>(std::toupper(ans))));
        questionLoaded++;
    }
    file.close();
    
    totalQuestions = static_cast<int>(questions.size());
    candidateAnswers.assign(totalQuestions, 'S'); // Khởi tạo mặc định ban đầu đáp án của thí sinh đều là 'S' (Skip)
    std::cout << "You chose: " <<currentSubject << std::endl;
    std::cout << "Total questions: " << totalQuestions << std::endl;
    
    return totalQuestions > 0;
}

void QuizManager::startExam() {
    std::cout << "\nPress Enter to start the exam...";
    std::cin.get();

    // Ghi nhận mốc thời gian hệ thống bắt đầu làm bài
    std::string startTime = TimeService::getCurrentTime();
    std::time_t startSecond = std::time(0);

    // Vòng lặp hiển thị và lấy đáp án cho từng câu hỏi
    for (int i = 0; i < totalQuestions; i++) {
        questions[i]->displayQuestion();
        int optionCount = questions[i]->getOptionCount();
        candidateAnswers[i] = InputValidator::getValidatedAnswer(optionCount);
    }

    // Ghi nhận mốc thời gian kết thúc bài thi và tính toán thời gian làm bài thực tế
    std::time_t endSecond = std::time(0);
    int duration = TimeService::calculateElapsedSeconds(startSecond, endSecond);

    // Đóng gói cấu hình chuyển giao kết quả đến lớp ExamResult để xử lý
    finalResult.setCandidate(currentCandidate);
    finalResult.setStartTime(startTime);
    finalResult.setDuration(duration);
    finalResult.setSubject(currentSubject);          
    finalResult.setTotalQuestions(totalQuestions);
    finalResult.calculateResult(questions, candidateAnswers);
    finalResult.setSubject(currentSubject);          
    finalResult.setTotalQuestions(totalQuestions);  
    std::cout << "\nYou have finished your exam in " << duration << " seconds.\n";
    reviewAndModifyAnswer();    // Chuyển tiếp sang màn hình hỗ trợ kiểm tra/sửa đổi đáp án bài làm
}

void QuizManager::reviewAndModifyAnswer() {
    while (true) {
        char choice;
        std::cout << "Do you want to review or change any answers? (Y/N): ";
        std::cin >> choice;
        choice = static_cast<char>(std::toupper(choice));

        if (choice != 'Y') break;   // Nếu người dùng không nhập 'Y', kết thúc vòng lặp để nộp bài luôn

        while (true) {
            int choiceIndex = 0;
            std::cout << "Enter the question number you want to change (1-" << totalQuestions << "): ";
            while (!(std::cin >> choiceIndex)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please enter a valid integer: ";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Kiểm tra số thứ tự nhập vào có nằm trong phạm vi mảng câu hỏi đề thi không
            if (!InputValidator::validateQuestionIndex(choiceIndex, totalQuestions)) {
                std::cout << "Question number out of range! Please try again.\n";
                continue;
            }

            int index = choiceIndex - 1;    // Quy đổi về chỉ mục mảng (bắt đầu từ 0)
            std::cout << "\n--- Reviewing Question " << choiceIndex << " ---";
            questions[index]->displayQuestion();

            std::cout << "Current saved answer: " << candidateAnswers[index] << "\n";
            std::cout << "Enter new choice ";
            candidateAnswers[index] = InputValidator::getValidatedAnswer(questions[index]->getOptionCount());
            std::cout << "Answer for question " << choiceIndex << " updated successfully!\n\n";
            break;
        }
    }
    
    // Tái tính toán lại điểm số cuối cùng sau khi đã điều chỉnh đáp án và in báo cáo kết quả ra màn hình
    finalResult.calculateResult(questions, candidateAnswers);
    finalResult.displayFinalReport();
}