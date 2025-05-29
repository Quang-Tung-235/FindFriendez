// CLI.cpp
#include "CLI.hpp"      // Bao gồm khai báo lớp CLI
#include <fstream>      // Để dùng ofstream khi xuất file DOT
#include <limits>       // Để dùng numeric_limits cho việc xóa bộ đệm cin

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
using namespace std;

// --- HÀM TIỆN ÍCH NỘI BỘ CHO CLI (Tương tự như trong DataLoader) ---
// Hàm tách một chuỗi `input_str` thành các phần nhỏ dựa trên một ký tự phân tách `delimiter`.
// Kết quả được lưu vào mảng `output_tokens_arr`.
// `output_tokens_count` sẽ lưu số lượng token thực tế được tách ra.
// `max_tokens`: Kích thước tối đa của mảng `output_tokens_arr`.
// Hàm này được điều chỉnh một chút để loại bỏ khoảng trắng ở đầu/cuối mỗi token.
static void cliSplitString(const string& input_str, char delimiter,
                           string output_tokens_arr[], int& output_tokens_count, int max_tokens) {
    output_tokens_count = 0;
    if (input_str.empty()) {
        return;
    }

    string current_token;
    for (int i = 0; i < input_str.length(); ++i) {
        char ch = input_str[i];
        if (ch == delimiter) {
            // Xử lý token hiện tại trước khi reset
            // Loại bỏ khoảng trắng ở đầu/cuối token (đơn giản)
            size_t start = current_token.find_first_not_of(" \t\n\r");
            size_t end = current_token.find_last_not_of(" \t\n\r");
            if (string::npos != start && string::npos != end) {
                current_token = current_token.substr(start, end - start + 1);
            } else if (string::npos != start) { // Chỉ có khoảng trắng ở cuối
                current_token = current_token.substr(start);
            } else { // Toàn khoảng trắng hoặc rỗng
                 current_token.clear();
            }

            if (!current_token.empty() && output_tokens_count < max_tokens) {
                output_tokens_arr[output_tokens_count++] = current_token;
            } else if (!current_token.empty()) {
                // cerr << "CANH BAO (CLI_Split): Mang output day, token '" << current_token << "' bi bo qua." << endl;
            }
            current_token.clear();
        } else {
            current_token += ch;
        }
    }
    // Xử lý token cuối cùng
    size_t start = current_token.find_first_not_of(" \t\n\r");
    size_t end = current_token.find_last_not_of(" \t\n\r");
     if (string::npos != start && string::npos != end) {
        current_token = current_token.substr(start, end - start + 1);
    } else if (string::npos != start) {
        current_token = current_token.substr(start);
    } else {
         current_token.clear();
    }

    if (!current_token.empty() && output_tokens_count < max_tokens) {
        output_tokens_arr[output_tokens_count++] = current_token;
    } else if (!current_token.empty()) {
        // cerr << "CANH BAO (CLI_Split): Mang output day, token cuoi '" << current_token << "' bi bo qua." << endl;
    }
}


// --- CONSTRUCTOR ---
// Khởi tạo đối tượng CLI với các tham chiếu đến thành phần dữ liệu và logic.
CLI::CLI(Graph& g, HobbyTable& hb_tbl, HabitTable& ha_tbl, Recommender& recomm)
    : graph_(g), hobby_table_(hb_tbl), habit_table_(ha_tbl), recommender_(recomm) {
    // Thân hàm khởi tạo có thể để trống nếu tất cả việc khởi tạo đã được thực hiện
    // trong danh sách khởi tạo thành viên (initializer list) ở trên.
}

// --- HÀM CHẠY CHÍNH ---
// Bắt đầu vòng lặp giao diện người dùng.
void CLI::run() {
    // Vòng lặp vô hạn, chỉ thoát khi người dùng chọn tùy chọn thoát (case 0 trong menu).
    while (true) {
        menu(); // Hiển thị menu và xử lý lựa chọn.
    }
}

