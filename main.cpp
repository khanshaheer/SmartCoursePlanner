
#include <iostream>
#include "CourseGraph.h"

using namespace std;

int main()
{
    CourseGraph graph;

    graph.addCourse(101);
    graph.addCourse(201);
    graph.addCourse(301);
    graph.addCourse(401);

    // 101 is prerequisite of 201
    graph.addPrerequisite(201, 101);

    // 201 is prerequisite of 301
    graph.addPrerequisite(301, 201);

    // 101 is prerequisite of 401
    graph.addPrerequisite(401, 101);

    graph.displayPrerequisites(301);

    graph.displayDependents(101);

    cout << "\n";

    graph.BFS(101);
    graph.DFS(101);

    cout << "\nCycle exists: ";

    if (graph.hasCycle())
        cout << "Yes";
    else
        cout << "No";

    cout << "\n\nTopological Order: ";

    DynamicArray<int> order;

    if (graph.topologicalSort(order))
    {
        for (int i = 0; i < order.size(); i++)
        {
            cout << order.get(i) << " ";
        }
    }
    else
    {
        cout << "Cannot generate order.";
    }

    cout << "\n\nTrying to create a cycle:\n";

    if (graph.addPrerequisite(101, 301))
        cout << "Prerequisite added.";
    else
        cout << "Rejected because it creates a cycle.";

    cout << endl;

    return 0;
}
