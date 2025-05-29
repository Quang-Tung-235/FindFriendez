// Graph.cpp
#include "Graph.hpp" // Bao gồm khai báo của lớp Graph

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
using namespace std;

// --- CONSTRUCTOR ---
// Hàm khởi tạo cho lớp Graph.
// Được gọi tự động khi một đối tượng Graph được tạo.
Graph::Graph() {
    student_count = 0; // Khởi tạo số lượng sinh viên trong đồ thị là 0 (đồ thị rỗng).
}

// --- HÀM TIỆN ÍCH NỘI BỘ ---
// Tìm chỉ số của sinh viên trong mảng students_arr dựa trên ID.
// id: ID của sinh viên cần tìm.
// Trả về: Chỉ số của sinh viên nếu tìm thấy, -1 nếu không.
// Hàm này là 'const' vì nó không thay đổi bất kỳ dữ liệu nào của đối tượng Graph.
int Graph::findStudentIndex(const string& id) const {
    for (int i = 0; i < student_count; ++i) { // Duyệt qua các sinh viên hiện có.
        if (students_arr[i].id == id) { // So sánh ID.
            return i; // Tìm thấy, trả về chỉ số.
        }
    }
    return -1; // Không tìm thấy.
}

// --- THAO TÁC VỚI SINH VIÊN (NÚT) ---
// Thêm một sinh viên vào đồ thị.
// s_param: Đối tượng Student cần thêm (được truyền bằng tham chiếu hằng để tránh sao chép không cần thiết nếu lớn).
// Trả về: true nếu thêm thành công, false nếu thất bại.
bool Graph::addStudent(const Student& s_param) {
    // Kiểm tra xem đồ thị đã đầy chưa.
    if (student_count >= MAX_STUDENTS_IN_GRAPH) {
        cerr << "LOI: Do thi da day (" << student_count << "/" << MAX_STUDENTS_IN_GRAPH 
             << "), khong the them sinh vien '" << s_param.id << "'!" << endl;
        return false;
    }
    // Kiểm tra xem ID của sinh viên sắp thêm đã tồn tại trong đồ thị chưa.
    if (findStudentIndex(s_param.id) != -1) {
        cerr << "LOI: ID sinh vien '" << s_param.id << "' da ton tai trong do thi!" << endl;
        return false;
    }

    // Nếu mọi thứ ổn, sao chép thông tin từ s_param vào vị trí tiếp theo trong mảng students_arr.
    // Phép gán '=' cho struct Student sẽ sao chép từng thành viên một cách mặc định.
    // Điều này hoạt động tốt vì Student của chúng ta chỉ chứa các kiểu cơ bản và std::string (quản lý bộ nhớ riêng).
    students_arr[student_count] = s_param;
    student_count++; // Tăng số lượng sinh viên trong đồ thị.
    return true;
}

// Xóa một sinh viên khỏi đồ thị dựa vào ID.
// id_to_remove: ID của sinh viên cần xóa.
// Trả về: true nếu xóa thành công, false nếu không tìm thấy sinh viên.
bool Graph::removeStudent(const string& id_to_remove) {
    int idx_to_remove = findStudentIndex(id_to_remove); // Tìm vị trí của sinh viên cần xóa.

    if (idx_to_remove == -1) {
        // cerr << "CANH BAO: Khong tim thay sinh vien co ID '" << id_to_remove << "' de xoa." << endl;
        return false; // Không tìm thấy sinh viên này.
    }

    // Bước 1: Xóa ID của sinh viên này khỏi danh sách bạn bè của TẤT CẢ các sinh viên khác.
    // Duyệt qua tất cả các sinh viên hiện có trong đồ thị.
    for (int i = 0; i < student_count; ++i) {
        if (i == idx_to_remove) {
            continue; // Bỏ qua chính sinh viên sắp bị xóa.
        }
        // Gọi hàm removeFriend của đối tượng Student tại students_arr[i]
        // để xóa id_to_remove khỏi danh sách bạn bè của họ.
        students_arr[i].removeFriend(id_to_remove);
    }

    // Bước 2: Xóa sinh viên khỏi mảng chính students_arr.
    // Thực hiện bằng cách dịch chuyển tất cả các sinh viên nằm phía sau vị trí idx_to_remove
    // lên một bậc để lấp vào chỗ trống.
    for (int i = idx_to_remove; i < student_count - 1; ++i) {
        students_arr[i] = students_arr[i + 1];
    }
    student_count--; // Giảm tổng số sinh viên trong đồ thị.
    return true; // Xóa thành công.
}

