// DataLoader.hpp
#pragma once
#include "Graph.hpp"
#include "DataTables.hpp"
#include <string>
#include <fstream>
#include <iostream>

// Constants for maximum array sizes
const int MAX_HOBBIES = 40;  // Maximum number of hobbies per student
const int MAX_HABITS = 30;   // Maximum number of habits per student 
const int MAX_FRIENDS = 50;  // Maximum number of friends per student
const int MAX_STUDENTS = 1000;  // Maximum number of students

using namespace std;

class DataLoader {
public:
    static bool loadHobbyTable(const string& filename, HobbyTable& table);
    static bool loadHabitTable(const string& filename, HabitTable& table);
    static bool loadStudents(Graph& graph, const string& filename);
    static bool loadFriendships(Graph& graph, const string& filename);
};