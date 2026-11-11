
#ifndef COURSE_GRAPH_H
#define COURSE_GRAPH_H

#include <iostream>
#include "DynamicArray.h"

using namespace std;

class CourseGraph
{
private:

    struct Edge
    {
        int courseID;
        Edge* next;

        Edge(int id)
        {
            courseID = id;
            next = NULL;
        }
    };

    struct Vertex
    {
        int courseID;
        Edge* firstEdge;
        Vertex* next;

        Vertex(int id)
        {
            courseID = id;
            firstEdge = NULL;
            next = NULL;
        }
    };

    Vertex* head;
    int vertexCount;

    Vertex* findVertex(int courseID) const;

    bool dfsCycleCheck(
        Vertex* vertex,
        DynamicArray<int>& visited,
        DynamicArray<int>& active
    ) const;

public:

    CourseGraph();
    ~CourseGraph();

    bool addCourse(int courseID);
    bool removeCourse(int courseID);

    bool addPrerequisite(int courseID, int prerequisiteID);
    bool removePrerequisite(int courseID, int prerequisiteID);

    void displayPrerequisites(int courseID) const;
    void displayDependents(int courseID) const;

    void BFS(int startID) const;
    void DFS(int startID) const;

    bool hasCycle() const;

    bool topologicalSort(DynamicArray<int>& order) const;

    bool getPrerequisites(
        int courseID,
        DynamicArray<int>& result
    ) const;

    int getVertexCount() const;
};

#endif
