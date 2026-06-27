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
    std::string input;
    while (true) {
        if (optionCount > 0) 
        {
            std::cout << "Your answer (or 'S' to skip): ";
        }
        
        // Xử lý luồng lỗi nhập dữ liệu (Ví dụ: nhập ký tự EOF hoặc lỗi phần cứng dòng nhập)
        if (!(std::cin >> input)) 
        {
            std::cin.clear();   // Xóa cờ trạng thái lỗi của cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Xóa toàn bộ bộ đệm lỗi
            std::cout << "Input error! Please try again.\n";
            continue;
        }

        // Xóa phần ký tự thừa nếu người dùng vô tình gõ nhiều hơn một chữ cái
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Chỉ chấp nhận chuỗi nhập có độ dài đúng bằng 1 ký tự
        if (input.length() == 1) 
        {
            char ch = static_cast<char>(std::toupper(input[0]));
            if (ch == 'S') return ch;

            if (optionCount > 0) {
                // Kiểm tra ký tự nằm trong phạm vi từ 'A' đến giới hạn của câu hỏi đó
                if (ch >= 'A' && ch < 'A' + optionCount) return ch;
            } else {
                if (ch >= 'A' && ch <= 'Z') return ch;
            }
        }
        std::cout << "Invalid choice! Please enter a valid character.\n";
    }
}

//Hàm nhập lại đáp án
if (isAnswerValid) {
    char confirm;
    while (true) {
        std::cout << "Do you want to re-enter the answer? (Y/N): ";
        if (std::cin >> confirm) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            confirm = static_cast<char>(std::toupper(confirm));

            if (confirm == 'Y') {
                break; // Thoát vòng lặp hỏi để tiếp tục vòng lặp lớn (nhập lại)
            }
            if (confirm == 'N') {
                return ch; // Không chọn lại -> Trả về đáp án và kết thúc câu hỏi
            }
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Invalid selecion! Please select only Y (Yes) or N (No).\n";
    }

    // Nếu thoát ra bằng lệnh 'break' (chọn Y), câu lệnh continue này 
    // sẽ đưa người dùng quay trở lại dòng "Your answer..." để nhập lại câu đó.
    continue;
}
        }
        std::cout << "Invalid choice! Please enter a valid character.\n";
    }
}


bool InputValidator::validateQuestionIndex(int choiceIndex, int totalQuestions) 
{
    return choiceIndex >= 1 && choiceIndex <= totalQuestions;
}