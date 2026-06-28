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
using namespace std;

/**
 * @brief Lớp điều phối trung tâm (Controller Class) điều khiển toàn bộ ứng dụng thi trắc nghiệm.
 * Quản lý vòng đời dữ liệu câu hỏi, giao tiếp file tệp tin, điều khiển luồng thi, cho phép xem lại và xuất kết quả.
 */
class QuizManager 
{
private:
    vector<Question*> questions;   // Danh sách các con trỏ quản lý câu hỏi (Đa hình Polymorphism)
    vector<char> candidateAnswers; // Mảng lưu trữ các đáp án do thí sinh lựa chọn ứng với từng câu hỏi
    int totalQuestions;                 // Tổng số lượng câu hỏi đã tải thành công của môn học
    string currentSubject;         // Môn học thi hiện tại được chọn
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
    bool loadQuestionFromFile(const string& filename);

    /**
     * @brief Kích hoạt quy trình làm bài thi, đếm ngược thời gian và thu nhận đáp án tuần tự.
     */
    void startExam();

    /**
     * @brief Cho phép thí sinh xem lại đề thi, sửa đáp án tùy ý trước khi xuất kết quả cuối cùng.
     */
    void showMenu(int currentQuestion);
    void changeAnswer(int currentQuestion);
    void showAnswerSheet(int currentQuestion);
};

QuizManager::QuizManager() : totalQuestions(0) {}

QuizManager::~QuizManager() {
    for (Question* q : questions) {
        delete q;
    }
    questions.clear();
}
bool QuizManager::loadQuestionFromFile(const string& filename) 
{
    system("cls");
    // Hiển thị màn hình chào mừng ứng dụng và lấy thông tin thí sinh đầu vào
    cout << "================================================================\n";
    cout << "                      QUIZ EXAM SYSTEM                          \n";
    cout << "================================================================\n";
    cout << " Date : " << TimeService::getCurrentDate() <<"                Time : " << TimeService::getCurrentTime() << endl;
    cout << "================================================================\n";
    cin >> currentCandidate;

    // Dọn dẹp dữ liệu cũ (nếu có) trước khi tải bài thi mới
    for (Question* q : questions) {
        delete q;
    }
    questions.clear();
    candidateAnswers.clear();
    totalQuestions = 0;
    currentSubject.clear();

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << "!\n";
        return false;
    }

    vector<string> subjects;
    string line;

    // BƯỚC 1: Đọc tiêu đề danh sách các môn học ở đầu tệp tin cho đến khi gặp dòng bắt đầu bằng chữ số
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (isdigit(line[0])) break;   // Gặp dòng chỉ mục câu hỏi -> dừng đọc danh sách môn
        subjects.push_back(line);
    }

    if (subjects.empty()) {
        cout << "No subjects found in file!\n";
        file.close();
        return false;
    }

    // BƯỚC 2: Hiển thị Menu chọn môn học trực quan cho người dùng
    cout << "\n-----------SUBJECT LIST -----------\n";
    for (size_t i = 0; i < subjects.size(); i++) {
        cout << " " << i + 1 << ". " << subjects[i] << "\n";
    }
    cout << "-----------------------------------\n";

    int choice;
    while (true) {
        cout << "Select subject (1-" << subjects.size() << "): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        if (choice >= 1 && choice <= static_cast<int>(subjects.size())) break;
        cout << "Choice out of range! Select again.\n";
    }

    cin.ignore(1000, '\n');    // Loại bỏ ký tự xuống dòng thừa
    file.clear();
    file.seekg(0);      // Di chuyển con trỏ đọc file về lại đầu tệp tin để tìm phân đoạn câu hỏi môn học

    currentSubject = subjects[choice - 1];
    string target = to_string(choice) + ". "; // Định dạng chuỗi tìm kiếm mục tiêu (Ví dụ: "1. ")
    bool found = false;

    // Tìm kiếm phân đoạn bắt đầu của môn học được chọn trong file
    while (getline(file, line)) {
        if (line.find(target) == 0) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Subject section not found in file!\n";
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
    while (questionLoaded < numQuestions && getline(file, line)) {
        if (line.empty()) continue;

        // Đọc dòng chứa ID và Nội dung câu hỏi
        stringstream ss(line);
        int id;
        ss >> id;
        string content;
        getline(ss, content);
        if (!content.empty() && content[0] == ' ') content.erase(0, 1); // Xóa dấu cách thừa ở đầu nội dung câu hỏi
            
        // Đọc dòng chứa Số lượng phương án lựa chọn và chi tiết các phương án
        string optLine;
        if (!getline(file, optLine)) break; 
        stringstream ss2(optLine);
        int count;
        ss2 >> count;

        vector<string> options;
        for (int j = 0; j < count; j++) {
            string opt;
            if (!(ss2 >> opt)) break;
            options.push_back(opt);
        }

        // Đọc dòng chứa Ký tự đáp án đúng cuối cùng câu hỏi
        string ansLine;
        if (!getline(file, ansLine)) break;
        stringstream ss3(ansLine);
        char ans;
        if (!(ss3 >> ans)) break;

        // Khởi tạo đối tượng câu hỏi cụ thể đưa vào danh sách quản lý đa hình đa lớp
        questions.push_back(new MultipleChoiceQuestion(id, content, options, static_cast<char>(std::toupper(ans))));
        questionLoaded++;
    }
    file.close();
    
    totalQuestions = static_cast<int>(questions.size());
    candidateAnswers.assign(totalQuestions, 'S'); // Khởi tạo mặc định ban đầu đáp án của thí sinh đều là 'S' (Skip)
    cout << "\nYou chose: " <<currentSubject << endl;
    cout << "Total questions: " << totalQuestions << endl;
    
    return totalQuestions > 0;
}