// --- THAO TÁC VỚI QUAN HỆ BẠN BÈ (CẠNH) ---
// Thêm quan hệ bạn bè giữa hai sinh viên.
// a_id, b_id: ID của hai sinh viên cần kết bạn.
// Trả về: true nếu thành công, false nếu thất bại.
bool Graph::addFriendship(const string& a_id, const string& b_id) {
    // Không cho phép tự kết bạn với chính mình.
    if (a_id == b_id) {
        cerr << "CANH BAO: Sinh vien khong the ket ban voi chinh minh (ID: " << a_id << ")." << endl;
        return false;
    }

    // Lấy con trỏ tới đối tượng Student của a_id và b_id.
    // getMutableStudent trả về con trỏ cho phép thay đổi đối tượng Student.
    Student* student_a = getMutableStudent(a_id);
    Student* student_b = getMutableStudent(b_id);

    // Kiểm tra xem cả hai sinh viên có tồn tại không.
    if (student_a == nullptr) { // 'nullptr' nghĩa là con trỏ không trỏ vào đâu cả (không tìm thấy).
        cerr << "LOI: Khong tim thay sinh vien A co ID '" << a_id << "' de them ban be." << endl;
        return false;
    }
    if (student_b == nullptr) {
        cerr << "LOI: Khong tim thay sinh vien B co ID '" << b_id << "' de them ban be." << endl;
        return false;
    }

    // Nếu cả hai sinh viên tồn tại, gọi hàm addFriend của mỗi người để thêm người kia vào danh sách bạn.
    // student_a->addFriend(b_id) dùng toán tử '->' vì student_a là một con trỏ.
    // Nó tương đương với (*student_a).addFriend(b_id).
    // Hàm addFriend của Student sẽ tự kiểm tra danh sách bạn bè đầy và bạn đã tồn tại chưa.
    bool success_a_adds_b = student_a->addFriend(b_id);
    bool success_b_adds_a = student_b->addFriend(a_id);

    // Chỉ coi là thành công nếu cả hai hành động thêm bạn đều thành công (hoặc đã là bạn từ trước).
    if (success_a_adds_b && success_b_adds_a) {
        return true;
    } else {
        // Nếu một trong hai thất bại (ví dụ do danh sách bạn của người đó đã đầy),
        // thì mối quan hệ bạn bè không được thiết lập hoàn chỉnh.
        // Có thể cần "rollback" nếu một chiều thành công còn chiều kia thất bại,
        // nhưng để đơn giản, ta chỉ báo lỗi chung.
        // Tuy nhiên, vì hàm Student::addFriend trả về true nếu đã là bạn,
        // nên trường hợp thất bại ở đây chủ yếu là do một trong hai danh sách bạn đã đầy
        // và người kia chưa có trong danh sách.
        if (!success_a_adds_b) cerr << "CANH BAO: Khong the them " << b_id << " vao danh sach ban cua " << a_id << " (co the da day)." << endl;
        if (!success_b_adds_a) cerr << "CANH BAO: Khong the them " << a_id << " vao danh sach ban cua " << b_id << " (co the da day)." << endl;
        return false;
    }
}

// Xóa quan hệ bạn bè giữa hai sinh viên.
// a_id, b_id: ID của hai sinh viên cần hủy kết bạn.
// Trả về: true nếu thành công (ít nhất một chiều quan hệ được xóa).
bool Graph::removeFriendship(const string& a_id, const string& b_id) {
    Student* student_a = getMutableStudent(a_id);
    Student* student_b = getMutableStudent(b_id);
    
    if (!student_a || !student_b) {
        return false;  // One or both students not found
    }

    bool removed = false;
    
    // Remove B from A's friend list
    for (int i = 0; i < student_a->friends_count; i++) {
        if (student_a->friends_arr[i] == b_id) {
            // Shift remaining friends left
            for (int j = i; j < student_a->friends_count - 1; j++) {
                student_a->friends_arr[j] = student_a->friends_arr[j + 1];
            }
            student_a->friends_count--;
            removed = true;
            break;
        }
    }

    // Remove A from B's friend list
    for (int i = 0; i < student_b->friends_count; i++) {
        if (student_b->friends_arr[i] == a_id) {
            // Shift remaining friends left
            for (int j = i; j < student_b->friends_count - 1; j++) {
                student_b->friends_arr[j] = student_b->friends_arr[j + 1];
            }
            student_b->friends_count--;
            removed = true;
            break;
        }
    }

    return removed;
}


// --- TRUY XUẤT THÔNG TIN SINH VIÊN ---
// Lấy con trỏ cho phép thay đổi Student.
Student* Graph::getMutableStudent(const string& id) {
    int idx = findStudentIndex(id);
    if (idx != -1) {
        return &students_arr[idx]; // Trả về địa chỉ của Student object trong mảng.
    }
    return nullptr; // Trả về con trỏ null nếu không tìm thấy.
}

// Lấy con trỏ hằng, chỉ cho phép đọc Student.
const Student* Graph::getStudent(const string& id) const {
    int idx = findStudentIndex(id);
    if (idx != -1) {
        return &students_arr[idx];
    }
    return nullptr;
}


