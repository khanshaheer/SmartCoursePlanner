
#ifndef GRAPH_STACK_H
#define GRAPH_STACK_H

// Linked-list LIFO stack used by iterative DFS. Every pushed node is owned by
// this stack and is deleted when popped or when the stack is destroyed.
class GraphStack
{
private:
    // One stack value and a link to the node beneath it.
    struct Node
    {
        int data;   // Course ID stored at this level.
        Node* next; // Previous top node, or NULL at the bottom.

        Node(int value)
        {
            data = value;
            next = NULL;
        }
    };

    Node* topNode; // Current top element; NULL means the stack has no nodes.

public:
    // Start with an empty stack.
    GraphStack()
    {
        topNode = NULL;
    }

    // Pop repeatedly so no linked nodes remain allocated.
    ~GraphStack()
    {
        int value;

        while (pop(value))
        {
        }
    }

    // Nodes are owned by this stack, so copying is disabled to prevent shared pointers.
    GraphStack(const GraphStack&) = delete;
    GraphStack& operator=(const GraphStack&) = delete;

    // Return true when there is no top node.
    bool isEmpty() const
    {
        return topNode == NULL;
    }

    // Allocate a new top node and link it to the old top.
    void push(int value)
    {
        Node* newNode = new Node(value);

        newNode->next = topNode;
        topNode = newNode;
    }

    // Remove the top node and write its ID to value; false means the stack is empty.
    bool pop(int& value)
    {
        if (isEmpty())
        {
            return false;
        }

        Node* temp = topNode;
        value = temp->data;

        topNode = topNode->next;

        delete temp;
        return true;
    }
};

#endif
