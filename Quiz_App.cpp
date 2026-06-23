#include <iostream>
#include "QuizManager.h" 

using namespace std;


int main() {
    QuizManager manager; // Khởi tạo một đối tượng quản lý bài thi tên là manager
    
    // Bước 1: Nạp dữ liệu từ file văn bản "input.txt" đặt cùng thư mục
    if (manager.loadQuestionFromFile("input.txt")) {
        // Bước 2: Nếu đọc file thành công, kích hoạt luồng thi trắc nghiệm ngay lập tức
        manager.startExam();
    }
    
    // Đoạn code giữ màn hình Console không bị tắt đột ngột sau khi in xong bảng điểm
    cout << "\nPress any key to continue . . . ";
    cin.ignore(); // Xóa bộ nhớ đệm thừa
    cin.get();    // Chờ người dùng nhấn một phím bất kỳ trên bàn phím để kết thúc đóng ứng dụng
    return 0;     // Trả về 0 báo hiệu chương trình kết thúc thành công, không gặp lỗi hệ thống
}