// --- TIỆN ÍCH ---
// Tạo chuỗi DOT biểu diễn đồ thị.
string Graph::toDot() const {
    ostringstream oss; // Dùng ostringstream để xây dựng chuỗi hiệu quả.
    
    // Bắt đầu định nghĩa đồ thị G.
    oss << "graph G {\n";
    // Các thuộc tính chung cho đồ thị, nút và cạnh (có thể tùy chỉnh).
    oss << "  graph [charset=\"UTF-8\", layout=sfdp, overlap=prism, splines=true, K=0.5, sep=\"+10,10\"];\n"; // sfdp, fdp, neato
    oss << "  node [shape= Mrecord, style=\"filled,rounded\", fillcolor=\"#E6F2FF\", color=\"#A0C4FF\", fontname=\"Arial\"];\n";
    oss << "  edge [color=\"#BDB2FF\", penwidth=1.5];\n";

    // Bước 1: Khai báo tất cả các nút (sinh viên) trong đồ thị.
    // Điều này đảm bảo ngay cả những sinh viên chưa có bạn nào cũng được vẽ.
    for (int i = 0; i < student_count; ++i) {
        // ID được đặt trong dấu ngoặc kép để xử lý các trường hợp ID có thể chứa ký tự đặc biệt.
        // Label hiển thị tên và ID của sinh viên. Dùng { } cho Mrecord.
        oss << "  \"" << students_arr[i].id << "\" [label=\"{Name: " << students_arr[i].name << " | ID: " << students_arr[i].id << "}\"];\n";
    }

    // Bước 2: Thêm các cạnh (mối quan hệ bạn bè) vào đồ thị.
    // Để tránh vẽ cạnh lặp lại trong đồ thị vô hướng (ví dụ A--B và B--A là một),
    // chúng ta cần một cơ chế để chỉ vẽ mỗi cạnh một lần.
    // Sử dụng một mảng để lưu các "khóa cạnh" đã được vẽ.
    // Kích thước ước lượng: mỗi sinh viên có thể có MAX_FRIENDS, nhưng mỗi cạnh nối 2 SV.
    const int MAX_POSSIBLE_EDGES_IN_DOT = MAX_STUDENTS_IN_GRAPH * MAX_FRIENDS_PER_STUDENT; // Ước lượng dư dả
    string seen_edges_arr[MAX_POSSIBLE_EDGES_IN_DOT];
    int seen_edges_count = 0;

    for (int i = 0; i < student_count; ++i) {
        const Student& current_student = students_arr[i]; // Lấy sinh viên hiện tại.
        // Duyệt qua danh sách bạn bè của sinh viên hiện tại.
        for (int j = 0; j < current_student.friends_count; ++j) {
            const string& friend_id = current_student.friends_arr[j];

            // Tạo một "khóa" chuẩn hóa cho cạnh để kiểm tra trùng lặp.
            // Khóa này được tạo bằng cách ghép ID của hai sinh viên theo thứ tự từ điển.
            // Ví dụ: nếu bạn là "S1" và "S2", khóa sẽ là "S1_S2" (nếu S1 < S2).
            string key_part1 = current_student.id;
            string key_part2 = friend_id;

            // Đảm bảo key_part1 luôn nhỏ hơn hoặc bằng key_part2 về mặt từ điển.
            if (key_part1 > key_part2) {
                string temp = key_part1;
                key_part1 = key_part2;
                key_part2 = temp;
            }
            string edge_key = key_part1 + "_to_" + key_part2; // Khóa cạnh

            // Kiểm tra xem cạnh này đã được xử lý (đã được thêm vào seen_edges_arr) chưa.
            bool already_seen = false;
            for (int k = 0; k < seen_edges_count; ++k) {
                if (seen_edges_arr[k] == edge_key) {
                    already_seen = true;
                    break;
                }
            }

            if (!already_seen) {
                // Nếu cạnh này chưa được thấy, thêm nó vào chuỗi DOT.
                oss << "  \"" << current_student.id << "\" -- \"" << friend_id << "\";\n";
                // Và đánh dấu là đã thấy bằng cách thêm khóa cạnh vào mảng theo dõi.
                if (seen_edges_count < MAX_POSSIBLE_EDGES_IN_DOT) {
                    seen_edges_arr[seen_edges_count++] = edge_key;
                } else {
                    // Trường hợp mảng seen_edges_arr đầy (rất hiếm nếu kích thước đủ lớn).
                    // Có thể bỏ qua hoặc ghi log lỗi.
                }
            }
        }
    }

    oss << "}\n"; // Kết thúc định nghĩa đồ thị.
    return oss.str(); // Trả về chuỗi DOT hoàn chỉnh.
}