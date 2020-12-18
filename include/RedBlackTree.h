#pragma once
#include "CustomConcepts.h"
#include "RedBlackTreeNode.h"
#include <compare>
#include <concepts>
#include <functional>
#include <memory>
#include <string>
template<CustomConcepts::copy_constructable_and_three_way_comparable T>
class RedBlackTree {
public:
    using ObjectReference = T &;
    using ConstObjectReference = const T &;
    using ObjectPointer = T *;
    using constObjectPointer = const T *;

private:
    std::shared_ptr<RedBlackTreeNode<T>> head;
    RedBlackTreeNode<T> *nullNode;
    RedBlackTreeNode<T> *current;
    RedBlackTreeNode<T> *parent;
    RedBlackTreeNode<T> *grand;
    RedBlackTreeNode<T> *great;
    template<std::copy_constructible U>
    U visitTreeByDFS(RedBlackTreeNode<T> *node, const std::function<U(RedBlackTreeNode<T> &)> visitWay) {
        if (node == nullptr) {
            return U{};
        }
        U result = visitWay(*node);
        result += visitTreeByDFS(node->left);
        result += visitTreeByDFS(node->right);
        return result;
    }
    void visitTreeByDFS(RedBlackTreeNode<T> *node, const std::function<void(RedBlackTreeNode<T> &)> visitWay) {
        if (node == nullptr) {
            return;
        }
        visitWay(*node);
        visitTreeByDFS(node->left);
        visitTreeByDFS(node->right);
    }
    void visitTreeByDFS(std::weak_ptr<const RedBlackTreeNode<T>> node, const std::function<void(const RedBlackTreeNode<T> &)> visitWay) const {
        return const_cast<RedBlackTree *>(this)->visitTree(node, visitWay);
    }
    std::shared_ptr<RedBlackTreeNode<T>> clone(const std::weak_ptr<RedBlackTreeNode<T>> node) const {
        if (node.expired()) {
            throw std::runtime_error("someone has already deleted this node!");
        }
        auto nodePtr = node.lock();
        if (nodePtr == nodePtr->left) {
            return nullptr;
        }
        return std::make_shared(nodePtr->element, clone(nodePtr->left), clone(nodePtr->right), nodePtr->color);
    }

public:
    explicit RedBlackTree(const T &element) : head(new RedBlackTreeNode{element}), nullNode(new RedBlackTreeNode<T>{}) {
        nullNode->left = nullNode->right = nullNode;
        head->left = head->right = nullNode;
    }
    //浅拷贝
    RedBlackTree(const RedBlackTree &) = default;
    RedBlackTree deepCopy(const RedBlackTree &another) {
        RedBlackTree instance;
    }
    //移动构造
    RedBlackTree(RedBlackTree &&another) noexcept : head(std::move(another.head)), nullNode(std::move(another.nullNode)),
                                                    current(std::move(another.current)), parent(std::move(another.parent)),
                                                    grand(std::move(another.grand), great(std::move(another.great))) {}
    virtual ~RedBlackTree() = default;

    RedBlackTree &operator=(const RedBlackTree &another) {
        if (this == another) {
            return *this;
        }
        head = another.head;
        parent = another.parent;
        grand = another.grand;
        great = another.great;
        return *this;
    }
    RedBlackTree &operator=(RedBlackTree &&another) noexcept {
        head = std::move(head);
        parent = another.parent;
        grand = another.grand;
        great = another.great;
        return *this;
    }
    const T &min() const {
        if (!head || head.get() == nullNode) {
            return;
        }
        RedBlackTreeNode<T> *cur = head.get();
        if (cur->left != nullNode) {
            cur = cur->left;
        }
        return cur->element;
    }
    const T &max() const {
        if (!head || head.get() == nullNode) {
            return;
        }
        RedBlackTreeNode<T> *cur = head.get();
        if (cur->right != nullNode) {
            cur = cur->right;
        }
        return cur->element;
    }
    [[nodiscard]] bool contains(ConstObjectReference x) const {
    }
    [[nodiscard]] bool empty() const {
        return head == nullNode;
    }
    [[nodiscard]] std::string printTreeByDFS() const {
        std::string result;
        visitTreeByDFS([&result](const RedBlackTreeNode<T> &node) {
            result += node.element + ' ';
        });
        return result;
    }
    void clear() {
        if (!head || head.get() == nullNode) {
            return;
        }
    }
    bool insert(const T &element) {
        current = parent = grand = head;
        nullNode->element = element;
        while (current->element != element) {
            great = grand;
            grand = parent;
            parent = current;
            current = element < current->element ? current->left : current->right;
            if (current->left->color == Color::RED && current->right->color == Color::RED) {
                handleReorient(element);
            }
            if (current != nullNode) {
                return false;
            }
            current = std::make_shared(element, nullNode, nullNode);
            if (element < parent->element) {
                parent->left = current;
            } else {
                parent->right = element;
            }
            handleReorient(element);
        }
    }
    bool remove(const T &) {
    }
    void reclaimMemory(std::shared_ptr<RedBlackTreeNode<T>> &node) {
    }
    template<std::copy_constructible U>
    U visitTreeByDFS(const std::function<U(RedBlackTreeNode<T> &)> visitWay) {
        return visitTreeByDFS(head, visitWay);
    }
    template<std::copy_constructible U>
    U visitTreeByDFS(const std::function<U(const RedBlackTreeNode<T> &)> visitWay) const {
        return const_cast<RedBlackTree *>(this)->visitTree(visitWay);
    }
    void visitTreeByDFS(const std::function<void(RedBlackTreeNode<T> &)> visitWay) {
        visitTreeByDFS(head, visitWay);
    }
    std::shared_ptr<RedBlackTreeNode<T>> clone() const {
        return clone(head);
    }
    std::shared_ptr<RedBlackTreeNode<T>> rotate(const T &element, RedBlackTreeNode<T> *parentPtr) {
        if (parentPtr == nullptr) {
            return nullptr;
        }
        if (element < parentPtr->element) {
            element < parentPtr->left->element ? rotateWithLeftChild(parentPtr->left) : rotateWithRightChild(parentPtr->left);
            return parentPtr->left;
        }
        element < parentPtr->right->element ? rotateWithLeftChild(parentPtr->right) : rotateWithRightChild(parentPtr->right);
        return parentPtr->right;
    }
    void handleReorient(const T &element) {
        current->color = Color::RED;
        current->left->color = Color::BLACK;
        current->right->color = Color::BLACK;
        if (parent->color == Color::RED) {
            grand->color = Color::RED;
            if (element->grand->element != element->parent->element) {
                parent = rotate(element, great);
                current->color = Color::BLACK;
            }
        }
        head->right->clor = Color::BLACK;
    }
    void rotateWithLeftChild(RedBlackTreeNode<T> *leftChild) {
    }
    void rotateWithRightChild(RedBlackTreeNode<T> *rightChild) {
    }
};