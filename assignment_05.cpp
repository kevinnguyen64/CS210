#include <iostream>

// Each array has a fixed capacity of five so the demonstration can show a full case.
class ArrayStack {
private:
    static constexpr int capacity = 5;
    int items[capacity]{};
    int count = 0;

public:
    bool push(int value) {
        if (count == capacity) return false;
        items[count++] = value;
        return true;
    }

    bool pop(int& value) {
        if (count == 0) return false;
        value = items[--count];
        return true;
    }

    void print() const {
        std::cout << "Final stack (top first): ";
        if (count == 0) std::cout << "empty";
        for (int i = count - 1; i >= 0; --i) std::cout << items[i] << ' ';
        std::cout << '\n';
    }
};

class LinkedStack {
private:
    struct Node { int value; Node* next; };
    Node* topNode = nullptr;

public:
    LinkedStack() = default;
    LinkedStack(const LinkedStack&) = delete;
    LinkedStack& operator=(const LinkedStack&) = delete;
    ~LinkedStack() { int ignored; while (pop(ignored)) {} }

    bool push(int value) {
        topNode = new Node{value, topNode};
        return true;
    }

    bool pop(int& value) {
        if (topNode == nullptr) return false;
        Node* old = topNode;
        value = old->value;
        topNode = old->next;
        delete old;
        return true;
    }

    void print() const {
        std::cout << "Final stack (top first): ";
        if (topNode == nullptr) std::cout << "empty";
        for (Node* node = topNode; node != nullptr; node = node->next) std::cout << node->value << ' ';
        std::cout << '\n';
    }
};

class ArrayQueue {
private:
    static constexpr int capacity = 5;
    int items[capacity]{};
    int frontIndex = 0;
    int count = 0;

public:
    bool enqueue(int value) {
        if (count == capacity) return false;
        items[(frontIndex + count) % capacity] = value;
        ++count;
        return true;
    }

    bool dequeue(int& value) {
        if (count == 0) return false;
        value = items[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        --count;
        return true;
    }

    void print() const {
        std::cout << "Final queue (front first): ";
        if (count == 0) std::cout << "empty";
        for (int i = 0; i < count; ++i) std::cout << items[(frontIndex + i) % capacity] << ' ';
        std::cout << '\n';
    }
};

class LinkedQueue {
private:
    struct Node { int value; Node* next; };
    Node* first = nullptr;
    Node* last = nullptr;

public:
    LinkedQueue() = default;
    LinkedQueue(const LinkedQueue&) = delete;
    LinkedQueue& operator=(const LinkedQueue&) = delete;
    ~LinkedQueue() { int ignored; while (dequeue(ignored)) {} }

    bool enqueue(int value) {
        Node* node = new Node{value, nullptr};
        if (last == nullptr) first = node;
        else last->next = node;
        last = node;
        return true;
    }

    bool dequeue(int& value) {
        if (first == nullptr) return false;
        Node* old = first;
        value = old->value;
        first = old->next;
        if (first == nullptr) last = nullptr;
        delete old;
        return true;
    }

    void print() const {
        std::cout << "Final queue (front first): ";
        if (first == nullptr) std::cout << "empty";
        for (Node* node = first; node != nullptr; node = node->next) std::cout << node->value << ' ';
        std::cout << '\n';
    }
};

class ArrayDeque {
private:
    static constexpr int capacity = 5;
    int items[capacity]{};
    int frontIndex = 0;
    int count = 0;

public:
    bool pushFront(int value) {
        if (count == capacity) return false;
        frontIndex = (frontIndex + capacity - 1) % capacity;
        items[frontIndex] = value;
        ++count;
        return true;
    }

    bool pushBack(int value) {
        if (count == capacity) return false;
        items[(frontIndex + count) % capacity] = value;
        ++count;
        return true;
    }

    bool popFront(int& value) {
        if (count == 0) return false;
        value = items[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        --count;
        return true;
    }

    bool popBack(int& value) {
        if (count == 0) return false;
        value = items[(frontIndex + count - 1) % capacity];
        --count;
        return true;
    }

    void print() const {
        std::cout << "Final deque (front first): ";
        if (count == 0) std::cout << "empty";
        for (int i = 0; i < count; ++i) std::cout << items[(frontIndex + i) % capacity] << ' ';
        std::cout << '\n';
    }
};

class LinkedDeque {
private:
    struct Node { int value; Node* previous; Node* next; };
    Node* first = nullptr;
    Node* last = nullptr;

public:
    LinkedDeque() = default;
    LinkedDeque(const LinkedDeque&) = delete;
    LinkedDeque& operator=(const LinkedDeque&) = delete;
    ~LinkedDeque() { int ignored; while (popFront(ignored)) {} }

    bool pushFront(int value) {
        Node* node = new Node{value, nullptr, first};
        if (first == nullptr) last = node;
        else first->previous = node;
        first = node;
        return true;
    }

