// DataTables.hpp
#pragma once
#include <string>
#include <iostream>

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
// CẢNH BÁO: Đây không phải là thực hành tốt nhất trong file header.
using namespace std;

// --- GIỚI HẠN KÍCH THƯỚC CỐ ĐỊNH CHO CÁC BẢNG ---
// Số lượng tối đa các sở thích/thói quen *duy nhất* mà hệ thống có thể quản lý.
const int MAX_UNIQUE_HOBBIES = 50;
const int MAX_UNIQUE_HABITS = 50;

// --- BẢNG SỞ THÍCH (HobbyTable) ---
// Dùng để lưu danh sách tất cả các tên sở thích duy nhất và ánh xạ chúng sang chỉ số (int).
// Ví dụ: "Đọc sách" -> 0, "Chơi game" -> 1, ...
struct HobbyTable {
    // list_arr: Mảng cố định lưu trữ tên các sở thích (dạng string).
    // Mỗi phần tử là một tên sở thích duy nhất.
    string list_arr[MAX_UNIQUE_HOBBIES];
    // count: Số lượng sở thích duy nhất hiện đang được lưu trong bảng.
    int count;

    // Constructor: Khởi tạo bảng rỗng khi một đối tượng HobbyTable được tạo.
    HobbyTable() {
        count = 0; // Ban đầu chưa có sở thích nào trong bảng.
    }

    // Tìm chỉ số (index) của một chuỗi sở thích `s` trong bảng.
    // Trả về chỉ số (0 đến count-1) nếu tìm thấy `s`.
    // Trả về -1 nếu `s` không có trong bảng.
    // `const` ở cuối: hàm này không thay đổi trạng thái của HobbyTable.
    int index(const string& s) const;

    // Thêm một chuỗi sở thích `s` vào bảng.
    // Nếu `s` đã tồn tại, trả về chỉ số hiện tại của nó.
    // Nếu `s` chưa tồn tại và bảng còn chỗ, thêm `s` vào và trả về chỉ số mới của nó.
    // Nếu `s` chưa tồn tại và bảng đã đầy, in cảnh báo và trả về -1.
    int add(const string& s);
};

// --- BẢNG THÓI QUEN (HabitTable) ---
// Cấu trúc và logic hoạt động tương tự như HobbyTable, nhưng dành cho thói quen.
struct HabitTable {
    string list_arr[MAX_UNIQUE_HABITS];
    int count;

    HabitTable() {
        count = 0;
    }
    int index(const string& s) const;
    int add(const string& s);
};