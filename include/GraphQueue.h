
#ifndef GRAPH_QUEUE_H
#define GRAPH_QUEUE_H

class GraphQueue
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

    Node* frontNode;
    Node* rearNode;

public:
    GraphQueue()
    {
        frontNode = NULL;
        rearNode = NULL;
    }

    ~GraphQueue()
    {
        int value;

        while (dequeue(value))
        {
        }
    }

    bool isEmpty() const
    {
        return frontNode == NULL;
    }

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
