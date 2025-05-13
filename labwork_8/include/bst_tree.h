#pragma once

#include "bst.h"

// BinarySearchTree

template<
    typename T,
    typename Compare = std::less<T>,
    typename Allocator = std::allocator<T>,
    typename TraversalTag = InOrderTag>
class BinarySearchTree {
    using Node = TreeNode<T>;
    Node* root = nullptr;
    Compare comp;
    Allocator alloc;

    using Iter = typename IteratorSelector<TraversalTag, Node, T>::type;

public:
    BinarySearchTree() = default;

    ~BinarySearchTree() { clear(root); }

    void insert(const T& value) {
        Node* newNode = new Node(value);
        if (!root) {
            root = newNode;
            return;
        }

        Node* current = root;
        while (true) {
            if (comp(value, current->value)) {
                if (!current->left) {
                    current->left = newNode;
                    newNode->parent = current;
                    break;
                }
                current = current->left;
            }
            else {
                if (!current->right) {
                    current->right = newNode;
                    newNode->parent = current;
                    break;
                }
                current = current->right;
            }
        }
    }

    Iter begin() const { return Iter(root); }
    Iter end() const { return Iter(root, true); }

private:

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};
