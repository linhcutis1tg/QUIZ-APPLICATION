# QUIZ-APPLICATION
Đồ án 3: Ứng dụng trắc nghiệm (Quiz Application)

Dự án bao gồm các chức năng cốt lõi sau:

Tải bộ câu hỏi và đáp án từ một tệp văn bản (.txt).

Hiển thị: Tổng số lượng câu hỏi và thời gian làm bài hiện tại.

Cho phép thí sinh nhập thông tin cá nhân.

Hiển thị câu hỏi và cho phép thí sinh nhập câu trả lời cho từng câu hỏi.

Cho phép thí sinh:

Bỏ qua câu hỏi và trả lời chúng sau.

Thay đổi/chỉnh sửa câu trả lời của mình bất kỳ lúc nào.

Hiển thị kết quả bài thi, bao gồm: Thông tin thí sinh, Thời gian bắt đầu thi, Tổng thời gian làm bài, Số câu trả lời đúng, Tổng số điểm.


1. Class Question (Quản lý từng câu hỏi lẻ)
   
      Thuộc tính (Attributes):

            id: Mã câu hỏi (int)

            content: Nội dung câu hỏi (string)

            options: Danh sách các lựa chọn A, B, C, D (Mảng hoặc Vector string)

            correctAnswer: Đáp án chính xác (char: 'A', 'B', 'C', 'D')

      Phương thức (Methods):

            Constructor khởi tạo câu hỏi.

            displayQuestion(): In câu hỏi và các lựa chọn ra màn hình.

            Các hàm Getter/Setter cần thiết.

2. Class Candidate (Quản lý thông tin thí sinh)
   
      Thuộc tính (Attributes):

            id: Số báo danh / Mã sinh viên (string)

            name: Họ và tên (string)

      Phương thức (Methods):

            Constructor.

            inputInfo(): Nhập thông tin từ bàn phím.

3. Class ExamResult (Quản lý kết quả bài thi)
   
      Thuộc tính (Attributes):

            candidate: Đối tượng thuộc class Candidate

            startTime: Thời gian bắt đầu (string/time_t)

            duration: Tổng thời gian làm bài tính bằng giây (int)

            correctCount: Số câu trả lời đúng (int)

            totalScore: Tổng điểm đạt được (double)

      Phương thức (Methods):

            calculateResult(vector<Question> questions, vector<char> candidateAnswers): Hàm tính toán số câu đúng và điểm số dựa trên đáp án của thí sinh đối chiếu với đáp án gốc.

            displayFinalReport(): In ra màn hình bảng kết quả chuẩn chỉnh như cuối video mẫu.

4. Class QuizManager (Bộ não của chương trình - Điều khiển luồng)
   
      Thuộc tính (Attributes):

            questions: Danh sách toàn bộ câu hỏi tải từ file (vector<Question>)

            candidateAnswers: Danh sách câu trả lời của thí sinh (mảng lưu các ký tự 'A','B','C','D' hoặc 'S' nếu bỏ qua).

            totalQuestions: Tổng số câu hỏi (đọc từ file hoặc lấy size của vector).

      Phương thức (Methods):

            loadQuestionsFromFile(string filename): Đọc file .txt, phân tách dữ liệu để nạp vào vector questions.

            startExam(): Vòng lặp chính hiển thị từng câu hỏi, nhận input 'A/B/C/D' hoặc 'S' (Skip) từ thí sinh.

            reviewAndModifyAnswers(): Xử lý tính năng "Do you want to change any answers? (Y/N)". Nếu chọn Y, cho phép nhập số thứ tự câu hỏi để sửa lại đáp án.

5. Class TimeService (Thời gian)

      Thuộc tính (Attributes):

            Không có thuộc tính. (Do chỉ làm nhiệm vụ tính toán và định dạng thời gian tức thời, tránh lãng phí tài nguyên bộ nhớ).

      Các phương thức (Static Methods):

            getCurrentDate():
               Kiểu trả về: string

               Chức năng: Truy cập thời gian hiện tại của máy tính, bóc tách lấy Ngày/Tháng/Năm để trả về chuỗi định dạng (ví dụ: 19/6/2026). Phương thức này được dùng để in thông tin trên khung tiêu đề lúc vừa mở app.

            getCurrentTime():

               Kiểu trả về: string

               Chức năng: Bóc tách Giờ:Phút:Giây của hệ thống để trả về chuỗi định dạng (ví dụ: 21:3:21). Lớp QuizManager sẽ gọi hàm này ngay khi thí sinh nhấn phím bắt đầu làm bài để lưu mốc thời gian.

            calculateElapsedSeconds(time_t start, time_t end):

               Kiểu trả về: int

               Tham số truyền vào: time_t start (thời điểm bắt đầu dạng tích tắc giây) và time_t end (thời điểm kết thúc).

               Chức năng: Sử dụng hàm tính khoảng cách thời gian (difftime) để trả về chính xác số giây mà thí sinh đã trải qua trong suốt bài thi (ví dụ: 40 seconds). Kết quả này sẽ được gán vào dữ liệu của lớp ExamResult để in ra báo cáo cuối cùng.


6. Class InputValidator (Kiểm tra dữ liệu nhập)
   
      Thuộc tính (Attributes):

            Không có thuộc tính. 

            * Các phương thức (Static Methods):
            * getValidatedAnswer():

            Kiểu trả về: char (Luôn trả về ký tự viết hoa)

       Chức năng: Bật vòng lặp vô hạn while(true) để nhận ký tự nhập vào từ thí sinh.

            Nếu thí sinh nhập các ký tự đáp án hợp lệ tương ứng với file dữ liệu như a, b, c, d, e (hoặc viết hoa A, B, C, D, E) hoặc chọn bỏ qua bằng ký tự s / S (Skip), phương thức sẽ tự động viết hoa ký tự đó bằng hàm toupper() và trả kết quả về cho QuizManager lưu trữ.

            Nếu thí sinh cố tình nhập sai (ví dụ nhập số, nhập chuỗi dài hoặc ký tự lạ như X, Y, Z), phương thức sẽ chặn lại, in thông báo lỗi và bắt nhập lại cho đến khi đúng định dạng mới giải phóng vòng lặp.

            validateQuestionIndex(int choiceIndex, int totalQuestions):

                  Kiểu trả về: bool

                  Tham số truyền vào: Số thứ tự câu hỏi thí sinh muốn sửa (choiceIndex) và tổng số câu hỏi hiện có trong đề thi (totalQuestions).

                  Chức năng: Được gọi bên trong tính năng sửa đáp án (Do you want to change any answers?). Hàm này kiểm tra xem số thứ tự nhập vào có nằm trong phạm vi hợp lệ hay không (ví dụ: từ 1 đến 5). Nếu nằm ngoài phạm vi, hàm trả về false để hệ thống cảnh báo thí sinh nhập lại, tránh việc truy cập sai vùng chỉ số mảng gây sập chương trình.


