// CLI.hpp
#pragma once
#include "Graph.hpp"        // CLI tương tác với Graph
#include "Recommender.hpp"  // CLI sử dụng Recommender
#include "DataTables.hpp"   // CLI cần truy cập HobbyTable, HabitTable để hiển thị tên
#include <string>
#include <iostream>

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
// CẢNH BÁO: Đây không phải là thực hành tốt nhất trong file header.
using namespace std;

// --- LỚP CLI ---
// Lớp CLI quản lý giao diện người dùng thông qua dòng lệnh.
// Nó cho phép người dùng thực hiện các chức năng như thêm sinh viên, kết bạn, xem gợi ý,...
class CLI {
public:
    // --- THUỘC TÍNH (Tham chiếu đến các thành phần khác) ---
    // CLI cần truy cập và thao tác trên các đối tượng này, nên chúng được lưu trữ dưới dạng tham chiếu.
    // Tham chiếu (&) cho phép CLI làm việc trực tiếp với các đối tượng gốc được tạo trong main(),
    // thay vì tạo bản sao. Điều này đảm bảo mọi thay đổi được phản ánh đúng.
    Graph& graph_;             // Tham chiếu đến đối tượng Graph chính.
    HobbyTable& hobby_table_;  // Tham chiếu đến bảng sở thích.
    HabitTable& habit_table_;  // Tham chiếu đến bảng thói quen.
    Recommender& recommender_; // Tham chiếu đến bộ gợi ý.

    // --- CONSTRUCTOR ---
    // Hàm khởi tạo cho CLI.
    // Nhận tham chiếu đến các thành phần dữ liệu và logic chính của chương trình.
    CLI(Graph& g, HobbyTable& hb_tbl, HabitTable& ha_tbl, Recommender& recomm);

    // --- HÀM CHẠY CHÍNH ---
    // Bắt đầu vòng lặp chính của giao diện dòng lệnh, hiển thị menu và xử lý lựa chọn.
    void run();

private: // Các hàm và thuộc tính chỉ dùng nội bộ trong lớp CLI.

    // --- CÁC HÀM XỬ LÝ LUỒNG CHỨC NĂNG ---
    // Mỗi hàm này tương ứng với một lựa chọn trong menu.

    // Hiển thị menu chính và điều phối đến các hàm xử lý tương ứng.
    void menu();

    // Luồng xử lý cho việc nhập thông tin và thêm một sinh viên mới.
    void addStudentFlow();

    // Luồng xử lý cho việc nhập ID của hai sinh viên và tạo mối quan hệ bạn bè giữa họ.
    void addFriendshipFlow();

    // Luồng xử lý cho việc nhập ID một sinh viên và hiển thị các gợi ý bạn bè cho họ.
    void suggestFlow();

    // Luồng xử lý cho việc xuất đồ thị hiện tại ra file định dạng DOT (graph.dot).
    void exportDotFlow();

    // Luồng xử lý cho việc liệt kê tất cả sinh viên trong đồ thị cùng thông tin chi tiết.
    void listStudentsFlow();

    // Luồng xử lý cho việc nhập ID của hai sinh viên và xóa mối quan hệ bạn bè giữa họ.
    void removeFriendshipFlow();
};