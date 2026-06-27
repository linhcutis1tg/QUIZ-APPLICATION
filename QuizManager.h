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

#include <iostream>
using namespace std;

class QuizManager 
{
    private:
        vector<Question*> questions;
        vector<char> candidateAnswer;
        int totalQuestions;
        string currentSubject;
        Candidate currentcandidate;
        ExamResult finalResult;
    public:
        QuizManager() : totalQuestions(0) {}
        ~QuizManager();
        bool loadQuestionFromFile(string filename);
        void startExam();
        void reviewAndModifyAnswer();
        
};
QuizManager::~QuizManager()
{
    for (Question* q : questions)
    {
        delete q;
    }
    questions.clear();
}

 bool QuizManager::loadQuestionFromFile(string filename) // code đọc lại file input
{
    // Vẽ khung giao diện trang trí bắt mắt lúc mở app giống video mẫu
    cout << "*********************************\n";
    cout << "* Quiz Program                  *\n";
    cout << "* Date: " << TimeService::getCurrentDate() << "              *\n"; // Gọi hàm lấy ngày tự động
    // thời gian giờ:phút:giây
    // Mon toan : 10 cau
    // Mon Anh : 12 cau
    //cout << "* Number of questions: " << totalQuestions << "        *\n";
    cout << "*********************************\n";
    cout << "Please enter your information!\n";
    cin >> currentcandidate;
    for (Question* q : questions)
    {
        delete q;
    }
    questions.clear();
    candidateAnswer.clear();
    totalQuestions = 0;
    currentSubject.clear();
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Cannot open file!" << endl;
        return false;
    }

    vector<string> subjects;
    string line;

    // Đọc danh sách môn ở đầu file
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        // Gặp dòng "1. ..." thì dừng
        if (isdigit(line[0]))
            break;

        subjects.push_back(line);
    }
    if (subjects.empty())
    {
        cout << "No subjects found!\n";
        file.close();
        return false;
    }
    // Hiển thị menu
    cout << "\n====== CHOOSE SUBJECT ======\n";
    for (size_t i = 0; i < subjects.size(); i++)
    {
        cout << i + 1 << ". " << subjects[i] << endl;
    }

    int choice;

    while (true)
    {
        cout << "Select: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice >= 1 && choice <= (int)subjects.size())
            break;

        cout << "Invalid choice! Select again.\n";
    }

    cin.ignore(1000, '\n');
    file.clear();
    file.seekg(0);

    // Lưu tên môn hiện tại
    currentSubject = subjects[choice - 1];

    string target = to_string(choice) + ". ";

    bool found = false;

    while (getline(file, line))
    {
        if (line.find(target) == 0)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Subject not found!\n";
        file.close();
        return false;
    }
    // Đọc câu hỏi của môn đó
    int numQuestions = 0;
    if (!(file >> numQuestions))
    {
        file.close();
        return false;
    }
    file.ignore(1000, '\n');
    int questionLoaded = 0;
    //Đọc từng câu hỏi
    while(questionLoaded < numQuestions && getline(file, line)) {
        //nếu gặp dòng trống thì bỏ qua để đọc tiếp
        if (line.empty()) {
            continue;
        }
        stringstream ss(line);
        int id;
        ss >> id; // tách lấy stt của câu hoit
        string content;
        getline(ss, content); //phần còn lại là nội dung
        if(!content.empty() && content[0] == ' ') content.erase(0, 1); // xóa khoảng thừa đầu dòng
            
        string optLine;
        // Kiểm tra an toàn: Nếu file bị hết dòng đột ngột trước khi kịp đọc dòng options thì thoát loop
        if(!getline(file, optLine)) break; 
        stringstream ss2(optLine);
        int count;
        ss2 >> count; // Đọc số lượng các lựa chọn

        vector<string> options;
        for (int j = 0; j < count; j++)
        {
            string opt;

            if (!(ss2 >> opt))
                break;

            options.push_back(opt);
        }
        string ansLine;
        if(!getline(file, ansLine)) break;
        stringstream ss3(ansLine);
        char ans;
        if (!(ss3 >> ans))
            break;
        // Viết hoa đáp án đúng bằng toupper() trước khi khởi tạo đối tượng câu hỏi trắc nghiệm mới
        questions.push_back(new MultipleChoiceQuestion(id, content, options, static_cast<char>(toupper(ans))));
        questionLoaded++; // Tăng số lượng câu hỏi đã nạp thành công lên 1
    }
    file.close();
    
    totalQuestions =(int)questions.size();
    candidateAnswer.assign(totalQuestions, 'S'); // Khởi tạo danh sách câu trả lời của thí sinh mặc định là 'S' (Skip)
    return totalQuestions > 0; // Trả về true nếu nạp thành công ít nhất 1 câu hỏi
}
void QuizManager::startExam() {

    
    
   
    cout << "Press enter to start...";
    cin.get();

    string startTime = TimeService::getCurrentTime();
    time_t startSecond = time(0);

    for (int i = 0; i < totalQuestions; i++) {
        questions[i]->displayQuestion();
        int optionCount = questions[i]->getOptionCount();
        candidateAnswer[i] = InputValidator::getValidatedAnswer(optionCount);
    }

    time_t endSecond = time(0);
    int duration = TimeService::calculateElapsedSeconds(startSecond, endSecond);

    finalResult.setCandidate(currentcandidate);
    finalResult.setStartTime(startTime);
    finalResult.setDuration(duration);
    finalResult.setSubject(currentSubject);          
    finalResult.setTotalQuestions(totalQuestions);
    finalResult.calculateResult(questions, candidateAnswer);

    cout << "You have finished your first attempt in " << duration << " seconds." << endl;
    reviewAndModifyAnswer();
}

void QuizManager::reviewAndModifyAnswer()
{
    while (true)
    {
        char choice;
        cout << "Do you want to change any answers? (Y/N): ";
        cin >> choice;
        choice = toupper(choice);

        if (choice != 'Y')
            break;

        while (true)
        {
            int choiceIndex = 0;
            cout << "Enter the question number you want to change:  ";
            while (!(cin >> choiceIndex))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid interger: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!InputValidator::validateQuestionIndex(choiceIndex, totalQuestions))
            {
                cout << "Invalid question number! Please enter again..." << endl;
                continue;
            }

            int index = choiceIndex - 1;
            cout << "Reviewing question no." << choiceIndex << endl;
            questions[index]->displayQuestion();

            cout << "Current answer  is: " << candidateAnswer[index] << endl;
            cout << "Re-select A/B/C/D (or S to skip): ";
            candidateAnswer[index] = InputValidator::getValidatedAnswer();
            cout << "Updating answer of question no." << choiceIndex << " successfully!" << endl;
            break;
        }
    }
    finalResult.setSubject(currentSubject);
    finalResult.setTotalQuestions(totalQuestions);
    finalResult.calculateResult(questions, candidateAnswer);
    finalResult.displayFinalReport();
}