// --- HIỂN THỊ MENU VÀ ĐIỀU PHỐI ---
void CLI::menu() {
    cout << "\n=========== HE THONG GOI Y BAN BE ===========" << endl;
    cout << "1. Them sinh vien moi" << endl;
    cout << "2. Ket ban giua hai sinh vien" << endl;
    cout << "3. Goi y ban be cho mot sinh vien" << endl;
    cout << "4. Liet ke danh sach sinh vien" << endl;
    cout << "5. Xoa quan he ban be" << endl;
    cout << "0. Thoat chuong trinh" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "Lua chon cua ban: ";

    int choice = -1;
    cin >> choice;

    // Xử lý trường hợp nhập không phải là số hoặc lỗi luồng đầu vào.
    if (cin.fail()) {
        cin.clear(); // Xóa cờ lỗi của cin.
        // Bỏ qua các ký tự không hợp lệ còn lại trong bộ đệm đầu vào cho đến khi gặp ký tự xuống dòng.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        choice = -1; // Đặt choice về giá trị không hợp lệ để đi vào default case.
    } else {
        // Nếu đọc số thành công, vẫn cần bỏ qua phần còn lại của dòng (ví dụ, ký tự xuống dòng).
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }


    switch (choice) {
        case 1:
            addStudentFlow();
            break;
        case 2:
            addFriendshipFlow();
            break;
        case 3:
            suggestFlow();
            break;
        case 4:
            listStudentsFlow();
            break;
        case 5:
            removeFriendshipFlow();
            break;
        case 0:
            cout << "Tam biet! Cam on ban da su dung he thong." << endl;
            exit(0); // Kết thúc chương trình.
        default:
            cout << "LOI: Lua chon khong hop le. Vui long chon lai." << endl;
            break;
    }
}

// --- CÁC LUỒNG XỬ LÝ CHỨC NĂNG ---

void CLI::addStudentFlow() {
    cout << "\n--- Them Sinh Vien Moi ---" << endl;
    Student new_student; // Tạo một đối tượng Student mới.

    cout << "Nhap ID sinh vien: ";
    // Đọc ID, giả sử ID không chứa khoảng trắng.
    // Nếu ID có thể chứa khoảng trắng, cần dùng getline.
    cin >> new_student.id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm sau khi đọc bằng cin >>

    // Kiểm tra xem ID đã tồn tại chưa trước khi nhập các thông tin khác.
    if (graph_.getStudent(new_student.id) != nullptr) {
        cout << "LOI: ID sinh vien '" << new_student.id << "' da ton tai trong he thong." << endl;
        return;
    }

    cout << "Nhap ten sinh vien: ";
    // Đọc tên. getline cho phép đọc cả dòng, bao gồm khoảng trắng.
    getline(cin, new_student.name);

    // Nhập sở thích (các tên sở thích, cách nhau bằng dấu phẩy)
    cout << "Nhap cac so thich (cach nhau bang dau phay ',' hoac khoang trang): ";
    string hobbies_input_str;
    getline(cin, hobbies_input_str);

    string hobby_tokens[MAX_HOBBIES_PER_STUDENT * 2]; // Mảng tạm lớn hơn một chút
    int hobby_token_count = 0;
    cliSplitString(hobbies_input_str, ',', hobby_tokens, hobby_token_count, MAX_HOBBIES_PER_STUDENT * 2);

    for (int i = 0; i < hobby_token_count; ++i) {
        if (hobby_tokens[i].empty()) continue;
        // Với mỗi tên sở thích, thêm vào HobbyTable để lấy/tạo chỉ số.
        int hobby_idx = hobby_table_.add(hobby_tokens[i]);
        if (hobby_idx != -1) { // Nếu thêm vào bảng thành công (bảng không đầy)
            // Thêm chỉ số sở thích này vào cho sinh viên.
            // Hàm addHobby của Student sẽ kiểm tra mảng sở thích của sinh viên có đầy không.
            if (!new_student.addHobby(hobby_idx)) {
                 cout << "CANH BAO: Khong the them so thich '" << hobby_tokens[i] << "' cho SV (danh sach so thich cua SV da day)." << endl;
            }
        } else {
            cout << "CANH BAO: Khong the them so thich '" << hobby_tokens[i] << "' vao he thong (bang so thich he thong da day)." << endl;
        }
    }


    // Nhập thói quen (tương tự như sở thích)
    cout << "Nhap cac thoi quen (cach nhau bang dau phay ',' hoac khoang trang): ";
    string habits_input_str;
    getline(cin, habits_input_str);

    string habit_tokens[MAX_HABITS_PER_STUDENT * 2];
    int habit_token_count = 0;
    cliSplitString(habits_input_str, ',', habit_tokens, habit_token_count, MAX_HABITS_PER_STUDENT * 2);

    for (int i = 0; i < habit_token_count; ++i) {
        if (habit_tokens[i].empty()) continue;
        int habit_idx = habit_table_.add(habit_tokens[i]);
        if (habit_idx != -1) {
            if(!new_student.addHabit(habit_idx)) {
                cout << "CANH BAO: Khong the them thoi quen '" << habit_tokens[i] << "' cho SV (danh sach thoi quen cua SV da day)." << endl;
            }
        } else {
             cout << "CANH BAO: Khong the them thoi quen '" << habit_tokens[i] << "' vao he thong (bang thoi quen he thong da day)." << endl;
        }
    }

    // Sau khi có đủ thông tin, thêm sinh viên vào đồ thị.
    if (graph_.addStudent(new_student)) {
        cout << "Them sinh vien '" << new_student.name << "' (ID: " << new_student.id << ") thanh cong!" << endl;
    } else {
        // graph_.addStudent đã in lỗi nếu có (ID trùng hoặc đồ thị đầy)
        // cout << "LOI: Khong the them sinh vien. ID co the da ton tai hoac do thi da day." << endl;
    }
}