    bool pushBack(int value) {
        Node* node = new Node{value, last, nullptr};
        if (last == nullptr) first = node;
        else last->next = node;
        last = node;
        return true;
    }

    bool popFront(int& value) {
        if (first == nullptr) return false;
        Node* old = first;
        value = old->value;
        first = old->next;
        if (first == nullptr) last = nullptr;
        else first->previous = nullptr;
        delete old;
        return true;
    }

    bool popBack(int& value) {
        if (last == nullptr) return false;
        Node* old = last;
        value = old->value;
        last = old->previous;
        if (last == nullptr) first = nullptr;
        else last->next = nullptr;
        delete old;
        return true;
    }

    void print() const {
        std::cout << "Final deque (front first): ";
        if (first == nullptr) std::cout << "empty";
        for (Node* node = first; node != nullptr; node = node->next) std::cout << node->value << ' ';
        std::cout << '\n';
    }
};

void added(int& step, const char* action, bool success) {
    std::cout << step++ << ". " << action << " -> " << (success ? "ok" : "full") << '\n';
}

void removed(int& step, const char* action, bool success, int& value) {
    std::cout << step++ << ". " << action << " -> ";
    if (success) std::cout << value << '\n';
    else std::cout << "empty\n";
}

template <typename Stack>
void demonstrateStack(const char* name, bool fixedCapacity) {
    Stack stack;
    int step = 1;
    int value = 0;
    std::cout << "\n=== " << name << " ===\n";
    removed(step, "pop", stack.pop(value), value);
    added(step, "push 10", stack.push(10));
    added(step, "push 20", stack.push(20));
    added(step, "push 30", stack.push(30));
    added(step, "push 40", stack.push(40));
    added(step, "push 50", stack.push(50));
    added(step, "push 60", stack.push(60));
    removed(step, "pop", stack.pop(value), value);
    removed(step, "pop", stack.pop(value), value);
    removed(step, "pop", stack.pop(value), value);
    added(step, "push 70", stack.push(70));
    for (int i = 0; i < 4; ++i) removed(step, "pop", stack.pop(value), value);
    if (!fixedCapacity) removed(step, "pop", stack.pop(value), value);
    stack.print();
    std::cout << "Operations: " << step - 1 << '\n';
}

template <typename Queue>
void demonstrateQueue(const char* name, bool fixedCapacity) {
    Queue queue;
    int step = 1;
    int value = 0;
    std::cout << "\n=== " << name << " ===\n";
    removed(step, "dequeue", queue.dequeue(value), value);
    added(step, "enqueue 10", queue.enqueue(10));
    added(step, "enqueue 20", queue.enqueue(20));
    added(step, "enqueue 30", queue.enqueue(30));
    added(step, "enqueue 40", queue.enqueue(40));
    added(step, "enqueue 50", queue.enqueue(50));
    added(step, "enqueue 60", queue.enqueue(60));
    removed(step, "dequeue", queue.dequeue(value), value);
    removed(step, "dequeue", queue.dequeue(value), value);
    added(step, "enqueue 70", queue.enqueue(70));
    added(step, "enqueue 80", queue.enqueue(80));
    for (int i = 0; i < 6; ++i) removed(step, "dequeue", queue.dequeue(value), value);
    if (!fixedCapacity) removed(step, "dequeue", queue.dequeue(value), value);
    queue.print();
    std::cout << "Operations: " << step - 1 << '\n';
}

template <typename Deque>
void demonstrateDeque(const char* name) {
    Deque deque;
    int step = 1;
    int value = 0;
    std::cout << "\n=== " << name << " ===\n";
    removed(step, "popFront", deque.popFront(value), value);
    added(step, "pushBack 10", deque.pushBack(10));
    added(step, "pushBack 20", deque.pushBack(20));
    added(step, "pushBack 30", deque.pushBack(30));
    added(step, "pushFront 5", deque.pushFront(5));
    added(step, "pushBack 40", deque.pushBack(40));
    added(step, "pushFront 1", deque.pushFront(1));
    removed(step, "popBack", deque.popBack(value), value);
    added(step, "pushFront 2", deque.pushFront(2));
    removed(step, "popFront", deque.popFront(value), value);
    removed(step, "popBack", deque.popBack(value), value);
    removed(step, "popFront", deque.popFront(value), value);
    removed(step, "popFront", deque.popFront(value), value);
    removed(step, "popBack", deque.popBack(value), value);
    removed(step, "popFront", deque.popFront(value), value);
    deque.print();
    std::cout << "Operations: " << step - 1 << '\n';
}

int main() {
    demonstrateStack<ArrayStack>("Array stack", true);
    demonstrateStack<LinkedStack>("Linked stack", false);
    demonstrateQueue<ArrayQueue>("Array queue", true);
    demonstrateQueue<LinkedQueue>("Linked queue", false);
    demonstrateDeque<ArrayDeque>("Array deque");
    demonstrateDeque<LinkedDeque>("Linked deque");
    return 0;
}
