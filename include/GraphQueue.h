#ifndef GRAPH_QUEUE_H
#define GRAPH_QUEUE_H

class GraphQueue
{
private:
    struct Node
    {
        int data;
        Node* next;
    };

    Node* frontNode;
    Node* rearNode;

public:
    GraphQueue();
    ~GraphQueue();

    void enqueue(int value);
    int dequeue();

    bool isEmpty() const;
};

#endif