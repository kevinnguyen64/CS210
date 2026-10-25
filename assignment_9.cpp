// Kevin Nguyen
// Assignment 09: Min-Heap and Treap

#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

// Part 1: Min-Heap
class MinHeap {
private:
    std::vector<int> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;

            if (heap[parent] <= heap[index]) {
                break;
            }

            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void heapifyDown(int index) {
        int size = static_cast<int>(heap.size());

        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left] < heap[smallest]) {
                smallest = left;
            }

            if (right < size && heap[right] < heap[smallest]) {
                smallest = right;
            }

            if (smallest == index) {
                break;
            }

            std::swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    void insert(int value) {
        heap.push_back(value);
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    int peekMin() const {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        return heap[0];
    }

    int removeMin() {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        int minimum = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            heapifyDown(0);
        }

        return minimum;
    }

    void print() const {
        for (int value : heap) {
            std::cout << value << " ";
        }

        std::cout << "\n";
    }
};

// Part 2: Treap
class Treap {
private:
    struct Node {
        int key;
        int priority;
        Node* left;
        Node* right;

        Node(int k, int p)
            : key(k), priority(p), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    // Move the left child above its parent.
    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;

        return newRoot;
    }

    // Move the right child above its parent.
    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;

        return newRoot;
    }

    Node* insertNode(Node* node, int key, int priority) {
        if (node == nullptr) {
            return new Node(key, priority);
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key, priority);

            if (node->left->priority < node->priority) {
                node = rotateRight(node);
            }
        } else if (key > node->key) {
            node->right = insertNode(node->right, key, priority);

            if (node->right->priority < node->priority) {
                node = rotateLeft(node);
            }
        }

        // Duplicate keys are ignored.
        return node;
    }

    // Inorder traversal displays keys in ascending order.
    void printInorder(const Node* node) const {
        if (node == nullptr) {
            return;
        }

        printInorder(node->left);
        std::cout << "(" << node->key << ", " << node->priority << ") ";
        printInorder(node->right);
    }

    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    Treap() = default;

    // Prevent copying a tree that owns dynamically allocated nodes.
    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;

    ~Treap() {
        destroy(root);
    }

    void insert(int key, int priority) {
        root = insertNode(root, key, priority);
    }

    bool search(int key) const {
        const Node* current = root;

        while (current != nullptr) {
            if (key == current->key) {
                return true;
            }

            if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return false;
    }

    void print() const {
        printInorder(root);
        std::cout << "\n";
    }
};

int main() {
    // Part 1: Build a min-heap from 10 integers.
    MinHeap heap;
    std::vector<int> values = {40, 15, 30, 10, 50, 20, 35, 5, 25, 45};

    std::cout << "Part 1: Min-Heap\n";

    for (int value : values) {
        heap.insert(value);
    }

    std::cout << "Initial heap: ";
    heap.print();

    std::cout << "Minimum: " << heap.peekMin() << "\n";

    heap.insert(3);
    std::cout << "After inserting 3: ";
    heap.print();

    heap.insert(12);
    std::cout << "After inserting 12: ";
    heap.print();

    std::cout << "Removed minimum: " << heap.removeMin() << "\n";
    std::cout << "After first removal: ";
    heap.print();

    std::cout << "Removed minimum: " << heap.removeMin() << "\n";
    std::cout << "After second removal: ";
    heap.print();

    std::cout << "New minimum: " << heap.peekMin() << "\n";

    // Part 2: Build a treap from 10 key-priority pairs.
    Treap treap;
    std::vector<std::pair<int, int>> pairs = {
        {50, 40}, {30, 60}, {70, 50}, {20, 80}, {40, 30},
        {60, 70}, {80, 20}, {10, 90}, {25, 55}, {65, 10}
    };

    for (const auto& pair : pairs) {
        treap.insert(pair.first, pair.second);
    }

    std::cout << "\nPart 2: Treap\n";
    std::cout << "Inorder traversal (key, priority):\n";
    treap.print();

    std::cout << "Search for 25: " << (treap.search(25) ? "Found" : "Not found") << "\n";
    std::cout << "Search for 65: " << (treap.search(65) ? "Found" : "Not found") << "\n";
    std::cout << "Search for 99: " << (treap.search(99) ? "Found" : "Not found") << "\n";

    return 0;
}
