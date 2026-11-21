# Smart Course Planner — Code Walkthrough

This guide follows the current source code. It describes the algorithms and menu behavior that are implemented today; it does not assume a feature merely from a class or function name.

## 1. Project overview

Smart Course Planner is a C++11 console program with two roles. An instructor maintains course records and prerequisite relationships and marks courses complete. A student can inspect progress, discover eligible and locked courses, traverse prerequisite relationships, search course names, and view a valid remaining course order. The database and completion flags exist only in memory while the application is running.

The graph uses a consistent direction: **prerequisite → dependent course**. Thus `101 -> 102` means that course 101 must be taken before course 102. In the example used below, course 101 is Programming Fundamentals, course 102 is Object Oriented Programming, and course 103 is Data Structures. The chain `101 -> 102 -> 103` encodes that order.

## 2. Overall folder structure

```text
include/                 Class declarations and custom data structures
  Course.h               A single course record
  CourseDatabase.h       Course storage, lookup, sorting, and searching interface
  CourseGraph.h          Directed prerequisite graph interface
  DynamicArray.h         Resizable array template
  GraphQueue.h           Linked FIFO queue for BFS and topological sorting
  GraphStack.h           Linked LIFO stack for iterative DFS
  HashTable.h            Open-addressed course-code index interface
  SampleData.h           Unimplemented optional loader declarations
  Sorting.h              Five in-place sorts for DynamicArray<Course>
  StringSearch.h         Brute Force, LPS construction, and KMP
  StudentPlanner.h       Progress and academic planning interface
src/                     Implementations of the application modules
main.cpp                 Role menus, input, and module coordination
README.md                Project setup and feature summary
CODE_WALKTHROUGH.md      This viva-oriented guide
```

`SampleData.h` declares two possible loading functions, but there is no matching `.cpp` implementation and the current `main.cpp` does not call them. The program therefore begins with an empty catalogue.

## 3. How the program starts

`main()` in `main.cpp` constructs one `CourseDatabase`, one `CourseGraph`, and one `StudentPlanner`. The planner receives references to the other two objects, so it sees the same records and graph that the instructor changes. The outer loop prints the role selection. Selecting Instructor or Student enters that role's menu loop; choosing 0 from a role menu returns to role selection. Choosing 0 at the first menu exits.

`readInt(prompt, value)` reads a full line and parses exactly one integer. Extra non-whitespace characters make the input invalid, so input such as `2abc` is not accepted as 2. It repeats after invalid input and returns false if input ends. `readText(prompt, value)` reads a full, non-blank line, which permits course names containing spaces.

## 4. How all modules communicate

- `main.cpp` reads choices and calls public methods on the modules.
- `CourseDatabase` owns a `DynamicArray<Course>` for records and a `HashTable` containing course copies indexed by code.
- `CourseGraph` independently owns one vertex for each course and directed adjacency edges for prerequisites. `main.cpp` adds or removes graph vertices when a course is added or deleted from the database.
- `StudentPlanner` holds references to the database and graph. It queries both to calculate eligibility and paths.
- Sorting and searching are helper classes called by `CourseDatabase`; they operate on its array or on course-name text.

Both roles share the same data. The student menu has no completion-changing action. The instructor's completion option calls `StudentPlanner::markCompleted`, which validates prerequisites before it asks `CourseDatabase` to update the record and its hash-table copy.

## 5. Course class

**File:** `include/Course.h`, `src/Course.cpp`

`Course` is a value object with `id`, `code`, `name`, `creditHours`, and `completed`. The default constructor sets numeric fields to zero, strings to empty, and completion to false. The four-argument constructor copies the provided values and also starts incomplete. Database insertion performs validation; the constructor itself does not.

The `getID`, `getCode`, `getName`, and `getCreditHours` functions return copies of their fields. `isCompleted` returns the Boolean completion flag. `setCode`, `setName`, `setCreditHours`, and `setCompleted` change one field. A `Course` has no raw owning pointer; its `string` fields manage their own memory.

## 6. Dynamic Array

**File:** `include/DynamicArray.h`

`DynamicArray<T>` owns a heap array through `data`. `capacity` is the number of allocated slots; `count` is the number of used positions. The constructor allocates two slots. `pushBack(value)` doubles capacity when full, appends the value, then increases count. `resize(newCapacity)` allocates a new block, copies the used values, deletes the old block, and points `data` at the new allocation. Doubling makes repeated append O(1) amortized, although one append that resizes is O(n).

