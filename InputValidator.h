#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

class InputValidator {
public:
    static char getValidatedAnswer(int optionCount = 0) {
        string input;
        while (true) {
            if (optionCount > 0) {
                cout << "Nhập đáp án của bạn (S để bỏ qua): ";
            }
            // nếu người dùng trả lời:
                // người dùng có muốn sửa đáp án hay chuyển đến câu tiếp 
            if (!(cin >> input)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Lỗi nhập liệu! Vui lòng thử lại.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (input.length() == 1) {
                char ch = static_cast<char>(std::toupper(input[0]));
                if (ch == 'S') {
                    return ch;
                }

                if (optionCount > 0) {
                    if (ch >= 'A' && ch < 'A' + optionCount) {
                        return ch;
                    }
                } else if ((ch >= 'A' && ch <= 'Z')) {
                    return ch;
                }
            }

            cout << "Đáp án không hợp lệ! Vui lòng nhập đúng ký tự lựa chọn.\n";
        }
    }

    static bool validateQuestionIndex(int choiceIndex, int totalQuestions) {
        return choiceIndex >= 1 && choiceIndex <= totalQuestions;
    }
};
