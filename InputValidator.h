#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

class InputValidator {
public:
    //Nhận và kiểm tra đáp án A,B,C,D,E,S
    static char getValidatedAnswer() {
        string input;
        while (true) {
            cout << "Nhập đáp án của bạn (A-E hoặc S để bỏ qua): ";

            // Sửa lỗi nếu luồng nhập std::cin bị hỏng
            if (!(cin >> input)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Lỗi nhập liệu!\n Vui lòng thử lại.\n";
                continue;
            }

            // Xóa mấy kí tự thừa
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Cho nhập 1 kí tự, nhiều hơn kh được, input[0]: tự chuyển qua in hoa
            if (input.length() == 1) {
                char ch = static_cast<char>(std::toupper(input[0]));

                // Kiểm tra, trùng thì cho chạy
                if (ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E' || ch == 'S') {
                    return ch; //Trả kết quả cho hệ thống
                }
            }

            // Thông báo lỗi nếu nhập sai
            cout << "Đáp án không hợp lệ!\n Chỉ chấp nhận các ký tự A, B, C, D, E hoặc S để Skip.\n";
        }
    }


    static bool validateQuestionIndex(int choiceIndex, int totalQuestions) {
        // Kiểm tra xem số có nằm trong số câu hỏi khong
        if (choiceIndex >= 1 && choiceIndex <= totalQuestions) {
            return true;
        }

        // Trả về false để thí sinh nhập lại
        return false;
    }
};
