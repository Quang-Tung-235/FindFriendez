// Recommender.hpp
#pragma once
#include "Graph.hpp" // Recommender cần truy cập vào Graph để lấy thông tin
#include <string>
#include <iostream> // Cho việc in ra màn hình

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
// CẢNH BÁO: Đây không phải là thực hành tốt nhất trong file header.
using namespace std;

// --- CẤU TRÚC CANDIDATE ---
// Dùng để lưu thông tin của một sinh viên được đề xuất làm bạn, cùng với điểm số gợi ý.
struct Candidate {
    string id;    // ID của sinh viên được gợi ý
    double score; // Điểm số đánh giá mức độ phù hợp của gợi ý
};

// --- LỚP RECOMMENDER ---
// Lớp này chứa logic để gợi ý bạn bè.
class Recommender {
public:
    // --- THUỘC TÍNH ---
    // graph_ LÀ MỘT THAM CHIẾU HẰNG (const Graph&) TỚI ĐỐI TƯỢNG Graph.
    // "Tham chiếu" (&) nghĩa là graph_ không phải là một bản sao của đồ thị,
    // mà nó "trỏ" trực tiếp tới đối tượng Graph gốc được truyền vào khi tạo Recommender.
    // "const" đảm bảo rằng Recommender không thể thay đổi đồ thị này, chỉ có thể đọc dữ liệu từ nó.
    // Điều này quan trọng vì Recommender chỉ nên phân tích đồ thị chứ không sửa đổi nó.
    const Graph& graph_; // Tham chiếu đến đồ thị mà Recommender sẽ làm việc trên đó.

    // --- CONSTRUCTOR ---
    // Hàm khởi tạo cho lớp Recommender.
    // Nó nhận một tham chiếu hằng tới một đối tượng Graph.
    // `explicit` ngăn chặn việc chuyển đổi kiểu ngầm không mong muốn sang Recommender.
    // Dấu ':' sau khai báo constructor và trước graph_(g) là cách để khởi tạo thành viên graph_
    // bằng giá trị g được truyền vào. Đây gọi là "initializer list".
    explicit Recommender(const Graph& g) : graph_(g) {}

    // --- HÀM GỢI Ý CHÍNH ---
    // Gợi ý bạn bè cho sinh viên có ID là `uid`.
    // `k`: Số lượng gợi ý tối đa muốn nhận.
    // `weightMutual`: Trọng số cho yếu tố "bạn chung".
    // `weightJaccard`: Trọng số cho yếu tố "tương đồng sở thích/thói quen".
    //
    // THAY ĐỔI: Thay vì trả về một vector (cần cấp phát động), hàm này sẽ
    // in trực tiếp các gợi ý ra màn hình để giữ sự đơn giản theo yêu cầu.
    // Nếu muốn trả về kết quả, bạn cần truyền vào một mảng Candidate và kích thước của nó
    // để hàm này điền vào, hoặc định nghĩa một kích thước trả về cố định.
    void suggestAndPrint(const string& uid, int k, double weightMutual, double weightJaccard) const;

    // (Nếu bạn muốn trả về kết quả thay vì in, có thể khai báo như sau,
    //  và người gọi sẽ cung cấp mảng 'results' và 'max_results_size')
    // void suggest(const string& uid, int k, double weightMutual, double weightJaccard,
    //              Candidate results[], int& actual_results_count, int max_results_size) const;

private:
    // Các hàm tiện ích hoặc hằng số có thể được đặt ở đây nếu cần.
    // Ví dụ: kích thước tối đa cho các mảng tạm thời trong quá trình gợi ý.
    // const static int MAX_TEMP_CANDIDATES = 200; // Giới hạn số ứng viên xem xét
    // const static int MAX_TEMP_SEEN = MAX_STUDENTS_IN_GRAPH + MAX_STUDENTS_IN_GRAPH; // Giới hạn mảng seen
};