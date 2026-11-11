
#include "CourseGraph.h"
#include "GraphQueue.h"
#include "GraphStack.h"

// Constructor
CourseGraph::CourseGraph()
{
    head = NULL;
    vertexCount = 0;
}

// Find a vertex using its course ID
CourseGraph::Vertex* CourseGraph::findVertex(int courseID) const
{
    Vertex* current = head;

    while (current != NULL)
    {
        if (current->courseID == courseID)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

// Destructor
CourseGraph::~CourseGraph()
{
    Vertex* current = head;

    while (current != NULL)
    {
        Edge* edge = current->firstEdge;

        while (edge != NULL)
        {
            Edge* tempEdge = edge;
            edge = edge->next;
            delete tempEdge;
        }

        Vertex* tempVertex = current;
        current = current->next;

        delete tempVertex;
    }

    head = NULL;
    vertexCount = 0;
}

// Add a course as a vertex
bool CourseGraph::addCourse(int courseID)
{
    if (findVertex(courseID) != NULL)
    {
        return false;
    }

    Vertex* newVertex = new Vertex(courseID);

    if (head == NULL)
    {
        head = newVertex;
    }
    else
    {
        Vertex* current = head;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = newVertex;
    }

    vertexCount++;

    return true;
}

// Remove a course and all its connections
bool CourseGraph::removeCourse(int courseID)
{
    Vertex* target = findVertex(courseID);

    if (target == NULL)
    {
        return false;
    }

    // Remove edges pointing towards this course
    Vertex* current = head;

    while (current != NULL)
    {
        Edge* edge = current->firstEdge;
        Edge* previous = NULL;

        while (edge != NULL)
        {
            if (edge->courseID == courseID)
            {
                Edge* temp = edge;

                if (previous == NULL)
                {
                    current->firstEdge = edge->next;
                }
                else
                {
                    previous->next = edge->next;
                }

                edge = edge->next;
                delete temp;
            }
            else
            {
                previous = edge;
                edge = edge->next;
            }
        }

        current = current->next;
    }

    // Remove the vertex itself
    Vertex* previous = NULL;
    current = head;

    while (current != NULL && current->courseID != courseID)
    {
        previous = current;
        current = current->next;
    }

    if (previous == NULL)
    {
        head = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    // Delete its outgoing edges
    Edge* edge = current->firstEdge;

    while (edge != NULL)
    {
        Edge* temp = edge;
        edge = edge->next;
        delete temp;
    }

    delete current;
    vertexCount--;

    return true;
}


// Add a prerequisite relationship
bool CourseGraph::addPrerequisite(int courseID, int prerequisiteID)
{
    Vertex* course = findVertex(courseID);
    Vertex* prerequisite = findVertex(prerequisiteID);

    // Both courses must exist
    if (course == NULL || prerequisite == NULL)
    {
        return false;
    }

    // A course cannot be its own prerequisite
    if (courseID == prerequisiteID)
    {
        return false;
    }

    // Check for duplicate edge
    Edge* current = prerequisite->firstEdge;

    while (current != NULL)
    {
        if (current->courseID == courseID)
        {
            return false;
        }

        current = current->next;
    }

    // Create edge from prerequisite to course
    Edge* newEdge = new Edge(courseID);

    newEdge->next = prerequisite->firstEdge;
    prerequisite->firstEdge = newEdge;

    // Reject the edge if it creates a cycle
    if (hasCycle())
    {
        removePrerequisite(courseID, prerequisiteID);
        return false;
    }

    return true;
}

// Remove a prerequisite relationship
bool CourseGraph::removePrerequisite(int courseID, int prerequisiteID)
{
    Vertex* prerequisite = findVertex(prerequisiteID);

    if (prerequisite == NULL)
    {
        return false;
    }

    Edge* current = prerequisite->firstEdge;
    Edge* previous = NULL;

    while (current != NULL)
    {
        if (current->courseID == courseID)
        {
            if (previous == NULL)
            {
                prerequisite->firstEdge = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}



// Display prerequisites of a course
void CourseGraph::displayPrerequisites(int courseID) const
{
    if (findVertex(courseID) == NULL)
    {
        cout << "Course not found in graph.\n";
        return;
    }

    bool found = false;
    Vertex* current = head;

    cout << "Prerequisites of course " << courseID << ": ";

    while (current != NULL)
    {
        Edge* edge = current->firstEdge;

        while (edge != NULL)
        {
            if (edge->courseID == courseID)
            {
                cout << current->courseID << " ";
                found = true;
            }

            edge = edge->next;
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "No prerequisites";
    }

    cout << endl;
}

// Display courses that depend on this course
void CourseGraph::displayDependents(int courseID) const
{
    Vertex* current = findVertex(courseID);

    if (current == NULL)
    {
        cout << "Course not found in graph.\n";
        return;
    }

    Edge* edge = current->firstEdge;

    cout << "Courses depending on " << courseID << ": ";

    if (edge == NULL)
    {
        cout << "No dependent courses";
    }

    while (edge != NULL)
    {
        cout << edge->courseID << " ";
        edge = edge->next;
    }

    cout << endl;
}

// Return prerequisites through a DynamicArray
bool CourseGraph::getPrerequisites(
    int courseID,
    DynamicArray<int>& result
) const
{
    if (findVertex(courseID) == NULL)
    {
        return false;
    }

    Vertex* current = head;

    while (current != NULL)
    {
        Edge* edge = current->firstEdge;

        while (edge != NULL)
        {
            if (edge->courseID == courseID)
            {
                result.pushBack(current->courseID);
            }

            edge = edge->next;
        }

        current = current->next;
    }

    return true;
}


// Breadth First Search
void CourseGraph::BFS(int startID) const
{
    if (findVertex(startID) == NULL)
    {
        cout << "Starting course not found.\n";
        return;
    }

    GraphQueue queue;
    DynamicArray<int> visited;

    queue.enqueue(startID);
    visited.pushBack(startID);

    cout << "BFS Traversal: ";

    while (!queue.isEmpty())
    {
        int currentID;
        queue.dequeue(currentID);

        cout << currentID << " ";

        Vertex* current = findVertex(currentID);
        Edge* edge = current->firstEdge;

        while (edge != NULL)
        {
            bool alreadyVisited = false;

            for (int i = 0; i < visited.size(); i++)
            {
                if (visited.get(i) == edge->courseID)
                {
                    alreadyVisited = true;
                    break;
                }
            }

            if (!alreadyVisited)
            {
                visited.pushBack(edge->courseID);
                queue.enqueue(edge->courseID);
            }

            edge = edge->next;
        }
    }

    cout << endl;
}


// Depth First Search
void CourseGraph::DFS(int startID) const
{
    if (findVertex(startID) == NULL)
    {
        cout << "Starting course not found.\n";
        return;
    }

    GraphStack stack;
    DynamicArray<int> visited;

    stack.push(startID);

    cout << "DFS Traversal: ";

    while (!stack.isEmpty())
    {
        int currentID;
        stack.pop(currentID);

        bool alreadyVisited = false;

        for (int i = 0; i < visited.size(); i++)
        {
            if (visited.get(i) == currentID)
            {
                alreadyVisited = true;
                break;
            }
        }

        if (alreadyVisited)
        {
            continue;
        }

        visited.pushBack(currentID);
        cout << currentID << " ";

        Vertex* current = findVertex(currentID);
        Edge* edge = current->firstEdge;

        while (edge != NULL)
        {
            stack.push(edge->courseID);
            edge = edge->next;
        }
    }

    cout << endl;
}


// Check whether an ID exists in an array
bool CourseGraph::dfsCycleCheck(
    Vertex* vertex,
    DynamicArray<int>& visited,
    DynamicArray<int>& active
) const
{
    visited.pushBack(vertex->courseID);
    active.pushBack(vertex->courseID);

    Edge* edge = vertex->firstEdge;

    while (edge != NULL)
    {
        int nextID = edge->courseID;

        bool inActive = false;

        for (int i = 0; i < active.size(); i++)
        {
            if (active.get(i) == nextID)
            {
                inActive = true;
                break;
            }
        }

        if (inActive)
        {
            return true;
        }

        bool inVisited = false;

        for (int i = 0; i < visited.size(); i++)
        {
            if (visited.get(i) == nextID)
            {
                inVisited = true;
                break;
            }
        }

        if (!inVisited)
        {
            Vertex* nextVertex = findVertex(nextID);

            if (dfsCycleCheck(nextVertex, visited, active))
            {
                return true;
            }
        }

        edge = edge->next;
    }

    // Remove this vertex from the current recursion path
    for (int i = 0; i < active.size(); i++)
    {
        if (active.get(i) == vertex->courseID)
        {
            active.removeAt(i);
            break;
        }
    }

    return false;
}

// Check the entire graph for a cycle
bool CourseGraph::hasCycle() const
{
    DynamicArray<int> visited;
    DynamicArray<int> active;

    Vertex* current = head;

    while (current != NULL)
    {
        bool alreadyVisited = false;

        for (int i = 0; i < visited.size(); i++)
        {
            if (visited.get(i) == current->courseID)
            {
                alreadyVisited = true;
                break;
            }
        }

        if (!alreadyVisited)
        {
            if (dfsCycleCheck(current, visited, active))
            {
                return true;
            }
        }

        current = current->next;
    }

    return false;
}



// Topological Sort using Kahn's Algorithm
bool CourseGraph::topologicalSort(DynamicArray<int>& order) const
{
    // Make sure the output array starts empty
    while (!order.isEmpty())
    {
        order.removeAt(order.size() - 1);
    }

    DynamicArray<int> vertexIDs;
    DynamicArray<int> indegree;

    Vertex* current = head;

    // Store all vertex IDs and initialize indegrees
    while (current != NULL)
    {
        vertexIDs.pushBack(current->courseID);
        indegree.pushBack(0);

        current = current->next;
    }

    // Calculate indegree of every vertex
    current = head;

    while (current != NULL)
    {
        Edge* edge = current->firstEdge;

        while (edge != NULL)
        {
            for (int i = 0; i < vertexIDs.size(); i++)
            {
                if (vertexIDs.get(i) == edge->courseID)
                {
                    indegree.get(i)++;
                    break;
                }
            }

            edge = edge->next;
        }

        current = current->next;
    }

    GraphQueue queue;

    // Add all vertices with indegree zero
    for (int i = 0; i < vertexIDs.size(); i++)
    {
        if (indegree.get(i) == 0)
        {
            queue.enqueue(vertexIDs.get(i));
        }
    }

    // Process vertices
    while (!queue.isEmpty())
    {
        int currentID;
        queue.dequeue(currentID);

        order.pushBack(currentID);

        Vertex* vertex = findVertex(currentID);
        Edge* edge = vertex->firstEdge;

        while (edge != NULL)
        {
            for (int i = 0; i < vertexIDs.size(); i++)
            {
                if (vertexIDs.get(i) == edge->courseID)
                {
                    indegree.get(i)--;

                    if (indegree.get(i) == 0)
                    {
                        queue.enqueue(edge->courseID);
                    }

                    break;
                }
            }

            edge = edge->next;
        }
    }

    // If not all vertices were processed, a cycle exists
    if (order.size() != vertexCount)
    {
        while (!order.isEmpty())
        {
            order.removeAt(order.size() - 1);
        }

        return false;
    }

    return true;
}

// Return number of vertices
int CourseGraph::getVertexCount() const
{
    return vertexCount;
}


