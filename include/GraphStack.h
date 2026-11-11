#ifndef GRAPH_STACK_H
#define GRAPH_STACK_H

class GraphStack
{
private:
    struct Node
    {
        int data;
        Node* next;
    };

    Node* topNode;

public:
    GraphStack();
    ~GraphStack();

    void push(int value);
    int pop();

    bool isEmpty() const;
};

#endif