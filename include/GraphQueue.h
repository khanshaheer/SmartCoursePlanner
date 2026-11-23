
#ifndef GRAPH_QUEUE_H
#define GRAPH_QUEUE_H

// Linked-list FIFO queue used by BFS and topological sorting. The queue owns
// each node until dequeue removes it or the destructor drains the remaining nodes.

/*It is particularly important because your Graph Module uses this queue in:
1. BFS (Breadth-First Search).
2. Topological Sorting (Kahn's Algorithm).*/

class GraphQueue

{
private:
    // One queued course ID and the link to the following queue node.
    struct Node//node of our queue
    {
        int data;     // Course ID held by this queue position.
        Node* next;   // stoers addres of next node

        Node(int value)
        {
            data = value;
            next = NULL;
        }
    };

    Node* frontNode; // Points to the first node, which will be removed next
    Node* rearNode;  // Points to the last node, where new nodes are added

public:
    // Start with no nodes; both ends are NULL.
    GraphQueue()
    {
        frontNode = NULL;
        rearNode = NULL;
    }

    // Repeatedly dequeue so every remaining node is deleted.
    ~GraphQueue()//Because we create nodes dynamically using new.
    {
        int value;

        while (dequeue(value))
        {
        }
    }

    // Nodes are owned by this queue, so copying is disabled to prevent shared pointers.
    // It means we prevent copying of GraphQueue objects.
    /*
    If we copied the queue normally, both objects could point to the same dynamically allocated nodes.

Then both destructors might try to delete the same memory.

That creates a memory-management problem.

So we disable copying.
    */
    GraphQueue(const GraphQueue&) = delete;
    GraphQueue& operator=(const GraphQueue&) = delete;

    // FIFO empty test: when front is NULL, rear is also NULL.
    bool isEmpty() const
    {
        return frontNode == NULL;
    }

    // Allocate a node at the rear; when empty it becomes both front and rear.
    void enqueue(int value)
    {
        Node* newNode = new Node(value);

        if (isEmpty())
        {
            frontNode = newNode;
            rearNode = newNode;
        }
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }
    }

    // Remove the front node and write its value through the reference parameter.
    // Return false without changing value when the queue is empty.
    bool dequeue(int& value)
    {
        if (isEmpty())
        {
            return false;
        }

        Node* temp = frontNode;
        value = temp->data;

        frontNode = frontNode->next;

        if (frontNode == NULL)
        {
            rearNode = NULL;
        }

        delete temp;
        return true;
    }
};

#endif
