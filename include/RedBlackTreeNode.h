#pragma once
#include "CustomConcepts.h"
#include "CustomEnum.h"
#include <compare>
enum class Color;
template<CustomConcepts::copy_constructable_and_three_way_comparable T>
struct RedBlackTreeNode {
    using ObjectPointer = RedBlackTreeNode *;
    T element;
    RedBlackTreeNode *left;
    RedBlackTreeNode *right;
    Color color;
    RedBlackTreeNode(const T &element_, RedBlackTreeNode *left_ = nullptr, RedBlackTreeNode *right_ = nullptr, const Color &color_ = Color::BLACK) : element{element_}, left{left_}, right{right_}, color{color_} {}
    RedBlackTreeNode(T &&element_=T{}, RedBlackTreeNode *left_ = nullptr, RedBlackTreeNode *right_ = nullptr, const Color &color_ = Color::BLACK) : element(std::move(element_)), left(left_), right(right_), color(color_) {}
};