void CLI::addFriendshipFlow() {
    cout << "\n--- Ket Ban Giua Hai Sinh Vien ---" << endl;
    string id_a, id_b;

    cout << "Nhap ID cua sinh vien thu nhat: ";
    cin >> id_a;
    cout << "Nhap ID cua sinh vien thu hai: ";
    cin >> id_b;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm

    if (id_a == id_b) {
        cout << "LOI: Hai ID giong nhau. Sinh vien khong the ket ban voi chinh minh." << endl;
        return;
    }

    if (graph_.addFriendship(id_a, id_b)) {
        cout << "Tao quan he ban be giua " << id_a << " va " << id_b << " thanh cong!" << endl;
    } else {
        cout << "LOI: Khong the tao quan he ban be. Vui long kiem tra lai ID hoac danh sach ban cua ho." << endl;
    }
}

void CLI::suggestFlow() {
    cout << "\n--- Goi Y Ban Be ---" << endl;
    string user_id;
    cout << "Nhap ID cua sinh vien can goi y ban be: ";
    cin >> user_id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Các trọng số này có thể được nhập từ người dùng hoặc đặt cố định.
    double weight_mutual = 1.0;
    double weight_jaccard = 0.5; // Ví dụ: ưu tiên bạn chung hơn một chút
    int k_suggestions = 5;     // Số lượng gợi ý muốn hiển thị

    // Gọi hàm suggestAndPrint của Recommender.
    // Hàm này sẽ tự in kết quả ra màn hình.
    recommender_.suggestAndPrint(user_id, k_suggestions, weight_mutual, weight_jaccard);
}

void CLI::exportDotFlow() {
    cout << "\n--- Xuat Do Thi Ra File DOT ---" << endl;
    string filename = "graph.dot";
    ofstream dot_file(filename.c_str()); // Mở file để ghi

    if (!dot_file.is_open()) {
        cerr << "LOI: Khong the mo file '" << filename << "' de ghi." << endl;
        return;
    }

    string dot_string = graph_.toDot(); // Lấy chuỗi DOT từ đối tượng Graph.
    dot_file << dot_string;            // Ghi chuỗi vào file.
    dot_file.close();                  // Đóng file.

    cout << "Da xuat do thi ra file '" << filename << "'." << endl;
    cout << "Ban co the dung Graphviz de chuyen doi file DOT sang hinh anh, vi du:" << endl;
    cout << "dot -Tpng " << filename << " -o graph.png" << endl;
    cout << "Hoac: dot -Tsvg " << filename << " -o graph.svg" << endl;
}

