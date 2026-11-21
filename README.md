# Smart Course Planner

Smart Course Planner is a beginner-friendly C++ console application for managing a university course catalogue and exploring prerequisite relationships. It keeps one shared completion record, which the instructor updates and students can view.

## Features

- Separate Instructor and Student menus, with no login system.
- Add, update, delete, and search courses by ID, course code, or course name.
- Validate positive, unique course IDs; unique course codes; non-empty names and codes; and credit hours from 1 through 6.
- Add and remove prerequisite relationships. An edge `101 -> 102` means course 101 is a prerequisite of course 102. Missing courses, duplicate links, self-links, and cycle-forming links are rejected.
- Display prerequisites and dependent courses, and traverse the graph with BFS and DFS.
- Let instructors officially mark courses complete only after all prerequisites are complete. Students can view completed, eligible, and locked courses but have no completion control.
- Generate a prerequisite-ordered remaining academic path. Completed courses are omitted, and a cycle prevents path generation.
- Sort by course ID using Merge Sort, Bubble Sort, Insertion Sort, Selection Sort, Comb Sort, or Shell Sort.
- Search course names with case-insensitive Brute Force or KMP string search.
- Search course codes case-insensitively using the custom hash table.

The application starts with an empty catalogue. Add courses as an instructor before exploring plans as a student. Course data is held in memory and is cleared when the program exits.

## Roles

### Instructor

The instructor manages course records and prerequisite relationships, runs graph and search tools, selects sorting algorithms, and records official course completion. A course cannot be completed until all its direct prerequisites have been completed.

### Student

Students can browse and search courses, view completed/eligible/locked courses, inspect prerequisites and dependents, run BFS/DFS, generate the remaining academic path, and search course names. The student menu does not provide a way to change completion status.

## Technologies and DSA

- C++11, console input/output, VS Code, PowerShell, and g++
- Custom `DynamicArray<T>` with dynamic allocation, resizing, insertion/removal, deep copying, and a destructor
- Linked-list adjacency representation for the directed course graph
- Custom linked-list queue for BFS and stack for iterative DFS
- Kahn topological sorting and DFS-based cycle detection
- Open-addressed hash table with linear probing, tombstones, collision handling, deletion, and rehashing
- Six custom sorting algorithms: Merge, Bubble, Insertion, Selection, Comb, and Shell Sort
- Brute Force and KMP course-name searching
- Rule of Three for owning dynamic arrays and hash table; graph, stack, and queue copying is disabled to prevent shallow copies

## Team Responsibilities

### Member 1 — Graph Module

Graph representation and adjacency lists; BFS and DFS; topological sorting; cycle detection; and prerequisite management.

### Member 2 — Course Database Module

Course records; dynamic arrays; database operations; hash tables and rehashing; sorting; and string searching.

Both members collaborate on integration, student planning, testing, and documentation.

## Project Structure

```text
include/                 Header files and custom data structures
  Course.h
  CourseDatabase.h
  CourseGraph.h
  DynamicArray.h
  GraphQueue.h
  GraphStack.h
  HashTable.h
  SampleData.h           Optional sample-data declarations
  Sorting.h
  StringSearch.h
  StudentPlanner.h
src/                     C++ implementations
  Course.cpp
  CourseDatabase.cpp
  CourseGraph.cpp
  HashTable.cpp
  StudentPlanner.cpp
main.cpp                 Role menus and console interaction
README.md                Project guide
```

## Compile and Run on Windows

Open the project folder in VS Code, then open a PowerShell terminal in that folder. Compile with the project command:

```powershell
g++ -std=c++11 -Wall -Wextra -pedantic -Iinclude main.cpp src/Course.cpp src/HashTable.cpp src/CourseDatabase.cpp src/CourseGraph.cpp src/StudentPlanner.cpp -o SmartCoursePlanner.exe
```

Run the program with:

```powershell
.\SmartCoursePlanner.exe
```

The commands above were verified with g++ in this project folder. If Windows reports that the executable is in use, close its running console window and run the compile command again.

## Example Usage

1. Choose **Instructor** from the start menu.
2. Add course `101` (`CS101`, `Introduction to Programming`, 3 credits) and course `102` (`CS102`, `Data Structures`, 4 credits).
3. Add prerequisite with course ID `102` and prerequisite ID `101`. The graph relationship is `101 -> 102`.
4. Try completing `102`; the application will explain that `101` must be completed first.
5. Mark `101` complete, then mark `102` complete.
6. Return to the start menu and choose **Student** to view completed courses or generate the remaining academic path.

## Testing and Verification

The final review compiled the complete application with `-std=c++11 -Wall -Wextra -pedantic` without warnings. Runtime checks covered the instructor and student menus; invalid menu input; duplicate and invalid course data; ID/code/name searches; course updates and deletion; prerequisite add/remove, duplicate, self-link, and cycle rejection; prerequisite completion gates; completed, eligible, and locked course logic; BFS, DFS, dependent display, and topological ordering; remaining-path output; all six sorts on empty, single-item, and multi-item arrays; case-insensitive Brute Force/KMP and code lookup; hash-table rehash, collision probing, deletion and copying; and empty-database behavior. Temporary review test files and executables were removed after verification.
