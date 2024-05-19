#include <iostream>
#include "Queue.h"
using namespace std;

class Tree
{
private:
    struct Node
    {
        int data;
        Node* left;
        Node* right;
        Node(int data) : data(data), left(nullptr), right(nullptr) {}
    };

    int size;
    Node* root;
    void clearnode(Node* node)
    {
        if (node == nullptr) return;
        clearnode(node->left);
        clearnode(node->right);
        delete node;
    }

    Node* deleteByMerging(Node*& node)
    {
        Node* tmp = node;
        if (node != nullptr)
        {
            if (!node->right) node = node->left;
            else if (node->left == nullptr) node = node->right;
            else
            {
                tmp = node->left;
                while (tmp->right != nullptr) tmp = tmp->right;
                tmp->right = node->right;
                tmp = node;
                node = node->left;
            }
            delete tmp;
            size--;
            return node;
        }
        return nullptr;
    }

    void traversePrefix(Node* node, int* arr, int& index) const
    {
        if (node == nullptr) return;
        arr[index++] = node->data;
        traversePrefix(node->left, arr, index);
        traversePrefix(node->right, arr, index);
    }

    void traverseInfix(Node* node, int* arr, int& index) const
    {
        if (node == nullptr) return;
        traverseInfix(node->left, arr, index);
        arr[index++] = node->data;
        traverseInfix(node->right, arr, index);
    }

    void traversePostfix(Node* node, int* arr, int& index) const
    {
        if (node == nullptr) return;
        traversePostfix(node->left, arr, index);
        traversePostfix(node->right, arr, index);
        arr[index++] = node->data;
    }

    void traverseLevelsUpLeft(Node* node, int* arr, int& index) const
    {
        if (node == nullptr) return;
        Queue<Node*> q;
        q.queue(node);
        while (!q.is_empty())
        {
            Node* current = q.unqueue();
            arr[index++] = current->data;
            if (current->left) q.queue(current->left);
            if (current->right) q.queue(current->right);
        }
    }

    void traverseLevelsUpRight(Node* node, int* arr, int& index) const
    {
        if (node == nullptr) return;
        Queue<Node*> q;
        q.queue(node);
        while (!q.is_empty())
        {
            Node* current = q.unqueue();
            arr[index++] = current->data;
            if (current->right) q.queue(current->right);
            if (current->left) q.queue(current->left);
        }
    }

    void traverseLevelsDownLeft(Node* node, int* arr, int& index) const
    {
        if (node == nullptr) return;
        Queue<Node*> q;
        q.queue(node);
        int* tempArr = new int[size];
        int count = 0;
        while (!q.is_empty())
        {
            Node* current = q.unqueue();
            tempArr[count++] = current->data;
            if (current->left) q.queue(current->left);
            if (current->right) q.queue(current->right);
        }
        for (int i = count - 1; i >= 0; --i)
        {
            arr[index++] = tempArr[i];
        }
        delete[] tempArr;
    }
    

    void traverseLevelsDownRight(Node* node, int* arr, int& index) const
    {
        if (node == nullptr) return;
        Queue<Node*> q;
        q.queue(node);
        int* tempArr = new int[size];
        int count = 0;
        while (!q.is_empty())
        {
            Node* current = q.unqueue();
            tempArr[count++] = current->data;
            if (current->right) q.queue(current->right);
            if (current->left) q.queue(current->left);
        }
        for (int i = count - 1; i >= 0; --i)
        {
            arr[index++] = tempArr[i];
        }
        delete[] tempArr;
    }