`removeAt(index)` checks the index, shifts later values one position to the left, and decreases count; it does not shrink capacity. `get(index)` has mutable and const overloads and throws `out_of_range` for an invalid position. `size`, `getCapacity`, and `isEmpty` report array state.

This class implements the Rule of Three:

- The copy constructor allocates a separate array and copies each used value.
- Copy assignment checks self-assignment, allocates and fills replacement storage, deletes the old storage, and takes ownership of the replacement.
- The destructor calls `delete[] data` exactly once for the owned array.

Without deep copying, two array objects could point at the same allocation and both delete it. This implementation is intended for types that can be default-constructed and assigned, such as `Course` and `int`.

## 7. Linked Lists

The graph and the queue/stack each use singly linked nodes.

In `CourseGraph`, a `Vertex` stores a course ID, a pointer to its first outgoing `Edge`, and a pointer to the next vertex. Each `Edge` stores the destination course ID and a pointer to the next edge in that vertex's adjacency list. Inserting an edge at the front of a list is O(1); finding a vertex or scanning a list takes linear time. The graph owns all vertices and edges. Its destructor deletes each edge chain before its vertex. Removing a course unlinks incoming edges from all vertices and then frees the target's outgoing edges and vertex.

Queue and stack nodes each store an integer course ID and one `next` pointer. Queue insertion is at the rear and removal at the front; stack insertion and removal are both at the top. Their destructors repeatedly remove nodes until empty.

## 8. Stack and Queue

**Files:** `include/GraphStack.h`, `include/GraphQueue.h`

The stack is **LIFO** (last in, first out). `push(value)` allocates a node, links it to the previous top, and makes it the new top. `pop(value)` returns false when empty; otherwise it copies the top value out, advances the top pointer, deletes the removed node, and returns true. Iterative DFS uses this stack to defer vertices for later exploration.

The queue is **FIFO** (first in, first out). It tracks both `frontNode` and `rearNode`. `enqueue(value)` appends a new node at the rear in O(1); for an empty queue, the new node becomes both ends. `dequeue(value)` removes the front in O(1), and when that was the last node it resets the rear pointer to null. BFS and Kahn's topological sort use this queue.

Copy construction and assignment are disabled for both classes because their raw node pointers represent unique ownership. Their destructors drain and delete all remaining nodes.

## 9. Hash Table and Rehashing

**File:** `include/HashTable.h`, `src/HashTable.cpp`

`HashTable` stores `Course` copies in `table`. `occupied[i]` says that slot contains a live course. `deleted[i]` is a tombstone flag. The table uses **open addressing with linear probing**, not separate chaining: after a collision at a bucket, it tests the next slot, wrapping around at the end.

`hashFunction(code)` lowercases the code, combines characters with a base-31 calculation, and takes the result modulo capacity. This maps a code to a starting slot. `searchByCode` uses the same normalization and follows the probe sequence. A never-used slot ends an unsuccessful search; a tombstone does not, because another copy of that code may lie farther along the sequence. Code lookup is expected O(1) at a moderate load and O(n) in the worst case.

`insert(course)` first rejects an existing code. If the projected load would exceed 70%, it calls `rehash`. It then probes from the code's bucket, remembers the first tombstone, and inserts at that tombstone or the first never-used slot. The `count` field tracks only live entries. `searchByID` and `remove(id)` scan the entire table because IDs are not the hash key. Removal marks a tombstone instead of clearing the slot as never-used, preserving code-search probe chains.

`rehash()` saves the old pointers, doubles capacity, creates fresh state arrays, and reinserts each occupied old course. Rehashing recomputes bucket positions for the new capacity. It then deletes all old arrays. `getCount` returns live count and `display` prints occupied slots.

The Rule of Three is implemented: the destructor frees all three arrays, the copy constructor allocates independent arrays and copies each slot, and assignment creates replacement arrays before deleting the old ones. `Course` itself safely copies its strings.

## 10. Course Database

**File:** `include/CourseDatabase.h`, `src/CourseDatabase.cpp`

The database uses two structures for different tasks: the dynamic array stores records in display/sort order and supports scanning by ID or name; the hash table provides code lookup. Any record inserted into the array is also inserted into the table. Update and completion changes replace the matching hash copy and array value; deletion removes from both. Sorting changes only the array order, so the code index remains valid.

