// Student.hpp
#pragma once // Đảm bảo file này chỉ được include một lần

#include <string>   // Để sử dụng kiểu dữ liệu string (chuỗi ký tự)
#include <iostream> // Để sử dụng cout, cerr (xuất/lỗi chuẩn)

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
// CẢNH BÁO: Đây không phải là thực hành tốt nhất trong file header.
using namespace std;

// --- GIỚI HẠN KÍCH THƯỚC CỐ ĐỊNH ---
// Các hằng số này xác định số lượng tối đa mà mỗi đối tượng Student có thể lưu trữ.
const int MAX_HOBBIES_PER_STUDENT = 10;
const int MAX_HABITS_PER_STUDENT = 10;
const int MAX_FRIENDS_PER_STUDENT = 20;

// --- CẤU TRÚC STUDENT ---
// Struct 'Student' dùng để đóng gói tất cả thông tin liên quan đến một sinh viên.
struct Student {
    // --- THUỘC TÍNH CƠ BẢN ---
    string id;   // Mã số sinh viên, dùng để định danh duy nhất mỗi sinh viên.
    string name; // Tên hiển thị của sinh viên.

    // --- SỞ THÍCH ---
    // hobbies_arr: Mảng cố định, lưu trữ các CHỈ SỐ (kiểu int) của các sở thích.
    // Mỗi chỉ số này sẽ tương ứng với một sở thích cụ thể trong HobbyTable.
    // Kích thước của mảng này được xác định bởi MAX_HOBBIES_PER_STUDENT.
    int hobbies_arr[MAX_HOBBIES_PER_STUDENT];
    // hobbies_count: Biến này theo dõi số lượng sở thích *thực tế* mà sinh viên hiện đang có.
    // Giá trị của nó luôn nhỏ hơn hoặc bằng MAX_HOBBIES_PER_STUDENT.
    int hobbies_count;

    // --- THÓI QUEN ---
    // Tương tự như sở thích, nhưng dùng cho thói quen.
    // habits_arr lưu các chỉ số thói quen từ HabitTable.
    int habits_arr[MAX_HABITS_PER_STUDENT];
    int habits_count;

    // --- BẠN BÈ ---
    // friends_arr: Mảng cố định, lưu trữ các ID (dạng string) của những sinh viên khác là bạn của sinh viên này.
    string friends_arr[MAX_FRIENDS_PER_STUDENT];
    // friends_count: Số lượng bạn bè *thực tế* mà sinh viên hiện đang có.
    int friends_count;
     Student();                                      // định nghĩa ở Student.cpp

    bool addHobby (int  hobby_idx);
    bool addHabit (int  habit_idx);

    bool addFriend    (const string& friend_id);
    bool removeFriend (const string& friend_id);
    bool isFriend     (const string& other_id) const;
};


  