    Node* rotateRight(Node*& node)
    {
        if (node == nullptr || node->left == nullptr) return node;
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    Node* rotateLeft(Node*& node)
    {
        if (node == nullptr || node->right == nullptr) return node;
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    Node* findAndRotateRight(Node* node, int data)
    {
        if (node == nullptr) return nullptr;
        if (data < node->data)
        {
            node->left = findAndRotateRight(node->left, data);
        }
        else if (data > node->data)
        {
            node->right = findAndRotateRight(node->right, data);
        }
        else
        {
            node = rotateRight(node);
        }
        return node;
    }

    Node* findAndRotateLeft(Node* node, int data)
    {
        if (node == nullptr) return nullptr;
        if (data < node->data)
        {
            node->left = findAndRotateLeft(node->left, data);
        }
        else if (data > node->data)
        {
            node->right = findAndRotateLeft(node->right, data);
        }
        else
        {
            node = rotateLeft(node);
        }
        return node;
    }
    Node* buildBalancedTree(int* arr, int start, int end)
    {
        if (start > end) return nullptr;
        int mid = (start + end) / 2;
        Node* node = new Node(arr[mid]);
        node->left = buildBalancedTree(arr, start, mid - 1);
        node->right = buildBalancedTree(arr, mid + 1, end);
        return node;
    }

public:
    enum Order { Prefix, Infix, Postfix, LevelsUpLeft, LevelsUpRight, LevelsDownLeft, LevelsDownRight };
    Tree();
    ~Tree();
    void add(int data);
    void remove(int data);
    bool contains(int data);
    int count() const;
    void clear();
    int* ToArray(Order order) const;
    int* ToArray();
    void ToRight(int data);
    void ToLeft(int data);
    void Balance();
};

Tree::Tree()
{
    size = 0;
    root = nullptr;
}

Tree::~Tree()
{
    clear();
}

void Tree::add(int data)
{
    if (contains(data)) return;
    Node* newnode = new Node(data);
    if (root == nullptr)
    {
        root = newnode;
    }
    else {
        Node* current = root;
        while (true)
        {
            if (data < current->data)
            {
                if (current->left == nullptr)
                {
                    current->left = newnode;
                    break;
                }
                current = current->left;
            }
            else
            {
                if (current->right == nullptr)
                {
                    current->right = newnode;
                    break;
                }
                current = current->right;
            }
        }
    }
    size++;
}

void Tree::remove(int data)
{
    Node* node = root, * prev = nullptr;
    while (node != nullptr)
    {
        if (node->data == data) break;
        prev = node;
        if (data < node->data) node = node->left;
        else node = node->right;
    }
    if (node != nullptr && node->data == data)
    {
        if (node == root) root = deleteByMerging(root);
        else if (prev->left == node) prev->left = deleteByMerging(prev->left);
        else prev->right = deleteByMerging(prev->right);
    }
}

bool Tree::contains(int data)
{
    Node* current = root;
    while (current != nullptr)
    {
        if (current->data == data) return true;
        if (data < current->data) current = current->left;
        else current = current->right;
    }
    return false;
}

int Tree::count() const
{
    return size;
}

void Tree::clear()
{
    clearnode(root);
    root = nullptr;
    size = 0;
}

int* Tree::ToArray(Order order) const
{
    int* arr = new int[size];
    int index = 0;
    switch (order)
    {
    case Prefix:
        traversePrefix(root, arr, index);
        break;
    case Infix:
        traverseInfix(root, arr, index);
        break;
    case Postfix:
        traversePostfix(root, arr, index);
        break;
    case LevelsUpLeft:
        traverseLevelsUpLeft(root, arr, index);
        break;
    case LevelsUpRight:
        traverseLevelsUpRight(root, arr, index);
        break;
    case LevelsDownLeft:
        traverseLevelsDownLeft(root, arr, index);
        break;
    case LevelsDownRight:
        traverseLevelsDownRight(root, arr, index);
        break;
    }
    return arr;
}

int* Tree::ToArray()
{
    return ToArray(Infix);
}

void Tree::ToRight(int data)
{
    root = findAndRotateRight(root, data);
}

void Tree::ToLeft(int data)
{
    root = findAndRotateLeft(root, data);
}

void Tree::Balance()
{
    if (root == nullptr) return;
    int* arr = ToArray(Infix);
    root = buildBalancedTree(arr, 0, size - 1);
    delete[] arr;
}

int main()
{
    Tree tree;
    int choice;
    do {
        cout << "1. Add an element. " << endl;
        cout << "2. Remove an element. " << endl;
        cout << "3. Check if an element is in the tree. " << endl;
        cout << "4. Size of the tree. " << endl;
        cout << "5. Clear the tree. " << endl;
        cout << "6. Show tree elements in order. " << endl;
        cout << "7. Rotate right at a node. " << endl;
        cout << "8. Rotate left at a node. " << endl;
        cout << "9. Balance the tree. " << endl;
        cout << "0. Exit. " << endl;
        cin >> choice;
        int value, traversal;
        int* res;
        switch (choice) {
        case 1:
            cout << "Enter the element you want to add: " << endl;
            cin >> value;
            tree.add(value);
            break;
        case 2:
            cout << "Enter the element to remove: " << endl;
            cin >> value;
            tree.remove(value);
            break;
        case 3:
            cout << "Enter the element to check: " << endl;
            cin >> value;
            if (tree.contains(value)) {
                cout << "The element is in the tree. " << endl;
            }
            else {
                cout << "The element is not in the tree. " << endl;
            }
            break;
        case 4:
            cout << "The size of the tree is: " << tree.count() << endl;
            break;
        case 5:
            tree.clear();
            cout << "The tree is cleared. " << endl;
            break;
        case 6:
            cout << "Select order of traversal:\n";
            cout << "   1. Prefix\n";
            cout << "   2. Infix\n";
            cout << "   3. Postfix\n";
            cout << "   4. Levels Up Left\n";
            cout << "   5. Levels Up Right\n";
            cout << "   6. Levels Down Left\n";
            cout << "   7. Levels Down Right\n";
            cout << "Enter your choice: ";
            cin >> traversal;
            switch (traversal) {
            case 1:
                res = tree.ToArray(Tree::Prefix);
                break;
            case 2:
                res = tree.ToArray(Tree::Infix);
                break;
            case 3:
                res = tree.ToArray(Tree::Postfix);
                break;
            case 4:
                res = tree.ToArray(Tree::LevelsUpLeft);
                break;
            case 5:
                res = tree.ToArray(Tree::LevelsUpRight);
                break;
            case 6:
                res = tree.ToArray(Tree::LevelsDownLeft);
                break;
            case 7:
                res = tree.ToArray(Tree::LevelsDownRight);
                break;
            default:
                cout << "Invalid traversal choice.\n";
                continue;
            }
            cout << "Tree elements: ";
            for (int i = 0; i < tree.count(); i++) {
                cout << res[i] << " ";
            }
            cout << "\n";
            delete[] res;
            break;
        case 7:
            cout << "Enter the element to rotate right at: " << endl;
            cin >> value;
            tree.ToRight(value);
            break;
        case 8:
            cout << "Enter the element to rotate left at: " << endl;
            cin >> value;
            tree.ToLeft(value);
            break;
        case 9:
            tree.Balance();
            cout << "The tree is balanced. " << endl;
            break;
        case 0:
            break;
        default:
            cout << "An unknown command. Try again. " << endl;
            break;
        }
    } while (choice != 0);
    return 0;
}
