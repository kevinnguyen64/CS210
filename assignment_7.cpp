#include <algorithm>
#include <iostream>

struct Node {
    int data;
    Node* left;
    Node* right;

    explicit Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root = nullptr;

    static Node* insert(Node* node, int value) {
        if (node == nullptr) return new Node(value);
        if (value < node->data)
            node->left = insert(node->left, value);
        else if (value > node->data)
            node->right = insert(node->right, value);
        return node; // Ignore duplicates.
    }

    static bool search(const Node* node, int value) {
        if (node == nullptr) return false;
        if (value == node->data) return true;
        return value < node->data ? search(node->left, value) : search(node->right, value);
    }

    static Node* findMin(Node* node) {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    static Node* remove(Node* node, int value) {
        if (node == nullptr) return nullptr;

        if (value < node->data) {
            node->left = remove(node->left, value);
        } else if (value > node->data) {
            node->right = remove(node->right, value);
        } else {
            // Leaf: no children.
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }

            // One child: return that child to the parent.
            if (node->left == nullptr) {
                Node* child = node->right;
                delete node;
                return child;
            }
            if (node->right == nullptr) {
                Node* child = node->left;
                delete node;
                return child;
            }

            // Two children: replace with the inorder successor.
            Node* successor = findMin(node->right);
            node->data = successor->data;
            node->right = remove(node->right, successor->data);
        }

        return node;
    }

    static void inorder(const Node* node) {
        if (node == nullptr) return;
        inorder(node->left);
        std::cout << node->data << ' ';
        inorder(node->right);
    }

    static void preorder(const Node* node) {
        if (node == nullptr) return;
        std::cout << node->data << ' ';
        preorder(node->left);
        preorder(node->right);
    }

    static void postorder(const Node* node) {
        if (node == nullptr) return;
        postorder(node->left);
        postorder(node->right);
        std::cout << node->data << ' ';
    }

    static int height(const Node* node) {
        // Height counts edges: empty = -1, leaf = 0.
        if (node == nullptr) return -1;
        return 1 + std::max(height(node->left), height(node->right));
    }

    static void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BinarySearchTree() = default;
    ~BinarySearchTree() { clear(root); }

    // Prevent shallow copies of owning pointers.
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    void insert(int value) { root = insert(root, value); }
    bool search(int value) const { return search(root, value); }
    void remove(int value) { root = remove(root, value); }
    void inorder() const { inorder(root); std::cout << '\n'; }
    void preorder() const { preorder(root); std::cout << '\n'; }
    void postorder() const { postorder(root); std::cout << '\n'; }
    int height() const { return height(root); }
};

int main() {
    BinarySearchTree tree;
    const int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 55, 65};

    std::cout << "Inserted: ";
    for (int value : values) {
        tree.insert(value);
        std::cout << value << ' ';
    }
    std::cout << '\n';

    std::cout << "Inorder: "; tree.inorder();
    std::cout << "Preorder: "; tree.preorder();
    std::cout << "Postorder: "; tree.postorder();
    std::cout << "Height (edges): " << tree.height() << '\n';
    std::cout << "Search 55: " << (tree.search(55) ? "found" : "not found") << '\n';
    std::cout << "Search 100: " << (tree.search(100) ? "found" : "not found") << '\n';

    std::cout << "\nRemove 40 (leaf)\n";
    tree.remove(40);
    std::cout << "Inorder: "; tree.inorder();

    std::cout << "\nRemove 20 (one child: 10)\n";
    tree.remove(20);
    std::cout << "Inorder: "; tree.inorder();

    std::cout << "\nRemove 70 (two children: 60 and 80)\n";
    tree.remove(70);
    std::cout << "Inorder: "; tree.inorder();
    std::cout << "Final height (edges): " << tree.height() << '\n';

    return 0;
}