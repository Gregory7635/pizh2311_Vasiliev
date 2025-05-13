#pragma once

#include <stack>
#include <iterator>

// Tags for traversal type
struct InOrderTag {};
struct PreOrderTag {};
struct PostOrderTag {};

// Node structure template
template<typename T>
struct TreeNode {
    T value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode* parent = nullptr;

    TreeNode(const T& val) : value(val) {}
};

// InOrder Iterator
template<typename Node, typename T>
class InOrderIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    InOrderIterator() : current(nullptr) {}
    InOrderIterator(Node* root) { current = leftmost(root); }
    InOrderIterator(Node* root, bool) : current(nullptr) {}

    reference operator*() const { return current->value; }
    pointer operator->() const { return &current->value; }

    InOrderIterator& operator++() {
        if (!current) return *this;
        if (current->right) {
            current = leftmost(current->right);
        }
        else {
            Node* p = current->parent;
            while (p && current == p->right) {
                current = p;
                p = p->parent;
            }
            current = p;
        }
        return *this;
    }

    InOrderIterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    bool operator==(const InOrderIterator& other) const { return current == other.current; }
    bool operator!=(const InOrderIterator& other) const { return !(*this == other); }

private:
    Node* current;
    Node* leftmost(Node* node) const {
        while (node && node->left)
            node = node->left;
        return node;
    }
};

// PreOrder Iterator
template<typename Node, typename T>
class PreOrderIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    PreOrderIterator() : current(nullptr) {}
    PreOrderIterator(Node* root) { if (root) stack.push(root); advance(); }
    PreOrderIterator(Node* root, bool) : current(nullptr) {}

    reference operator*() const { return current->value; }
    pointer operator->() const { return &current->value; }

    PreOrderIterator& operator++() {
        advance();
        return *this;
    }

    PreOrderIterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    bool operator==(const PreOrderIterator& other) const { return current == other.current; }
    bool operator!=(const PreOrderIterator& other) const { return !(*this == other); }

private:
    Node* current;
    std::stack<Node*> stack;

    void advance() {
        if (stack.empty()) {
            current = nullptr;
            return;
        }
        current = stack.top(); stack.pop();
        if (current->right) stack.push(current->right);
        if (current->left) stack.push(current->left);
    }
};

// PostOrder Iterator
template<typename Node, typename T>
class PostOrderIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    PostOrderIterator() : current(nullptr) {}
    PostOrderIterator(Node* root) { fill_stack(root); advance(); }
    PostOrderIterator(Node* root, bool) : current(nullptr) {}

    reference operator*() const { return current->value; }
    pointer operator->() const { return &current->value; }

    PostOrderIterator& operator++() {
        advance();
        return *this;
    }

    PostOrderIterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    bool operator==(const PostOrderIterator& other) const { return current == other.current; }
    bool operator!=(const PostOrderIterator& other) const { return !(*this == other); }

private:
    std::stack<Node*> stack;
    Node* current;

    void fill_stack(Node* node) {
        std::stack<Node*> temp;
        if (node) temp.push(node);
        while (!temp.empty()) {
            Node* curr = temp.top(); temp.pop();
            stack.push(curr);
            if (curr->left) temp.push(curr->left);
            if (curr->right) temp.push(curr->right);
        }
    }

    void advance() {
        if (stack.empty()) {
            current = nullptr;
            return;
        }
        current = stack.top();
        stack.pop();
    }
};

// IteratorSelector
template<typename Tag, typename Node, typename T>
struct IteratorSelector;

template<typename Node, typename T>
struct IteratorSelector<InOrderTag, Node, T> {
    using type = InOrderIterator<Node, T>;
};

template<typename Node, typename T>
struct IteratorSelector<PreOrderTag, Node, T> {
    using type = PreOrderIterator<Node, T>;
};

template<typename Node, typename T>
struct IteratorSelector<PostOrderTag, Node, T> {
    using type = PostOrderIterator<Node, T>;
};
