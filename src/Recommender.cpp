#include "Recommender.hpp" // Bao gồm khai báo của lớp Recommender và struct Candidate
#include <string>
#include <iostream>
#include <algorithm> // Sẽ dùng std::swap nếu cần trong Bubble Sort, hoặc tự viết swap

// Đưa tất cả các tên từ không gian tên 'std' vào phạm vi toàn cục của file này.
using namespace std;

// Constants for recommendation system
static const int MAX_POTENTIAL_CANDIDATES = 100; // Adjust this value based on your needs

// --- HÀM TIỆN ÍCH TĨNH (STATIC HELPER FUNCTIONS) ---
// Các hàm này chỉ được dùng bên trong file Recommender.cpp này.
// "static" có nghĩa là chúng không thuộc về một đối tượng cụ thể nào của lớp Recommender.

// Hàm tính chỉ số Jaccard giữa hai tập hợp (biểu diễn bằng mảng cố định).
// a_arr, b_arr: Con trỏ tới mảng các chỉ số (sở thích hoặc thói quen).
// a_count, b_count: Số lượng phần tử thực tế trong mỗi mảng.
// Trả về: Giá trị chỉ số Jaccard (từ 0.0 đến 1.0).
static double jaccard_fixed(const int* a_arr, int a_count, const int* b_arr, int b_count) {
    // Nếu cả hai tập hợp đều rỗng, độ tương đồng là 0 (hoặc 1 tùy định nghĩa, ở đây là 0).
    if (a_count == 0 && b_count == 0) {
        return 0.0;
    }

    size_t intersection_count = 0; // Đếm số phần tử chung.
    // Duyệt qua từng phần tử của tập A.
    for (int i = 0; i < a_count; ++i) {
        // Với mỗi phần tử của A, duyệt qua từng phần tử của tập B để tìm điểm chung.
        for (int j = 0; j < b_count; ++j) {
            if (a_arr[i] == b_arr[j]) {
                intersection_count++; // Tìm thấy phần tử chung.
                break; // Không cần tìm tiếp trong B cho a_arr[i] này nữa.
            }
        }
    }

    // Tính tổng số phần tử duy nhất trong cả hai tập (union_count).
    // |A U B| = |A| + |B| - |A ∩ B|
    size_t union_count = a_count + b_count - intersection_count;

    // Nếu union_count là 0 (chỉ xảy ra nếu cả a_count, b_count, intersection_count đều là 0,
    // đã được xử lý ở trên), thì trả về 0 để tránh chia cho 0.
    if (union_count == 0) {
        // Trường hợp này cũng có thể là cả hai tập giống hệt nhau và không rỗng,
        // khi đó intersection_count = union_count = a_count = b_count.
        // Nếu intersection_count > 0 và union_count = 0 (không thể xảy ra trừ khi code sai),
        // thì nên xem lại logic.
        // Để an toàn, nếu union là 0 thì jaccard là 0, trừ khi intersection cũng là 0 (đã xử lý).
        // Nếu intersection > 0 và union_count = 0 (nghĩa là a_count + b_count = intersection_count),
        // điều này có nghĩa là A và B giống hệt nhau, Jaccard nên là 1.
        // Tuy nhiên, với công thức |A U B| = |A| + |B| - |A ∩ B|, nếu |A U B|=0 thì |A|=0, |B|=0.
        return 0.0;
    }

    return static_cast<double>(intersection_count) / union_count;
}

