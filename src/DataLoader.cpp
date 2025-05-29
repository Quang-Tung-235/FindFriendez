// DataLoader.cpp
#include "DataLoader.hpp"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Static helper functions (file scope)
static void splitString(const string& input_str, char delimiter,
                       string output_tokens_arr[], int& output_tokens_count, int max_tokens) {
    output_tokens_count = 0; // Reset số lượng token
    if (input_str.empty()) {
        return; // Chuỗi rỗng thì không có gì để tách
    }

    string current_token;
    for (int i = 0; i < input_str.length(); ++i) {
        char ch = input_str[i];
        if (ch == delimiter) {
            if (output_tokens_count < max_tokens) {
                // Loại bỏ khoảng trắng thừa ở đầu và cuối current_token (nếu muốn)
                // (Phần này có thể phức tạp hơn, tạm thời bỏ qua để giữ đơn giản)
                // Ví dụ đơn giản: chỉ thêm nếu token không rỗng
                if (!current_token.empty()) {
                     output_tokens_arr[output_tokens_count++] = current_token;
                }
            } else {
                cerr << "CANH BAO: Mang output_tokens_arr da day khi tach chuoi. Mot so token co the bi bo qua." << endl;
                return; // Dừng nếu mảng output đầy
            }
            current_token.clear(); // Reset để bắt đầu token mới
        } else {
            current_token += ch; // Thêm ký tự vào token hiện tại
        }
    }

    // Thêm token cuối cùng (nếu có và mảng chưa đầy)
    if (!current_token.empty() && output_tokens_count < max_tokens) {
        output_tokens_arr[output_tokens_count++] = current_token;
    } else if (!current_token.empty() && output_tokens_count >= max_tokens) {
         cerr << "CANH BAO: Mang output_tokens_arr da day. Token cuoi cung '" << current_token << "' bi bo qua." << endl;
    }
}

// Hàm chuyển một chuỗi số thành số nguyên.
// Trả về số nguyên nếu thành công, hoặc một giá trị lỗi (ví dụ -1) nếu thất bại.
// Đây là một phiên bản rất đơn giản, không xử lý lỗi tốt như std::stoi.
static int simple_stoi(const string& s) {
    if (s.empty()) return -1;
    int res = 0;
    int sign = 1;
    int i = 0;
    if (s[0] == '-') {
        sign = -1;
        i = 1;
    } else if (s[0] == '+') {
        i = 1;
    }
    for (; i < s.length(); ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            res = res * 10 + (s[i] - '0');
        } else {
            return -1; // Ký tự không phải số, báo lỗi (hoặc ném exception)
        }
    }
    return res * sign;
}


// Class method implementations
bool DataLoader::loadHobbyTable(const string& filename, HobbyTable& table) {
    ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        cerr << "LOI: Khong the mo file hobby: " << filename << endl;
        return false;
    }

    string line_content;
    while (getline(file_stream, line_content)) {
        if (!line_content.empty()) {
            table.add(line_content);
        }
    }
    file_stream.close();
    return true;
}

bool DataLoader::loadHabitTable(const string& filename, HabitTable& table) {
    ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        cerr << "LOI: Khong the mo file habit: " << filename << endl;
        return false;
    }
    
    string line_content;
    while (getline(file_stream, line_content)) {
        if (!line_content.empty()) {
            table.add(line_content);
        }
    }
    file_stream.close();
    return true;
}