void CLI::listStudentsFlow() {
    cout << "\n--- Danh Sach Sinh Vien Trong He Thong ---" << endl;
    if (graph_.student_count == 0) {
        cout << "Chua co sinh vien nao trong he thong." << endl;
        return;
    }

    cout << "------------------------------------------------------------------------------------------" << endl;
    cout << "ID         | Ten                 | So thich                               | Thoi quen                              | Ban be" << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;

    // Duyệt qua mảng sinh viên trong đồ thị.
    for (int i = 0; i < graph_.student_count; ++i) {
        const Student& s = graph_.students_arr[i]; // Lấy tham chiếu hằng tới sinh viên hiện tại.

        // In ID và Tên (căn chỉnh cơ bản)
        cout.width(10); cout << left << s.id << " | ";
        cout.width(20); cout << left << s.name << " | ";

        // In Sở thích
        string hobbies_str;
        for (int j = 0; j < s.hobbies_count; ++j) {
            int hobby_idx = s.hobbies_arr[j];
            // Lấy tên sở thích từ HobbyTable dựa vào chỉ số.
            if (hobby_idx >= 0 && hobby_idx < hobby_table_.count) {
                hobbies_str += hobby_table_.list_arr[hobby_idx];
            } else {
                hobbies_str += "?idx" + to_string(hobby_idx) + "?"; // Chỉ số không hợp lệ
            }
            if (j < s.hobbies_count - 1) {
                hobbies_str += ", ";
            }
        }
        cout.width(30); cout << left << (hobbies_str.empty() ? "(khong co)" : hobbies_str) << " | ";

        // In Thói quen
        string habits_str;
        for (int j = 0; j < s.habits_count; ++j) {
            int habit_idx = s.habits_arr[j];
            if (habit_idx >= 0 && habit_idx < habit_table_.count) {
                habits_str += habit_table_.list_arr[habit_idx];
            } else {
                 habits_str += "?idx" + to_string(habit_idx) + "?";
            }
            if (j < s.habits_count - 1) {
                habits_str += ", ";
            }
        }
        cout.width(30); cout << left << (habits_str.empty() ? "(khong co)" : habits_str) << " | ";

        // In Bạn bè (chỉ ID)
        string friends_str;
        for (int j = 0; j < s.friends_count; ++j) {
            friends_str += s.friends_arr[j];
            // Nếu muốn hiển thị tên bạn bè:
            // const Student* friend_obj = graph_.getStudent(s.friends_arr[j]);
            // if(friend_obj) friends_str += friend_obj->name; else friends_str += s.friends_arr[j];

            if (j < s.friends_count - 1) {
                friends_str += ", ";
            }
        }
        cout << left << (friends_str.empty() ? "(chua co ban)" : friends_str) << endl;
    }
    cout << "------------------------------------------------------------------------------------------" << endl;
}

void CLI::removeFriendshipFlow() {
    cout << "\n--- Xoa Quan He Ban Be ---" << endl;
    
    string id1, id2;
    cout << "Nhap ID sinh vien thu nhat: ";
    cin >> id1;
    
    cout << "Nhap ID sinh vien thu hai: ";
    cin >> id2;
    
    // Show student info for confirmation
    const Student* student1 = graph_.getStudent(id1);
    const Student* student2 = graph_.getStudent(id2);
    
    if (!student1 || !student2) {
        cout << "LOI: Khong tim thay mot hoac ca hai sinh vien!" << endl;
        return;
    }

    cout << "\nBan muon xoa quan he ban be giua:" << endl;
    cout << "1. " << student1->name << " (ID: " << id1 << ")" << endl;
    cout << "2. " << student2->name << " (ID: " << id2 << ")" << endl;
    
    cout << "\nBan co chac chan? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (tolower(confirm) == 'y') {
        if (graph_.removeFriendship(id1, id2)) {
            cout << "Da xoa quan he ban be thanh cong!" << endl;
        } else {
            cout << "LOI: Hai sinh vien nay khong phai la ban be!" << endl;
        }
    } else {
        cout << "Da huy thao tac xoa quan he ban be." << endl;
    }
}