// Hàm sắp xếp mảng các ứng viên (Candidate) theo điểm số (score) giảm dần.
// Sử dụng thuật toán Bubble Sort đơn giản.
// arr[]: Mảng các Candidate cần sắp xếp.
// n: Số lượng phần tử trong mảng.
static void bubbleSortCandidates(Candidate arr[], int n) {
    if (n <= 0) return; // Không có gì để sắp xếp

    for (int i = 0; i < n - 1; i++) {
        // Phần tử lớn nhất sẽ "nổi" lên cuối mảng sau mỗi lượt của vòng lặp ngoài.
        for (int j = 0; j < n - i - 1; j++) {
            // So sánh hai phần tử liền kề.
            // Nếu phần tử bên trái có điểm nhỏ hơn phần tử bên phải, đổi chỗ chúng.
            if (arr[j].score < arr[j + 1].score) {
                // Đổi chỗ (swap) arr[j] và arr[j+1]
                Candidate temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// --- TRIỂN KHAI HÀM GỢI Ý CHÍNH ---
// uid: ID của sinh viên cần gợi ý bạn bè.
// k: Số lượng gợi ý tối đa muốn hiển thị.
// weightMutual: Trọng số cho điểm bạn chung.
// weightJaccard: Trọng số cho điểm tương đồng Jaccard (sở thích + thói quen).
// Hàm này là 'const' vì nó không thay đổi trạng thái của đối tượng Recommender (chỉ đọc từ graph_).
void Recommender::suggestAndPrint(const string& uid, int k, double weightMutual, double weightJaccard) const {
    const Student* user_ptr = graph_.getStudent(uid);
    if (!user_ptr) {
        cout << "Không tìm thấy sinh viên với ID " << uid << endl;
        return;
    }

    const Student& user = *user_ptr;
    Candidate potential_candidates_arr[MAX_POTENTIAL_CANDIDATES] = {}; // Zero-initialize all elements
    int potential_candidates_count = 0;

    // Thêm mảng để theo dõi các ID đã xem
    string seen_users_arr[MAX_POTENTIAL_CANDIDATES];
    int seen_users_count = 0;

    // Duyệt qua tất cả sinh viên trong graph
    for (int i = 0; i < graph_.student_count; i++) {
        const Student& candidate = graph_.students_arr[i];
        
        // Bỏ qua chính user và bạn bè hiện tại
        if (candidate.id == uid || user.isFriend(candidate.id)) {
            continue;
        }

        // Tính điểm số
        double mutual_friends_score = 0;
        for (int j = 0; j < user.friends_count; j++) {
            if (candidate.isFriend(user.friends_arr[j])) {
                mutual_friends_score++;
            }
        }

        double jaccard_hobbies = jaccard_fixed(user.hobbies_arr, user.hobbies_count,
                                             candidate.hobbies_arr, candidate.hobbies_count);
        double jaccard_habits = jaccard_fixed(user.habits_arr, user.habits_count,
                                            candidate.habits_arr, candidate.habits_count);

        // Sửa lại cách tính điểm tổng hợp
        double total_jaccard_score = (jaccard_hobbies + jaccard_habits) / 2.0; // Lấy trung bình
        double final_score = (weightMutual * mutual_friends_score) + (weightJaccard * total_jaccard_score);

        // Thêm điều kiện kiểm tra điểm số
        if (final_score > 0) {
            if (potential_candidates_count < MAX_POTENTIAL_CANDIDATES) {
                potential_candidates_arr[potential_candidates_count].id = candidate.id;
                potential_candidates_arr[potential_candidates_count].score = final_score;
                seen_users_arr[seen_users_count++] = candidate.id;
                potential_candidates_count++;
            }
        }
    }

    // Sắp xếp và in kết quả
    bubbleSortCandidates(potential_candidates_arr, potential_candidates_count);
    
    // In kết quả
    cout << "\nGợi ý bạn bè cho " << user.name << " (ID: " << uid << "):\n";
    int num_to_print = (k > 0) ? min(k, potential_candidates_count) : potential_candidates_count;
    
    if (num_to_print == 0) {
        cout << "Không có gợi ý phù hợp.\n";
        return;
    }

    // Trước khi in kết quả, thêm debug
    cout << "Debug: Số lượng ứng viên tiềm năng: " << potential_candidates_count << endl;
    for (int i = 0; i < potential_candidates_count; ++i) {
        cout << "Debug: Ứng viên " << i << ": " << potential_candidates_arr[i].id 
             << " - Điểm: " << potential_candidates_arr[i].score << endl;
    }

    for (int i = 0; i < num_to_print; i++) {
        const Student* suggested = graph_.getStudent(potential_candidates_arr[i].id);
        if (suggested) {
            cout << (i + 1) << ". " << suggested->name 
                 << " (ID: " << suggested->id 
                 << ") - Điểm: " << potential_candidates_arr[i].score << endl;
        }
    }
    
    // --- PHẦN MỞ RỘNG: ĐỀ XUẤT BẠN BÈ CỦA BẠN BÈ (FOF) ---
    // Đề xuất bạn bè của bạn bè (Friend-of-Friend - FOF) nếu chưa đủ số lượng gợi ý.
    if (num_to_print < k) {
        // Duyệt qua từng ứng viên tiềm năng đã tìm được.
        for (int i = 0; i < potential_candidates_count; i++) {
            const Student* fof_candidate = graph_.getStudent(potential_candidates_arr[i].id);
            if (!fof_candidate) {
                continue; // Không tìm thấy thông tin sinh viên, bỏ qua.
            }

            // Duyệt qua danh sách bạn bè của ứng viên tiềm năng này.
            for (int j = 0; j < fof_candidate->friends_count; j++) {
                const string& fof_id = fof_candidate->friends_arr[j];
                const Student* fof_student = graph_.getStudent(fof_id);  // Lấy thông tin sinh viên FOF
                
                if (!fof_student || fof_id == uid || user.isFriend(fof_id)) {
                    continue;
                }

                // Kiểm tra đã xem chưa
                bool already_seen = false;
                for (int s_idx = 0; s_idx < seen_users_count; ++s_idx) {
                    if (seen_users_arr[s_idx] == fof_id) {
                        already_seen = true;
                        break;
                    }
                }
                if (already_seen) continue;

                // Tính điểm với thông tin sinh viên FOF đúng
                double fof_mutual_friends_score = 0;
                for (int m = 0; m < user.friends_count; m++) {
                    if (fof_student->isFriend(user.friends_arr[m])) {
                        fof_mutual_friends_score++;
                    }
                }

                double fof_jaccard_hobbies = jaccard_fixed(user.hobbies_arr, user.hobbies_count,
                                                          fof_student->hobbies_arr, fof_student->hobbies_count);
                double fof_jaccard_habits = jaccard_fixed(user.habits_arr, user.habits_count,
                                                           fof_student->habits_arr, fof_student->habits_count);

                double total_fof_jaccard_score = (fof_jaccard_hobbies + fof_jaccard_habits) / 2.0; // Lấy trung bình
                double fof_final_score = (weightMutual * fof_mutual_friends_score) + (weightJaccard * total_fof_jaccard_score);

                // Nếu điểm số cuối cùng của ứng viên FOF lớn hơn 0, thêm vào danh sách gợi ý.
                if (fof_final_score > 0) {
                    if (potential_candidates_count < MAX_POTENTIAL_CANDIDATES) {
                        potential_candidates_arr[potential_candidates_count].id = fof_id;
                        potential_candidates_arr[potential_candidates_count].score = fof_final_score;
                        seen_users_arr[seen_users_count++] = fof_id;
                        potential_candidates_count++;
                    }
                }

                // Debug thông tin của ứng viên FOF
                cout << "Debug: Đang xét FoF " << fof_id << endl;
                cout << "Debug: Số bạn chung: " << fof_mutual_friends_score << endl;
                cout << "Debug: Jaccard hobbies: " << fof_jaccard_hobbies << endl;
                cout << "Debug: Jaccard habits: " << fof_jaccard_habits << endl;
                cout << "Debug: Final score: " << fof_final_score << endl;
            }

            // Nếu đã đủ số lượng gợi ý, thoát khỏi vòng lặp.
            if (potential_candidates_count >= MAX_POTENTIAL_CANDIDATES) {
                break;
            }
        }

        // Sắp xếp lại danh sách gợi ý sau khi thêm các ứng viên FOF.
        bubbleSortCandidates(potential_candidates_arr, potential_candidates_count);

        // In kết quả gợi ý bổ sung (nếu có).
        cout << "\nGợi ý bổ sung cho " << user.name << " (ID: " << uid << "):\n";
        num_to_print = (k > 0) ? min(k, potential_candidates_count) : potential_candidates_count;
        
        // In kết quả
        for (int i = 0; i < num_to_print; i++) {
            const Student* suggested = graph_.getStudent(potential_candidates_arr[i].id);
            if (suggested) {
                cout << (i + 1) << ". " << suggested->name 
                     << " (ID: " << suggested->id 
                     << ") - Điểm: " << potential_candidates_arr[i].score << endl;
            }
        }
    }
}