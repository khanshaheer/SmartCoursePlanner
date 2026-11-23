
#ifndef COURSE_GRAPH_H
#define COURSE_GRAPH_H

#include <iostream>
#include "DynamicArray.h"

using namespace std;

// Directed adjacency-list graph: an edge A -> B means A is a prerequisite of B.
// For example, 101 -> 102 means Programming Fundamentals (101) comes before
// Object Oriented Programming (102), and 102 -> 103 can lead to Data Structures.
class CourseGraph//Its responsibility is to represent courses and their prerequisite relationships.
{
private:
    // One outgoing adjacency-list entry; courseID is the dependent course at its end.

    
    struct Edge//We are creating a structure to represent one connection in our graph.
    {
        int courseID; // Destination ID for this directed edge.
        Edge *next;   // Next outgoing edge in the same linked list.

        Edge(int id)
        {
            courseID = id;
            next = NULL;
        }
    };

    // One graph vertex owns the list of courses that depend directly on it.
    struct Vertex//A Vertex represents an actual course in our graph.

        /* Vertex represents a course.
        Edge represents a connection between courses.*/

    {
        int courseID;    // Stores the ID of the destination course.
        Edge *firstEdge; // First dependent-course edge, or NULL when none exist.
        Vertex *next;    // Next vertex in the graph's linked list.

        Vertex(int id)
        {
            courseID = id;
            firstEdge = NULL;
            next = NULL;
        }
    };

    Vertex *head;    // head points to the first vertex in the graph.

    /*Because we need a starting point to access all vertices in our linked list*/

    int vertexCount; // Number of vertices, kept in sync by add/remove operations.

    // Find a vertex by ID, returning NULL if it is not present.
    Vertex *findVertex(int courseID) const;

    // Recursive directed-cycle check. visited spans finished work; active is the
    // current recursion path, so reaching an active ID proves a cycle.
    bool dfsCycleCheck(
        Vertex *vertex,
        DynamicArray<int> &visited, //Vertices already explored
        DynamicArray<int> &active) const; //Vertices currently on the recursive path

        //Because a vertex that was visited earlier is not necessarily part of a cycle
        //A vertex encountered again on the current recursion path indicates a directed cycle.
        

public:
    // Start with an empty adjacency list.
    CourseGraph();
    // Release every edge list and vertex owned by this graph.
    ~CourseGraph();//Because our graph creates nodes using dynamic memory allocation (new). 
                    //We must release that memory using delete to avoid memory leaks.

    // Raw linked-list ownership is not copied; copying is disabled to avoid shared nodes.
    CourseGraph(const CourseGraph &) = delete;
    CourseGraph &operator=(const CourseGraph &) = delete;

    // Add a positive course ID as a vertex; false means invalid or already present.
    bool addCourse(int courseID);//creates a new vertex
    // Remove a vertex and every edge entering or leaving it; false if absent.
    bool removeCourse(int courseID);

    // Add prerequisiteID -> courseID if both vertices exist and no cycle is created.
    bool addPrerequisite(int courseID, int prerequisiteID);
    // Remove the edge prerequisiteID -> courseID; false if that edge is absent.
    bool removePrerequisite(int courseID, int prerequisiteID);

    // Print incoming prerequisite IDs for courseID.
    void displayPrerequisites(int courseID) const;
    // Print outgoing dependent-course IDs for courseID.
    void displayDependents(int courseID) const;

    // Traverse reachable outgoing edges from startID using GraphQueue (BFS).
    void BFS(int startID) const;
    // Traverse reachable outgoing edges from startID using GraphStack (iterative DFS).
    void DFS(int startID) const;

    // Return true if any directed cycle exists in the graph.
    bool hasCycle() const;

    // Fill order using Kahn's algorithm; false on a cycle. Existing order is cleared first.
    bool topologicalSort(DynamicArray<int> &order) const;

    // Replace result with incoming prerequisite IDs; false if courseID is not a vertex.
    bool getPrerequisites(//The first function fills the supplied array with the prerequisites of a course.
        int courseID,
        DynamicArray<int> &result) const;

    // Return the current number of vertices.
    int getVertexCount() const;
};

#endif
