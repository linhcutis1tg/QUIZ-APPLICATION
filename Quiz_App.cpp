#include <iostream>
#include "QuizManager.h"

using namespace std;

int main() {
    // Sửa lỗi hiển thị tiếng Việt có dấu trên Terminal/Console của Windows
#if defined(_WIN32) || defined(_WIN64)
    system("chcp 65001 > nul");
#endif

    QuizManager manager;

    // Gọi hàm load dữ liệu từ file input.txt
    if (manager.loadQuestionFromFile("input.txt")) {
        // Kích hoạt luồng làm bài, chọn môn học, sửa câu hỏi và chấm điểm
        manager.startExam();
    } else {
        cout << "[Lỗi] Không thể mở hoặc đọc dữ liệu từ file 'input.txt'!" << endl;
        cout << "Vui lòng kiểm tra lại xem file 'input.txt' đã nằm cùng thư mục với file chạy chưa." << endl;
    }

    cout << "\nPress any key to continue . . . ";
    cin.ignore(); // Xóa bộ nhớ đệm thừa
    cin.get();
    return 0;
}