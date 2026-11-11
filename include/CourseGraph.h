#ifndef COURSE_GRAPH_H
#define COURSE_GRAPH_H

#include "DynamicArray.h"
#include "CourseDatabase.h"

class CourseGraph
{
private:

    struct Edge
    {
        int courseID;
        Edge* next;
    };

    struct Vertex
    {
        int courseID;
        Edge* firstEdge;
        Vertex* next;
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

    bool addPrerequisite(
        int courseID,
        int prerequisiteID
    );

    bool removePrerequisite(
        int courseID,
        int prerequisiteID
    );

    void displayPrerequisites(int courseID) const;
    void displayDependents(int courseID) const;

    void BFS(int startID) const;
    void DFS(int startID) const;

    bool hasCycle() const;

    bool topologicalSort(
        DynamicArray<int>& order
    ) const;

    bool getPrerequisites(
        int courseID,
        DynamicArray<int>& result
    ) const;

    int getVertexCount() const;
};

#endif