Function guide:

| Function | Purpose and behavior |
|---|---|
| `addCourse(course)` | Rejects non-positive IDs, blank code/name, credits outside 1–6, duplicate IDs, and duplicate codes. Adds to the array, then hash table; removes the array copy if hash insertion fails. Returns success. |
| `updateCourse(id)` | Interactive overload. Finds the record, reads replacement code/name/credits from `cin`, and delegates to the parameter overload. Bad credit input is cleared and reported. |
| `updateCourse(id, code, name, credits)` | Validates fields and duplicate code, builds a temporary copy, updates the hash index, and then replaces the array record. Returns false if validation, lookup, or index update fails. |
| `deleteCourse(id)` | Finds the array position, removes the ID from the hash table, then shifts the array to close the gap. Returns false for a missing course or hash inconsistency. `main.cpp` also removes the corresponding graph vertex. |
| `searchByID(id, result)` | Scans the array and copies the first matching record to `result`; O(n). |
| `searchByCode(code, result)` | Delegates to the case-insensitive hash-table search. |
| `setCompleted(id, status)` | Builds an updated copy, refreshes the hash entry, then replaces the array record. Returns whether both stores were updated. |
| `displayAll()` | Prints each array record in its current order and shows completion state. |
| `getCount()` | Returns the dynamic-array size. |
| `getCourseAt(index, result)` | Checks bounds, then copies the array record to `result`. |
| `sortByID()` | Uses Merge Sort and a temporary `Course` array. Zero or one course needs no work. |
| `bubbleSort`, `insertionSort`, `selectionSort`, `combSort`, `shellSort` | Forward the owned course array to the matching custom algorithm in `Sorting.h`. |
| `searchByName(name, result)` | Lowercases the query and each title and returns the first title containing the query. Empty query returns false. |
| `searchByText(pattern, choice)` | Validates nonempty pattern and method 1/2, lowercases pattern and titles, runs Brute Force or KMP for each course, and prints all matching records. |

The Merge Sort helpers are `mergeSortCourses(courses, temp, left, right)` and `mergeCourses(courses, temp, left, mid, right)`. The recursive helper divides a range until one-element ranges remain. The merge helper copies the active range into `temp`, then uses indices `i` and `j` for the two halves and `k` for the output position. It repeatedly writes the smaller course ID and finally copies any remaining half. Sorting takes O(n log n) time in all cases and O(n) auxiliary storage.

## 11. Graph representation

**File:** `include/CourseGraph.h`, `src/CourseGraph.cpp`

Each course is a vertex. For 101 Programming Fundamentals, 102 Object Oriented Programming, and 103 Data Structures, edges `101 -> 102` and `102 -> 103` encode prerequisites. The adjacency list for 101 contains 102; the adjacency list for 102 contains 103. Therefore, the edge points from a prerequisite to its dependent. To display prerequisites of 103, the code scans all adjacency lists for edges ending at 103; to display dependents of 102, it follows 102's outgoing list.

`findVertex(courseID)` scans vertices and returns a non-owning pointer or null. `addCourse(courseID)` rejects non-positive or duplicate IDs and appends a vertex. `removeCourse(courseID)` removes incoming references, unlinks the vertex, frees its outgoing edges, and decrements the count. The graph owns these nodes; copy operations are disabled to prevent accidental sharing.

`addPrerequisite(courseID, prerequisiteID)` checks that both vertices exist, rejects a self-link and duplicate edge, creates the edge on the prerequisite vertex, and calls `hasCycle`. If the edge creates a cycle, it removes that edge and returns false. `removePrerequisite` searches only the prerequisite's outgoing list and unlinks the requested edge. `displayPrerequisites`, `displayDependents`, and `getPrerequisites` use the same edge direction. `getPrerequisites` first clears its output array; it returns false for a missing vertex and otherwise fills it with incoming source IDs.

## 12. BFS

`CourseGraph::BFS(startID)` performs breadth-first traversal over outgoing edges, so it visits the start course and then reachable dependents by increasing number of edges from the start. It rejects a missing start ID. It places the start in `GraphQueue` and in a `DynamicArray<int>` called `visited`. Each loop removes the front ID, prints it, finds its vertex, and examines outgoing edges. An unvisited destination is marked before enqueueing so it is not queued repeatedly. The loop ends when the queue is empty. The graph traversal idea is O(V+E), but this implementation's dynamic-array membership checks and vertex searches add linear-scan overhead.

