#include "Student.hpp"
#include <iostream>   // nếu bạn vẫn muốn ghi log warn / error
using namespace std;  // giữ nguyên style dự án
// --- CONSTRUCTOR (HÀM KHỞI TẠO) ---
    // Đây là một hàm đặc biệt được tự động gọi mỗi khi một đối tượng 'Student' mới được tạo.
    // Mục đích của nó là để thiết lập các giá trị ban đầu (mặc định) cho các thuộc tính của đối tượng.
    Student::Student() {
        id = "";    // Khởi tạo ID là một chuỗi rỗng.
        name = "";  // Khởi tạo tên là một chuỗi rỗng.
        hobbies_count = 0; // Khi mới tạo, sinh viên chưa có sở thích nào.
        habits_count = 0;  // Tương tự, chưa có thói quen nào.
        friends_count = 0; // Và cũng chưa có bạn bè nào.
    }

    // --- CÁC HÀM TIỆN ÍCH (Member Functions) ---
    // Các hàm này thực hiện các thao tác trên dữ liệu của một đối tượng Student cụ thể.

    // Hàm thêm một chỉ số sở thích vào danh sách sở thích của sinh viên.
    // hobby_idx: Chỉ số của sở thích cần thêm (lấy từ HobbyTable).
    // Trả về: true nếu thêm thành công, false nếu danh sách đã đầy hoặc sở thích đã tồn tại.
    bool Student::addHobby(int hobby_idx) {
        // Kiểm tra xem mảng sở thích đã đầy chưa.
        if (hobbies_count >= MAX_HOBBIES_PER_STUDENT) {
            // cerr << "CANH BAO: Sinh vien " << id << " da co toi da " << MAX_HOBBIES_PER_STUDENT << " so thich. Khong the them!" << endl;
            return false; // Mảng đã đầy, không thêm được.
        }
        // Kiểm tra xem sở thích này đã có trong danh sách chưa (để tránh trùng lặp).
        for (int i = 0; i < hobbies_count; ++i) {
            if (hobbies_arr[i] == hobby_idx) {
                return true; // Sở thích đã tồn tại, không cần thêm lại, coi như thành công.
            }
        }
        // Nếu chưa đầy và chưa tồn tại, thêm sở thích mới vào cuối phần tử đã sử dụng của mảng.
        hobbies_arr[hobbies_count] = hobby_idx;
        hobbies_count++; // Tăng biến đếm số lượng sở thích lên.
        return true;
    }

    // Hàm thêm một chỉ số thói quen vào danh sách thói quen của sinh viên.
    // Logic tương tự như addHobby.
    bool Student::addHabit(int habit_idx) {
        if (habits_count >= MAX_HABITS_PER_STUDENT) {
            // cerr << "CANH BAO: Sinh vien " << id << " da co toi da " << MAX_HABITS_PER_STUDENT << " thoi quen. Khong the them!" << endl;
            return false;
        }
        for (int i = 0; i < habits_count; ++i) {
            if (habits_arr[i] == habit_idx) {
                return true;
            }
        }
        habits_arr[habits_count] = habit_idx;
        habits_count++;
        return true;
    }

    // Hàm thêm ID của một người bạn vào danh sách bạn bè của sinh viên.
    // friend_id: ID của sinh viên sẽ trở thành bạn.
    // Trả về: true nếu thêm thành công, false nếu danh sách bạn bè đã đầy,
    //         đã là bạn, hoặc cố gắng tự kết bạn với chính mình.
    bool Student::addFriend(const string& friend_id_to_add) {
        // Sinh viên không thể tự kết bạn với chính mình.
        if (friend_id_to_add == id) {
            // cerr << "CANH BAO: Sinh vien " << id << " khong the tu ket ban voi chinh minh." << endl;
            return false;
        }
        // Kiểm tra xem danh sách bạn bè đã đầy chưa.
        if (friends_count >= MAX_FRIENDS_PER_STUDENT) {
            // cerr << "CANH BAO: Sinh vien " << id << " da co toi da " << MAX_FRIENDS_PER_STUDENT << " ban be. Khong the them!" << endl;
            return false;
        }
        // Kiểm tra xem friend_id_to_add đã có trong danh sách bạn bè chưa.
        for (int i = 0; i < friends_count; ++i) {
            if (friends_arr[i] == friend_id_to_add) {
                return true; // Đã là bạn, không cần thêm lại.
            }
        }
        // Thêm bạn mới.
        friends_arr[friends_count] = friend_id_to_add;
        friends_count++;
        return true;
    }

    // Hàm xóa một ID bạn bè khỏi danh sách bạn bè của sinh viên.
    // friend_id_to_remove: ID của người bạn cần xóa.
    // Trả về: true nếu xóa thành công, false nếu không tìm thấy người bạn đó trong danh sách.
    bool Student::removeFriend(const string& friend_id_to_remove) {
        int found_idx = -1; // Biến lưu chỉ số của người bạn cần xóa, -1 nếu chưa tìm thấy.
        // Tìm kiếm người bạn cần xóa trong mảng friends_arr.
        for (int i = 0; i < friends_count; ++i) {
            if (friends_arr[i] == friend_id_to_remove) {
                found_idx = i; // Tìm thấy tại chỉ số i.
                break;         // Thoát vòng lặp vì đã tìm thấy.
            }
        }

        if (found_idx != -1) {
            // Nếu tìm thấy (found_idx khác -1), tiến hành xóa.
            // Xóa bằng cách dịch chuyển tất cả các phần tử phía sau vị trí found_idx
            // lên một bậc để lấp vào chỗ trống.
            for (int i = found_idx; i < friends_count - 1; ++i) {
                friends_arr[i] = friends_arr[i + 1];
            }
            friends_count--; // Giảm số lượng bạn bè đi một.
            return true;    // Xóa thành công.
        }
        return false; // Không tìm thấy người bạn để xóa.
    }

    // Hàm kiểm tra xem một sinh viên có ID là 'other_id' có phải là bạn của sinh viên này không.
    // 'const' ở cuối khai báo hàm này có nghĩa là hàm này không được phép thay đổi
    // bất kỳ thuộc tính nào của đối tượng Student hiện tại. Nó chỉ đọc dữ liệu.
    bool Student::isFriend(const string& other_id) const {
        for (int i = 0; i < friends_count; ++i) {
            if (friends_arr[i] == other_id) {
                return true; // Tìm thấy other_id trong danh sách bạn bè.
            }
        }
        return false; // Không tìm thấy.
    };