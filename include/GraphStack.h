
#ifndef GRAPH_STACK_H
#define GRAPH_STACK_H

class GraphStack
{
private:
    struct Node
    {
        int data;
        Node* next;

        Node(int value)
        {
            data = value;
            next = NULL;
        }
    };

    Node* topNode;

public:
    GraphStack()
    {
        topNode = NULL;
    }

    ~GraphStack()
    {
        int value;

        while (pop(value))
        {
        }
    }

    bool isEmpty() const
    {
        return topNode == NULL;
    }

    void push(int value)
    {
        Node* newNode = new Node(value);

        newNode->next = topNode;
        topNode = newNode;
    }

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
