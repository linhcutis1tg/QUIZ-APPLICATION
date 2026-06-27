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
    //cout << "* Number of questions: " << totalQuestions << "        *\n";
    cout << "*********************************\n";
    cout << "Please enter your information!\n";
    cin >> currentcandidate;
    char choice;
    // Lap chon mon hoc
    while (true) {
        cout << "\n====== CHOOSE SUBJECT ======\n";
        cout << "1. Math\n2. English\n3. Literature\n";
        cout << "Select (1-3): ";
        cin >> choice;
        cin.ignore(1000, '\n'); //xóa bộ đệm để tránh trôi lệnh đọc chuỗi
        
        if (choice >= '1' && choice <= '3') { //chọn đúng thì thoát vòng lặp
            break;
        }
        cout << "Invalid choice! Select again.\n";
    }

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Cannot open file!" << endl;
        return false;
    }
    // đọc chủ đề 
    string line, target = string(1, choice) + ". ";
    
    // duyệt từng dòng để tìm tiêu đề môn học cần hiển thị
    while (getline(file, line)) {
        // Kiểm tra dòng chứa dấu ". " và ký tự đầu tiên là chữ số
        if (line.find(". ") != string::npos && isdigit(line[0])) { // nếu vị trí tìm khác giá trị "ko tìm thấy" -> tìm thấy dòng có chưa ". "
            //Nếu dòng đó khớp chính xác với mã môn học cần tìm thì dừng lại
            if (line.find(target) != string::npos) {
                break;
            }
        }
    }
    // Đọc câu hỏi của môn đó
    int numQuestions = 0;
    if(!(file >> numQuestions)) return false;
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
        for (int j = 0; j < count; j++) {
            string opt;
            ss2 >> opt;
            options.push_back(opt);
        }
        string ansLine;
        if(!getline(file, ansLine)) break;
        stringstream ss3(ansLine);
        char ans;
        ss3 >> ans;
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

    finalResult.calculateResult(questions, candidateAnswer);
    finalResult.displayFinalReport();
}