## 13. DFS

`CourseGraph::DFS(startID)` uses `GraphStack`, not recursion. It pushes the start ID. Each iteration pops an ID; if that ID was already printed/visited, the duplicate pending stack entry is skipped. Otherwise it records and prints it, then pushes every outgoing dependent. The last pushed neighbor is explored first, so the traversal order depends on adjacency-list insertion order. It terminates when the stack is empty. It does not explicitly backtrack through recursive calls; pending stack entries represent the deferred branches.

## 14. Topological Sorting

`topologicalSort(order)` implements Kahn's algorithm. It first clears `order`, copies every vertex ID into `vertexIDs`, and initializes a parallel `indegree` array to zero. It counts incoming edges for each destination. Every zero-indegree ID enters `GraphQueue`. The algorithm dequeues one ID, appends it to `order`, and lowers the indegree of each outgoing dependent. A dependent enters the queue when its indegree reaches zero. Because edges point prerequisite → dependent, prerequisites appear first. If fewer than `vertexCount` IDs were output, some cycle kept vertices blocked; the function clears partial output and returns false.

This version uses linear scans of `vertexIDs` to find indegrees. Its time is O(VE + V²) in the worst case with these arrays/lists; the ID, indegree, queue, and output storage is O(V). A standard implementation with direct-index or hash lookup can achieve O(V+E), but this project keeps its custom array and linked queue.

## 15. Cycle Detection

`hasCycle()` starts a recursive DFS from every vertex that is not already in `visited`, so disconnected parts are checked too. `dfsCycleCheck(vertex, visited, active)` adds the current ID to both arrays. For each outgoing edge, a destination found in `active` is a back-edge to the current recursion path and proves a directed cycle. A destination found only in `visited` was fully explored and does not need another recursive call. On normal return the current ID is removed from `active`; `visited` remains for the rest of the whole-graph search.

A cycle such as `101 -> 102 -> 103 -> 101` would mean each course depends, directly or indirectly, on itself, so no valid prerequisite order exists. Normal application calls to `addPrerequisite` reject a new edge that creates such a cycle. `topologicalSort` also detects cycles by failing to process all vertices, and `generateRemainingPath` reports that it cannot make a path.

## 16. Prerequisite Management

The instructor menu asks for a dependent course ID and a prerequisite ID. Before modifying the graph, `main.cpp` verifies that both IDs exist in `CourseDatabase`. The graph method then creates the directed prerequisite-to-dependent edge. Self-links, duplicates, and cycles return false and produce an explanatory menu message. Removal uses the same two IDs and direction. Deleting a course from the instructor menu first removes the database record and then calls `removeCourse`, which also removes relationships touching that course. BFS/DFS and dependent displays follow outgoing edges; prerequisite displays inspect incoming edges.

## 17. All six sorting algorithms

All algorithms sort the `DynamicArray<Course>` by `Course::getID()` and modify the array in place. They move whole `Course` values, leaving the hash table's code lookup valid.

| Algorithm | What this implementation does | Time complexity | Auxiliary space |
|---|---|---|---|
| Bubble Sort | Compares adjacent IDs and swaps an out-of-order pair. Each pass moves a large remaining ID toward the end. There is no early-exit check. | Best/average/worst O(n²) for this implementation. | O(1) |
| Insertion Sort | Saves the next course as `key`, shifts larger IDs right in the sorted prefix, and places the key into the gap. | Best O(n); average/worst O(n²). | O(1) |
| Selection Sort | Scans the unsorted suffix for its minimum ID and swaps it into the next output position. | Best/average/worst O(n²). | O(1) |
| Comb Sort | Starts with a gap of n, shrinks it by roughly 1.3, and swaps pairs that far apart. It continues with gap 1 until a pass makes no swap. | Worst O(n²); typical average O(n²). Best/average behavior depends on the input and gap sequence. | O(1) |
| Shell Sort | Runs insertion-like shifts at gaps n/2, n/4, …, 1. | With this halving sequence: best O(n log n), average often described as about O(n^1.5), worst O(n²); the exact average depends on input. | O(1) |
| Merge Sort | Recursively splits ID ranges, then merges two sorted halves using a temporary array. | Best/average/worst O(n log n). | O(n) temporary array, plus O(log n) recursion stack |

