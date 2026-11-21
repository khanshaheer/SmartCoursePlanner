
#include "CourseGraph.h"
#include "GraphQueue.h"
#include "GraphStack.h"

// Create an empty graph: there is no first vertex and the recorded vertex count is zero.
CourseGraph::CourseGraph()
{
    head = NULL;
    vertexCount = 0;
}

// Walk the vertex linked list and return the node with courseID.
// The caller does not own the returned pointer; it remains owned by this graph.
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

// Release every adjacency edge before deleting its vertex. Each next pointer is
// saved before delete because reading a deleted node would be invalid.
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

// Add one positive course ID as a vertex. The graph owns the new node, and a
// duplicate ID is rejected so each course has exactly one vertex.
bool CourseGraph::addCourse(int courseID)
{
    if (courseID <= 0 || findVertex(courseID) != NULL)
    {
        return false;
    }

    // Allocate a detached vertex first, then link it at the end of the vertex list.
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

// Remove the vertex and all graph relationships involving it. First unlink every
// incoming edge from other vertices; then unlink and destroy its own outgoing list.
bool CourseGraph::removeCourse(int courseID)
{
    Vertex* target = findVertex(courseID);

    if (target == NULL)
    {
        return false;
    }

    // Edges point prerequisite -> dependent, so incoming edges identify this course
    // as the dependent at the edge destination.
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

    // Unlink the target vertex from the head or from its previous vertex.
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

    // The target owns its outgoing edges; free those nodes before freeing target.
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


// Add prerequisiteID -> courseID. Both vertices must exist, a self-edge and
// duplicate edge are invalid, and any new directed cycle is rolled back.
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

    // Store the edge on the prerequisite vertex so traversal follows toward dependents.
    Edge* newEdge = new Edge(courseID);

    newEdge->next = prerequisite->firstEdge;
    prerequisite->firstEdge = newEdge;

    // Checking after insertion tests the complete proposed graph. If cyclic, remove
    // the just-added edge to restore the previous graph exactly.
    if (hasCycle())
    {
        removePrerequisite(courseID, prerequisiteID);
        return false;
    }

    return true;
}

// Find and unlink the edge prerequisiteID -> courseID from the prerequisite's list.
// A missing prerequisite vertex or missing edge leaves the graph unchanged.
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



// Prerequisites are incoming edges, so scan every source vertex's adjacency list
// and print source IDs whose edge destination is courseID.
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

// Dependents are outgoing edges stored directly at courseID's vertex.
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

// Replace result with the source IDs of all incoming edges. Clearing first makes
// this function safe when the caller reuses an array from an earlier query.
bool CourseGraph::getPrerequisites(
    int courseID,
    DynamicArray<int>& result
) const
{
    if (findVertex(courseID) == NULL)
    {
        return false;
    }

    while (!result.isEmpty())
    {
        result.removeAt(result.size() - 1);
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


// Breadth-first traversal follows prerequisite -> dependent edges. The queue visits
// nearer reachable courses before farther ones; visited prevents repeated enqueueing.
// The custom array's linear membership checks add overhead beyond ideal O(V+E) BFS.
void CourseGraph::BFS(int startID) const
{
    if (findVertex(startID) == NULL)
    {
        cout << "Starting course not found.\n";
        return;
    }

    GraphQueue queue;
    DynamicArray<int> visited;

    // Seed the search with its start vertex and mark it before adding more vertices.
    queue.enqueue(startID);
    visited.pushBack(startID);

    cout << "BFS Traversal: ";

    while (!queue.isEmpty())
    {
        int currentID;
        queue.dequeue(currentID);

        cout << currentID << " ";

        // Find each reached vertex and inspect its outgoing dependent-course edges.
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


// Iterative depth-first traversal uses the custom LIFO stack rather than recursion.
// IDs are marked when popped; duplicate pending IDs may be pushed, then skipped later.
void CourseGraph::DFS(int startID) const
{
    if (findVertex(startID) == NULL)
    {
        cout << "Starting course not found.\n";
        return;
    }

    GraphStack stack;
    DynamicArray<int> visited;

    // Begin with the requested vertex. Popping a vertex makes its outgoing neighbors
    // the next work items, so the most recently pushed neighbor is explored first.
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

        // Mark only when popped; a later duplicate stack entry will be ignored.
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


// DFS helper for directed cycle detection. visited records vertices already explored;
// active records only the current recursion route. An edge back to active is a cycle.
bool CourseGraph::dfsCycleCheck(
    Vertex* vertex,
    DynamicArray<int>& visited,
    DynamicArray<int>& active
) const
{
    // Enter this recursion branch by marking the vertex globally and on the active path.
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

        // A back-edge to the current recursion path closes a directed cycle.
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

        // A fully explored vertex cannot reveal a new cycle from this branch.
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

// Start directed DFS from every not-yet-visited vertex, including disconnected parts.
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



// Kahn's algorithm repeatedly removes a zero-indegree course. Because edges point
// prerequisite -> dependent, a course enters the queue only after its prerequisites
// have been processed. False means a cycle prevented processing every vertex.
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

    // Count incoming prerequisites for every destination vertex.
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

    // No incoming edges means no remaining prerequisite blocks the course.
    for (int i = 0; i < vertexIDs.size(); i++)
    {
        if (indegree.get(i) == 0)
        {
            queue.enqueue(vertexIDs.get(i));
        }
    }

    // Removing a prerequisite from the active graph lowers each dependent's indegree.
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

    // A cycle keeps every vertex in it above indegree zero, so fewer than V are output.
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

// Return the maintained vertex count; edge counts are not included.
int CourseGraph::getVertexCount() const
{
    return vertexCount;
}


