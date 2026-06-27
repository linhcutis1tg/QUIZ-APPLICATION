#pragma once
#include <iostream>
#include <string>

/**
 * @brief Lớp tiện ích (Utility Class) cung cấp các phương thức tĩnh kiểm tra dữ liệu nhập từ bàn phím.
 * Không cho phép khởi tạo đối tượng (Constructor bị delete).
 */
class InputValidator {
public:
    InputValidator() = delete; // Khóa việc tạo instance vì class này chỉ cung cấp hàm Static tiện ích
    bool askToReenter();
    
    /**
     * @brief Ràng buộc dữ liệu đáp án nhập vào từ người dùng (Chỉ nhận ký tự đáp án hợp lệ hoặc 'S' để bỏ qua).
     * @param optionCount Số lượng phương án lựa chọn tối đa của câu hỏi (Ví dụ: 4 tương ứng A, B, C, D).
     * @return Ký tự đáp án hợp lệ đã được chuẩn hóa viết hoa.
     */
    static char getValidatedAnswer(int optionCount = 0);

    /**
     * @brief Kiểm tra số thứ tự câu hỏi người dùng muốn chỉnh sửa có nằm trong phạm vi cho phép hay không.
     */
    static bool validateQuestionIndex(int choiceIndex, int totalQuestions);
};

char InputValidator::getValidatedAnswer(int optionCount) 
{
    string input;
    while (true) {
        if (optionCount > 0) 
        {
            cout << "Your answer (or 'S' to skip): ";
        }
        
        // Xử lý luồng lỗi nhập dữ liệu (Ví dụ: nhập ký tự EOF hoặc lỗi phần cứng dòng nhập)
        if (!(cin >> input)) 
        {
            cin.clear();   // Xóa cờ trạng thái lỗi của cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa toàn bộ bộ đệm lỗi
            cout << "Input error! Please try again.\n";
            continue;
        }

        // Xóa phần ký tự thừa nếu người dùng vô tình gõ nhiều hơn một chữ cái
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Chỉ chấp nhận chuỗi nhập có độ dài đúng bằng 1 ký tự
        if (input.length() == 1) 
        {
            char ch = static_cast<char>(toupper(input[0]));
            if (ch == 'S') return ch;

            if (optionCount > 0) {
                // Kiểm tra ký tự nằm trong phạm vi từ 'A' đến giới hạn của câu hỏi đó
                if (ch >= 'A' && ch < 'A' + optionCount) return ch;
            } else {
                if (ch >= 'A' && ch <= 'Z') return ch;
            }
        }
        cout << "Invalid choice! Please enter a valid character.\n";
    }
}

bool InputValidator::validateQuestionIndex(int choiceIndex, int totalQuestions) 
{
    return choiceIndex >= 1 && choiceIndex <= totalQuestions;
}

//Cài hàm hỏi nhập lại
bool InputValidator::askToReenter()
{
    char choice;
    while (true) {
        std::cout << "Do you want to re-enter the answer? (Y/N): ";
        if (std::cin >> choice) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa bộ đệm thừa

            choice = static_cast<char>(std::toupper(choice));
            if (choice == 'Y') return true;  // Đồng ý nhập lại
            if (choice == 'N') return false; // Không nhập lại
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Invalid selection! Please enter only Y (Yes) hoac N (No).\n";
    }
}