The `Sorting` methods receive `DynamicArray<Course>&`, a reference, so their swaps and shifts change the database array directly. `CourseDatabase::sortByID` is the Merge Sort entry point; the other five database methods call their matching static function.

## 18. Brute Force

`StringSearch::bruteForce(text, pattern)` tests each possible start index in the text. At one start it compares characters from `j=0` forward. A mismatch ends that attempt and the next start index is tried; a complete pattern match returns its start index. An empty pattern returns 0. If no start works, it returns -1. With text length n and pattern length m, worst-case work is O(nm), and extra space is O(1). `CourseDatabase::searchByText` lowercases both strings first, making menu use case-insensitive.

## 19. KMP

KMP avoids rechecking a text prefix that already matched. `buildLPS(pattern, lps)` fills an array where `lps[i]` is the longest proper prefix of `pattern[0..i]` that is also its suffix. It tracks `length`, the current candidate prefix length, and `i`, the next pattern index. A match extends the candidate. On mismatch, a nonzero candidate falls back to `lps[length-1]` without moving i; if there is no candidate, that LPS position is zeroed and i advances.

`KMP(text, pattern)` allocates `m` integers for LPS, builds the array, then scans with text index i and pattern index j. Matching characters advance both. If j reaches m, the first match begins at `i-j`; the allocated array is deleted before returning. On a mismatch with j>0, j falls back to `lps[j-1]` and i stays in place. With j=0, i advances. At end of text it deletes LPS and returns -1. LPS creation and search take O(m+n) time in total, and the LPS array uses O(m) extra space. KMP avoids repeated comparisons, but it is not guaranteed to be faster for every short or easy input.

`buildLPS` is a helper intended for a non-empty pattern and a caller-provided array with at least m slots. The current code writes `lps[0]` unconditionally; see the known issue note in the final report for direct calls with an empty pattern.

## 20. Student Planner

**File:** `include/StudentPlanner.h`, `src/StudentPlanner.cpp`

`StudentPlanner` contains references to `CourseDatabase` and `CourseGraph`. It does not own either object.

- `markCompleted(courseID)` searches the database; it then asks the graph for direct prerequisites and checks that each prerequisite is present and already completed. Only then does it call `database.setCompleted(courseID, true)`. It returns false for a missing course, missing graph vertex/prerequisite, or incomplete prerequisite.
- `displayCompletedCourses()` scans all database positions and prints the records with a true flag. It reports when the scan finds none.
- `isEligible(courseID)` returns false for a missing or completed course, a missing graph vertex, a missing prerequisite record, or any incomplete direct prerequisite. If all direct prerequisites are complete, it returns true.
- `displayEligibleCourses()` scans database records and prints those for which `isEligible` is true.
- `displayLockedCourses()` selects incomplete courses that are not eligible, then prints their direct prerequisites that are still incomplete.
- `generateRemainingPath()` asks the graph for a topological order. On a cycle it prints an error. Otherwise it looks up the ordered IDs, omits completed courses, and prints the remaining IDs as code/name pairs. If none remain it reports all courses complete.

Eligibility checks direct prerequisites. Transitive ordering is provided by topological sorting; in a valid graph, prerequisites in earlier levels still must be completed before a course's direct prerequisites can themselves be completed.

## 21. Academic path generation

For the chain `101 -> 102 -> 103`, Kahn's algorithm puts 101 before 102 and 102 before 103. If 101 and 102 are complete, the planner prints only course 103. If the graph contains a cycle, `topologicalSort` returns false and the planner prints `Cannot generate path. Cycle detected.` It never prints a partial order as if it were valid.

## 22. Main menu and application flow

The first menu offers Instructor, Student, and Exit. `showCourseMenus(instructor)` displays one menu based on the selected role. The instructor menu supports course CRUD, searches, prerequisites, graph traversals, completion, completed-course display, sorting, and cycle checking. The student menu supports browsing/search, progress, eligibility, locked courses, graph displays/traversals, remaining path, and name search; it has no mark-completed branch.

Small helpers keep input/display work separate:

- `readInt` validates a whole-line integer and reports bad input.
- `readText` rejects blank strings.
- `displayCourse` prints one record.
- `searchCourse` dispatches to ID, code, or name lookup and displays one result.
- `searchCourseName` chooses Brute Force or KMP and calls `searchByText`.
- `showCourseMenus` prints the role's action list.