bool DataLoader::loadStudents(Graph& graph, const string& filename) {
    ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        cerr << "LOI: Khong the mo file student_info: " << filename << endl;
        return false;
    }

    // GIAI ĐOẠN 1: Đọc và lưu thông tin sinh viên vào mảng động
    Student* students = new Student[MAX_STUDENTS];
    string* friend_lists = new string[MAX_STUDENTS];  // Lưu danh sách bạn bè dạng chuỗi
    int student_count = 0;

    string line1_id, line2_name, line3_friends_str, line4_hobbies_idx_str, line5_habits_idx_str;

    while (getline(file_stream, line1_id) && student_count < MAX_STUDENTS) {
        // Đọc 4 dòng còn lại cho mỗi sinh viên
        if (!getline(file_stream, line2_name) ||
            !getline(file_stream, line3_friends_str) ||
            !getline(file_stream, line4_hobbies_idx_str) ||
            !getline(file_stream, line5_habits_idx_str)) {
            break;
        }

        // Khởi tạo thông tin sinh viên
        students[student_count].id = line1_id;
        students[student_count].name = line2_name;
        students[student_count].hobbies_count = 0;
        students[student_count].habits_count = 0;

        // Xử lý sở thích
        string hobby_tokens[MAX_HOBBIES];
        int hobby_count;
        splitString(line4_hobbies_idx_str, ' ', hobby_tokens, hobby_count, MAX_HOBBIES);
        for (int i = 0; i < hobby_count; i++) {
            int hobby_idx = simple_stoi(hobby_tokens[i]);
            if (hobby_idx > 0) {
                students[student_count].hobbies_arr[students[student_count].hobbies_count++] = hobby_idx;
            }
        }

        // Xử lý thói quen
        string habit_tokens[MAX_HABITS];
        int habit_count;
        splitString(line5_habits_idx_str, ' ', habit_tokens, habit_count, MAX_HABITS);
        for (int i = 0; i < habit_count; i++) {
            int habit_idx = simple_stoi(habit_tokens[i]);
            if (habit_idx > 0) {
                students[student_count].habits_arr[students[student_count].habits_count++] = habit_idx;
            }
        }

        // Lưu lại danh sách bạn bè để xử lý sau
        friend_lists[student_count] = line3_friends_str;
        
        student_count++;
    }

    // GIAI ĐOẠN 2: Thêm sinh viên vào đồ thị
    for (int i = 0; i < student_count; i++) {
        if (!graph.addStudent(students[i])) {
            cerr << "LOI: Khong the them sinh vien " << students[i].id << endl;
            delete[] students;
            delete[] friend_lists;
            return false;
        }
    }

    // GIAI ĐOẠN 3: Thiết lập quan hệ bạn bè
    for (int i = 0; i < student_count; i++) {
        string friend_tokens[MAX_FRIENDS];
        int friend_count;
        splitString(friend_lists[i], ' ', friend_tokens, friend_count, MAX_FRIENDS);
        
        for (int j = 0; j < friend_count; j++) {
            if (!friend_tokens[j].empty()) {
                // Kiểm tra xem sinh viên có tồn tại không trước khi thêm quan hệ bạn bè
                if (graph.getStudent(students[i].id) && graph.getStudent(friend_tokens[j])) {
                    graph.addFriendship(students[i].id, friend_tokens[j]);
                }
            }
        }
    }

    // Giải phóng bộ nhớ động
    delete[] students;
    delete[] friend_lists;

    file_stream.close();
    return true;
}

bool DataLoader::loadFriendships(Graph& graph, const string& filename) {
    ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        cerr << "LOI: Khong the mo file friendships: " << filename << endl;
        return false;
    }

    string line;
    int line_number = 0;
    bool success = true;

    while (getline(file_stream, line)) {
        line_number++;
        if (line.empty() || line[0] == '#') continue; // Bỏ qua dòng trống và comment
        
        string tokens_arr[2];
        int tokens_count;
        splitString(line, ',', tokens_arr, tokens_count, 2);
        
        if (tokens_count != 2) {
            cerr << "LOI: Dinh dang khong hop le tai dong " << line_number << endl;
            continue;
        }

        // Trim whitespace from IDs
        for (int i = 0; i < 2; i++) {
            tokens_arr[i].erase(0, tokens_arr[i].find_first_not_of(" \t\r\n"));
            tokens_arr[i].erase(tokens_arr[i].find_last_not_of(" \t\r\n") + 1);
        }

        if (!graph.addFriendship(tokens_arr[0], tokens_arr[1])) {
            cerr << "LOI: Khong the them moi quan he ban be tai dong " << line_number 
                 << " (" << tokens_arr[0] << " - " << tokens_arr[1] << ")" << endl;
            success = false;
        }
    }

    file_stream.close();
    return success;
}