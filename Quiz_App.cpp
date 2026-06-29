#include <iostream>
#include <Windows.h> // 1. Thêm thư viện này để can thiệp vào hệ thống Windows
#include "QuizManager.h"

using namespace std;

int main() {
    // Ép Terminal hiển thị chữ có dấu (UTF-8)
    SetConsoleOutputCP(65001);  
    SetConsoleCP(65001);

    QuizManager manager;

    // Gọi hàm load dữ liệu từ file input.txt
    if (manager.loadQuestionFromFile("input.txt")) {
        // Kích hoạt luồng làm bài, chọn môn học, sửa câu hỏi và chấm điểm
        manager.startExam();
    } else {
        cout << "[Error] Unable to open or read data from file 'input.txt'!" << endl;
        cout << "Please double-check that the 'input.txt' file is located in the same directory as the executable file." << endl;
    }

    cout << "\nPress any key to continue . . . ";
    cin.ignore(); // Xóa bộ nhớ đệm thừa
    cin.get();
    return 0;
}