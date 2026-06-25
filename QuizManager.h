#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
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
        vector<Question*>question;
        vector<char>candidateAnswer;
        int totalQuestions;
        Candidate currentcandidate;
        ExamResult finalResult;
    public:
        QuizManager();
        ~QuizManager();
        bool loadQuestionFromFile(string filename);
        void startExam();
        void reviewAndModifyAnswer();

};
QuizManager::QuizManager() : totalQuestions(0)
{
    
}
QuizManager::~QuizManager()
{
    for (Question* q : question)
    {
        delete q;
    }
    question.clear();
}
bool QuizManager::loadQuestionFromFile(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Cannot open file!" << endl;
        return false;
    }
    int n;
    file >> n;
    file.ignore();
    for (int i = 0; i < n; i++)
    {
        int id;
        string line;
        getline(file, line);
        stringstream ss(line);
        ss >> id;
        string content;
        getline(ss, content);
        if (!content.empty() && content[0] == ' ')
            content.erase(0, 1);
        string optLine;
        getline(file, optLine);
        stringstream ss2(optLine);
        int count;
        ss2 >> count;
        vector<string> options;
        for (int j = 0; j < count; j++)
        {
            string opt;
            ss2 >> opt;
            options.push_back(opt);
        }
        char ans;
        file >> ans;
        file.ignore();

        question.push_back(
            new MultipleChoiceQuestion(id, content, options, ans)
        );
    }
    totalQuestions =(int)question.size();
    candidateAnswer.resize(totalQuestions,' ');
    return true;
}
void QuizManager::startExam() {
    currentcandidate.inputInfo();
    cout << "Nhấn Enter để bắt đầu";
    cin.get();
    string startTime = TimeService::getCurrentTime(); // xu ly moc thgian bat dau
    time_t startSecond = time(0); // luu moc bat dau
    for(int i = 0; i < totalQuestions; i++) { // duyet tu cau 1 den het cac cau hoi
        question[i]->displayQuestion(); // hien thi cau hoi va lua chon (late binding)
        cout <<"Chọn A/B/C/D (S để bỏ qua): ";
        candidateAnswer[i] = InputValidator::getValidatedAnswer(); //lay dap an cua thi sinh
    }
    time_t endSecond = time(0); // luu moc ket thuc
    int duration = TimeService::calculateElapsedSeconds(startSecond, endSecond);
    finalResult.setCandidate(currentcandidate);
    finalResult.setStartTime(startTime);
    finalResult.setDuration(duration);
    finalResult.calculateResult(question, candidateAnswer);
    cout << "Bạn đã hoàn thành bài thi lượt thi lần đầu tiên trong" << duration << endl;
    reviewAndModifyAnswer();
}
void QuizManager::reviewAndModifyAnswer()
{
    while (true)
    {    
        char choice;
        cout << "Do you want to change any answers? (Y/N): ";
        cin >> choice;
        choice = toupper(choice); //chuyen ki tu tu thuong sang in

        if (choice != 'Y')
         break;

        int choiceIndex;
        cout << "Nhập câu muốn sửa ";
        cin >> choiceIndex;
        if(!InputValidator::validateQuestionIndex( choiceIndex, totalQuestions))
        {
            cout << "Sai số câu. Vui lòng nhập lại!";
            continue;
        }
        int index = choiceIndex - 1;
        cout << "Xem lại câu số" << choiceIndex;
        question[index] -> displayQuestion();

        cout << "Đáp án hiện tại là: " <<candidateAnswer[index] << endl; 
        cout << "Nhập lại đáp án A/B/C/D (hoặc S để bỏ qua): ";
        candidateAnswer[index] = InputValidator::getValidatedAnswer();
        cout << "cập nhật đáp án câu " << choiceIndex << " thành công!";
    }
    finalResult.displayFinalReport(); //goi ham de show ket qua   
    }