void QuizManager::startExam() {
    cout << "\nPress Enter to start the exam...";
    cin.get();

    // Ghi nhận mốc thời gian hệ thống bắt đầu làm bài
    string startTime = TimeService::getCurrentTime();
    time_t startSecond = time(0);

    // Vòng lặp hiển thị và lấy đáp án cho từng câu hỏi
    for (int i = 0; i < totalQuestions; i++)
    {
        system("cls");
        cout << "\n----------------------------------------------------\n";
        cout << "  Question " << i + 1 << " / " << totalQuestions << endl;
        cout << "  Subject : " << currentSubject << endl;
        cout << "----------------------------------------------------\n";

        // questions[i] có kiểu dữ liệu là Question* (Lớp cha)
        questions[i]->displayQuestion();
        
        int optionCount = questions[i]->getOptionCount();

        candidateAnswers[i] = InputValidator::getValidatedAnswer(optionCount);

        // Skip thì qua luôn câu tiếp
        if (candidateAnswers[i] == 'S')
        {
            system("cls");
            continue;
        }

        showMenu(i);
    }
    //============================
    // Menu cuối trước khi nộp bài
    //============================

    while (true)
    {
        cout << "=====================================\n";
        cout << "          EXAM FINISHED\n";
        cout << "=====================================\n";
        cout << "1. Submit Exam\n";
        cout << "2. Change Answer\n";
        cout << "3. View Answer Sheet\n";
        cout << "=====================================\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            bool hasSkip = false;

            for (int i = 0; i < totalQuestions; i++)
            {
                if (candidateAnswers[i] == 'S')
                {
                    hasSkip = true;
                    break;
                }
            }

            if (hasSkip)
            {
                cout << "\n=====================================\n";
                cout << " You still have skipped questions!\n";
                cout << " Please change all skipped answers\n";
                cout << " before submitting the exam.\n";
                cout << "=====================================\n";

                cout << "Press Enter...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();

                system("cls");
                break;      // quay lại menu cuối
            }

            system("cls");
            goto SUBMIT;
        }

        case 2:
            changeAnswer(totalQuestions);
            system("cls");
            break;

        case 3:
            showAnswerSheet(totalQuestions);
            cout << "\nPress Enter...";
            cin.ignore();
            cin.get();
            system("cls");
            break;

        default:
            cout << "Invalid choice!\n";
        }
    }
    SUBMIT:
    // Ghi nhận mốc thời gian kết thúc bài thi và tính toán thời gian làm bài thực tế
    time_t endSecond = time(0);
    int duration = TimeService::calculateElapsedSeconds(startSecond, endSecond);

    // Đóng gói cấu hình chuyển giao kết quả đến lớp ExamResult để xử lý
    finalResult.setCandidate(currentCandidate);
    finalResult.setStartTime(startTime);
    finalResult.setDuration(duration);
    finalResult.setSubject(currentSubject);          
    finalResult.setTotalQuestions(totalQuestions);
    finalResult.calculateResult(questions, candidateAnswers);         
    finalResult.setTotalQuestions(totalQuestions);  
    cout << "\nYou have finished your exam in " << duration << " seconds.\n";
    finalResult.displayFinalReport();
}

void QuizManager::showMenu(int currentQuestion)
{
    while (true)
    {
        cout << "\n========================================\n";
        cout << "1. Continue\n";
        cout << "2. Change previous answer\n";
        cout << "3. View answer sheet\n";
        cout << "========================================\n";
        cout << "Choose: ";

        int choice;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("cls");
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            system("cls");
            return;

        case 2:
            changeAnswer(currentQuestion);
            system("cls");
            continue;

        case 3:
            showAnswerSheet(currentQuestion);

            cout << "\nPress Enter to continue...";
            cin.get();

            system("cls");
            return;

        default:
            cout << "Invalid choice!\n";
        }
    }
}

void QuizManager::changeAnswer(int currentQuestion)
{
    int index;
    int maxQuestion;

    if (currentQuestion >= totalQuestions)
        maxQuestion = totalQuestions;
    else
        maxQuestion = currentQuestion + 1;

    // Nhập số câu hỏi cần sửa
    while (true)
    {
        cout << "\nEnter question number (1-" << maxQuestion << "): ";

        if (!(cin >> index))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (index >= 1 && index <= maxQuestion)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid question number! Please try again.\n";
    }

    system("cls");

    questions[index - 1]->displayQuestion();

    cout << "\nCurrent answer: " << candidateAnswers[index - 1] << endl;

    candidateAnswers[index - 1] =
        InputValidator::getValidatedAnswer(
            questions[index - 1]->getOptionCount());

    cout << "\nAnswer updated successfully!";
    cout << "\nPress Enter to continue...";
    cin.get();
}
void QuizManager::showAnswerSheet(int currentQuestion)
{
    cout << "\n============= ANSWER SHEET =============\n";

    int limit;

    if (currentQuestion >= totalQuestions)
        limit = totalQuestions;
    else
        limit = currentQuestion + 1;

    for (int i = 0; i < limit; i++)
    {
        cout << "Question " << i + 1
            << " : "
            << candidateAnswers[i]
            << endl;
    }
    cout << "========================================\n";
}
