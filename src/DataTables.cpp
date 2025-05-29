// DataTables.cpp
#include "DataTables.hpp" // Include file header tương ứng

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
using namespace std;

// --- TRIỂN KHAI CÁC HÀM CHO HobbyTable ---

// Hàm tìm chỉ số của một sở thích trong bảng.
// s: Tên sở thích cần tìm.
// Trả về: Chỉ số nếu tìm thấy, -1 nếu không.
int HobbyTable::index(const string& s) const {
    // Duyệt qua tất cả các sở thích hiện có trong bảng (từ chỉ số 0 đến count-1).
    // Đây là phương pháp tìm kiếm tuyến tính (linear search).
    for (int i = 0; i < count; ++i) {
        if (list_arr[i] == s) { // So sánh chuỗi s với từng phần tử trong mảng.
            return i; // Tìm thấy, trả về chỉ số hiện tại.
        }
    }
    return -1; // Duyệt hết mảng mà không tìm thấy.
}

// Hàm thêm một sở thích vào bảng.
// s: Tên sở thích cần thêm.
// Trả về: Chỉ số của sở thích trong bảng (nếu thành công), -1 nếu thất bại (bảng đầy).
int HobbyTable::add(const string& s) {
    // Bước 1: Kiểm tra xem sở thích `s` đã có trong bảng chưa.
    int existing_idx = index(s);
    if (existing_idx != -1) {
        // Nếu đã có, không cần thêm lại, chỉ cần trả về chỉ số hiện tại của nó.
        return existing_idx;
    }

    // Bước 2: Nếu sở thích `s` chưa có, kiểm tra xem bảng còn chỗ trống để thêm không.
    if (count < MAX_UNIQUE_HOBBIES) {
        // Nếu còn chỗ (số lượng hiện tại `count` nhỏ hơn kích thước tối đa).
        list_arr[count] = s; // Thêm `s` vào vị trí cuối cùng còn trống trong mảng.
        count++;             // Tăng biến đếm số lượng sở thích trong bảng.
        return count - 1;    // Trả về chỉ số mới của `s` (chính là `count` trước khi tăng).
    } else {
        // Nếu bảng đã đầy.
        cerr << "CANH BAO: Bang HobbyTable da day! Khong the them so thich '" << s << "'." << endl;
        return -1; // Báo hiệu không thêm được.
    }
}

// --- TRIỂN KHAI CÁC HÀM CHO HabitTable ---
// Logic hoàn toàn tương tự như HobbyTable.

int HabitTable::index(const string& s) const {
    for (int i = 0; i < count; ++i) {
        if (list_arr[i] == s) {
            return i;
        }
    }
    return -1;
}

int HabitTable::add(const string& s) {
    int existing_idx = index(s);
    if (existing_idx != -1) {
        return existing_idx;
    }

    if (count < MAX_UNIQUE_HABITS) {
        list_arr[count] = s;
        count++;
        return count - 1;
    } else {
        cerr << "CANH BAO: Bang HabitTable da day! Khong the them thoi quen '" << s << "'." << endl;
        return -1;
    }
}