In the main loop, each menu choice dispatches to the module that owns the operation. Course addition is rolled back from the database if graph vertex creation fails. Course deletion removes the graph vertex after database deletion succeeds. Prerequisite actions first check both IDs against the database. The completion branch is guarded by `instructor && choice == 10`; students entering 10 get an invalid-choice message. Choosing 0 in the inner loop returns to role selection; choosing 0 there ends the program. End-of-file also exits cleanly.

## 23. Memory management and Rule of Three

- `DynamicArray<T>` owns `data[]` and has a destructor, copy constructor, and copy-assignment operator. Those operations allocate/copy independent storage so one array cannot delete another array's memory.
- `HashTable` owns three arrays and also implements a destructor, deep-copy constructor, and deep-copy assignment.
- `CourseGraph` owns vertex and edge nodes, frees them in its destructor, and disables copying because a shallow copy would make both graphs own the same nodes.
- `GraphStack` and `GraphQueue` own linked nodes, delete them as they are popped/dequeued and drain leftovers in their destructors. Copying is disabled for the same ownership reason.
- `Course` contains `string` objects rather than raw owning pointers; the standard string type handles its own memory.
- KMP allocates the LPS array with `new[]` and deletes it on both the match and no-match return paths.

## How to Explain This Project in a Viva

Start with the problem: courses have prerequisites, so a student needs to know which courses are available now and what order remains. Explain the roles and then the data model. A `Course` is one record; the custom dynamic array stores records for traversal and sorting, while the hash table indexes codes. The directed graph stores prerequisite relationships as prerequisite-to-dependent edges.

Next explain traversal tools: BFS uses a FIFO queue to explore reachable dependents by layers; iterative DFS uses a LIFO stack to follow one pending branch; Kahn's algorithm uses indegrees and a queue to create a valid prerequisite order; cycle detection uses visited vertices and the active recursion path. Then cover the six sorting algorithms and the two substring searches. Finish with the planner's direct-prerequisite completion checks and how the main menu prevents students from changing official completion.

### Likely viva questions

**Q: What does an edge `101 -> 102` mean?**  
A: Course 101 is a prerequisite of course 102. The graph stores the edge on vertex 101, whose adjacency list contains dependent course 102.

**Q: Why use both a dynamic array and a hash table?**  
A: The array is convenient for displaying and sorting all records. The hash table provides faster average course-code lookup. They contain copies, so updates must refresh both.

**Q: Is the hash table using chaining?**  
A: No. It uses open addressing with linear probing. After a collision it checks the next slot. Tombstones preserve probe paths after deletion.

**Q: Does the hash table use course ID for hashing?**  
A: No. Course code is the hash key. ID search and deletion currently scan occupied slots.

**Q: Why does BFS use a queue and DFS use a stack?**  
A: A queue is FIFO, so BFS explores earlier discovered levels first. A stack is LIFO, so the most recently deferred neighbor is explored next in iterative DFS.

**Q: How does topological sorting know which course to take next?**  
A: It starts with zero-indegree vertices. When a prerequisite is processed, it lowers the indegree of each dependent; a dependent becomes available when that count reaches zero.

**Q: How is a cycle detected by the recursive DFS?**  
A: `visited` marks explored vertices and `active` marks the current recursion route. An edge to a vertex still in `active` is a cycle.

**Q: Why can a course be locked?**  
A: It is incomplete and at least one of its direct prerequisites is incomplete or missing. `displayLockedCourses` prints incomplete direct prerequisites by name/code.

**Q: Can students mark courses complete?**  
A: The student menu does not contain a completion action. Only the instructor menu dispatches to `StudentPlanner::markCompleted`.

**Q: What is the LPS array in KMP?**  
A: Each position stores the longest proper prefix ending at that position that is also a suffix. On a mismatch, KMP uses that length to keep a useful part of the matched pattern instead of restarting from its first character.

**Q: What does the Rule of Three protect here?**  
A: For classes that own raw dynamic memory, the destructor, copy constructor, and copy-assignment operator must agree on who owns each allocation. Deep copies prevent shared pointers from being deleted twice.

**Q: Why does Merge Sort need extra space?**  
A: The merge step copies the active range into a temporary array so values are not overwritten before being compared. This version uses O(n) auxiliary storage.

**Q: Where is sample data loaded?**  
A: It is not loaded in the current program. `SampleData.h` only declares optional functions; there is no implementation file and `main.cpp` does not call them.
