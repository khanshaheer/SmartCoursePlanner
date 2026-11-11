template <class T>
class DynamicArray
{
private:
    T* data;
    int capacity;
    int count;

public:
    DynamicArray();

    DynamicArray(const DynamicArray<T>& other);

    DynamicArray<T>& operator=(
        const DynamicArray<T>& other
    );

    ~DynamicArray();

    void pushBack(const T& value);
    void removeAt(int index);

    T& get(int index);
    const T& get(int index) const;

    int size() const;
    bool isEmpty() const;
};