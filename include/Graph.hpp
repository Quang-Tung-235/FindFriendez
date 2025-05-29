// Graph.hpp
#pragma once
#include "Student.hpp" // Graph quản lý các đối tượng Student
#include <string>      // Dùng cho ID, tên, và các chuỗi khác
#include <sstream>     // Dùng cho std::ostringstream để tạo chuỗi DOT
#include <iostream>    // Dùng cho std::cerr, std::cout

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
// CẢNH BÁO: Đây không phải là thực hành tốt nhất trong file header.
using namespace std;

// --- GIỚI HẠN KÍCH THƯỚC CỐ ĐỊNH CHO ĐỒ THỊ ---
// Số lượng sinh viên tối đa mà đồ thị có thể quản lý.
const int MAX_STUDENTS_IN_GRAPH = 100;

// --- LỚP GRAPH ---
// Lớp Graph dùng để biểu diễn mạng lưới sinh viên và các mối quan hệ bạn bè.
// Mỗi sinh viên là một nút (node), mỗi mối quan hệ bạn bè là một cạnh (edge).
class Graph {
public: // Các thành viên có thể truy cập từ bên ngoài lớp

    // --- MẢNG LƯU TRỮ SINH VIÊN (CÁC NÚT CỦA ĐỒ THỊ) ---
    // students_arr: Mảng cố định các đối tượng Student.
    // Đây là nơi lưu trữ toàn bộ thông tin của các sinh viên trong mạng xã hội.
    Student students_arr[MAX_STUDENTS_IN_GRAPH];
    // student_count: Số lượng sinh viên hiện tại đang có trong đồ thị.
    int student_count;

    // --- CONSTRUCTOR (HÀM KHỞI TẠO) ---
    // Được gọi khi một đối tượng Graph được tạo, dùng để khởi tạo đồ thị rỗng.
    Graph();

    // --- THAO TÁC VỚI SINH VIÊN (NÚT) ---
    // Thêm một đối tượng sinh viên `s_param` vào đồ thị.
    // Trả về: true nếu thêm thành công.
    //         false nếu đồ thị đã đầy hoặc ID của `s_param` đã tồn tại trong đồ thị.
    bool addStudent(const Student& s_param);

    // Xóa một sinh viên khỏi đồ thị dựa vào ID của sinh viên đó.
    // Việc xóa này cũng bao gồm việc xóa sinh viên đó khỏi danh sách bạn bè của tất cả các sinh viên khác.
    // Trả về: true nếu xóa thành công.
    //         false nếu không tìm thấy sinh viên có ID đó để xóa.
    bool removeStudent(const string& id_to_remove);

    // --- THAO TÁC VỚI QUAN HỆ BẠN BÈ (CẠNH) ---
    // Tạo một mối quan hệ bạn bè (vô hướng, hai chiều) giữa hai sinh viên có ID là `a_id` và `b_id`.
    // Trả về: true nếu kết bạn thành công.
    //         false nếu một trong hai ID không tồn tại, họ cố gắng tự kết bạn,
    //         hoặc danh sách bạn của một trong hai đã đạt giới hạn tối đa.
    bool addFriendship(const string& a_id, const string& b_id);

    // Xóa mối quan hệ bạn bè giữa hai sinh viên có ID là `a_id` và `b_id`.
    // Trả về: true nếu xóa thành công (ít nhất một chiều của tình bạn được xóa).
    //         false nếu một trong hai ID không tồn tại hoặc họ vốn không phải là bạn.
    bool removeFriendship(const string& a_id, const string& b_id);

    // --- TRUY XUẤT THÔNG TIN SINH VIÊN ---
    // Lấy một CON TRỎ tới đối tượng Student trong đồ thị, cho phép THAY ĐỔI thông tin của sinh viên đó.
    // `id`: ID của sinh viên cần tìm.
    // Trả về: Con trỏ tới đối tượng Student nếu tìm thấy.
    //         `nullptr` (con trỏ null) nếu không tìm thấy sinh viên nào có ID đó.
    // *Giải thích Con trỏ*: Con trỏ lưu trữ địa chỉ bộ nhớ. Trả về con trỏ cho phép
    // chúng ta làm việc trực tiếp với đối tượng Student gốc trong mảng `students_arr`,
    // thay vì làm việc với một bản sao.
    Student* getMutableStudent(const string& id);

    // Lấy một CON TRỎ HẰNG tới đối tượng Student, chỉ cho phép ĐỌC thông tin.
    // `const` Student*: Con trỏ này không cho phép thay đổi đối tượng Student mà nó trỏ tới.
    // `const` ở cuối hàm: Hàm này không thay đổi trạng thái của đối tượng Graph.
    // Trả về: Con trỏ hằng tới Student nếu tìm thấy, `nullptr` nếu không.
    const Student* getStudent(const string& id) const;

    // --- TIỆN ÍCH ---
    // Tạo và trả về một chuỗi string biểu diễn đồ thị ở định dạng DOT.
    // Định dạng DOT có thể được sử dụng bởi các công cụ như Graphviz để vẽ đồ thị.
    // `const` ở cuối: Hàm này không thay đổi trạng thái của đối tượng Graph.
    string toDot() const;

private: // Các thành viên chỉ có thể truy cập từ bên trong lớp Graph

    // Hàm tiện ích nội bộ: Tìm chỉ số của sinh viên trong mảng `students_arr` dựa trên ID.
    // Trả về: Chỉ số (0 đến student_count-1) nếu tìm thấy.
    //         -1 nếu không tìm thấy.
    // `const` ở cuối: Hàm này không thay đổi trạng thái của đối tượng Graph.
    int findStudentIndex(const string& id) const;
};