// main.cpp
#include <iostream>     // cin, cout, cerr
#include <iomanip>     // setw, setfill - để format output đẹp hơn
#include <string>      // xử lý string
#include <fstream>     // đọc/ghi file
#include <cstdlib>     // clear screen và một số hàm tiện ích
#include "DataTables.hpp"
#include "Graph.hpp"
#include "DataLoader.hpp"
#include "Recommender.hpp"
#include "CLI.hpp"

using namespace std;

// Hàm tiện ích để xóa màn hình console
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    clearScreen();  // Xóa màn hình trước khi bắt đầu
    cout << "Khoi tao he thong goi y ban be..." << endl;

    // 1. Khởi tạo các bảng dữ liệu
    HobbyTable global_hobby_table;
    HabitTable global_habit_table;
    cout << "Da khoi tao bang so thich va thoi quen." << endl;

    // 2. Nạp dữ liệu sở thích và thói quen
    if (!DataLoader::loadHobbyTable("data/hobbyc.txt", global_hobby_table) ||
        !DataLoader::loadHabitTable("data/habitc.txt", global_habit_table)) {
        cerr << "LOI: Khong the nap du lieu so thich va thoi quen." << endl;
        return 1;
    }
    
    // Hiển thị danh sách sở thích
    cout << "\n=== DANH SACH SO THICH ===" << endl;
    for (int i = 0; i < global_hobby_table.count; i++) {
        cout << (i + 1) << ". " << global_hobby_table.list_arr[i] << endl;
    }

    // Hiển thị danh sách thói quen
    cout << "\n=== DANH SACH THOI QUEN ===" << endl;
    for (int i = 0; i < global_habit_table.count; i++) {
        cout << (i + 1) << ". " << global_habit_table.list_arr[i] << endl;
    }
    
    cout << "\nDa nap du lieu so thich (" << global_hobby_table.count 
         << " muc) va thoi quen (" << global_habit_table.count << " muc)." << endl;
    cout << "---------------------------------------------" << endl;

    // 3. Khởi tạo đồ thị sinh viên
    Graph social_graph;
    cout << "Da khoi tao do thi sinh vien." << endl;

    // 4. Nạp dữ liệu sinh viên và quan hệ bạn bè - sử dụng tên file chính xác
    if (!DataLoader::loadStudents(social_graph, "data/student_info.txt")) {
        cerr << "LOI: Khong the nap du lieu sinh vien." << endl;
        return 1;
    }
    cout << "Da nap du lieu sinh vien (" << social_graph.student_count << " sinh vien)." << endl;

    // Bỏ qua việc nạp friendships.txt vì mối quan hệ bạn bè đã được định nghĩa trong student_info.txt
    /*
    if (!DataLoader::loadFriendships(social_graph, "data/friendships.txt")) {
        cerr << "LOI: Khong the nap du lieu moi quan he ban be." << endl;
        return 1;
    }
    */
    cout << "Da nap du lieu moi quan he ban be." << endl;
    

    // 5. Khởi tạo bộ gợi ý, truyền đồ thị vào cho nó.
    Recommender friend_recommender(social_graph);
    cout << "Da khoi tao bo goi y." << endl;

    // 6. Khởi tạo giao diện dòng lệnh, truyền các thành phần cần thiết vào.
    CLI user_interface(social_graph, global_hobby_table, global_habit_table, friend_recommender);
    cout << "Da khoi tao giao dien dong lenh." << endl;
    cout << "---------------------------------------------" << endl;


    // 7. Chạy giao diện dòng lệnh.
    //    Chương trình sẽ tiếp tục chạy trong hàm run() cho đến khi người dùng chọn thoát.
    user_interface.run();


    // Khi hàm run() của CLI kết thúc (do người dùng thoát), chương trình sẽ kết thúc ở đây.
    return 0; // Trả về 0 cho hệ điều hành, báo hiệu chương trình kết thúc thành công.
}