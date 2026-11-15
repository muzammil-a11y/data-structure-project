
#ifndef AVLTREE_H
#define AVLTREE_H

#include "Models.h"

class AVLNode {
private:
    CourseOffering* data;
    AVLNode* left;
    AVLNode* right;
    int height;

public:
    AVLNode(CourseOffering* offering)
        : data(offering), left(nullptr), right(nullptr), height(1) {}

    CourseOffering* getData() const { return data; }
    AVLNode* getLeft() const { return left; }
    AVLNode* getRight() const { return right; }
    int getHeight() const { return height; }

    void setLeft(AVLNode* l) { left = l; }
    void setRight(AVLNode* r) { right = r; }
    void setHeight(int h) { height = h; }
};

class CourseAVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    int maxVal(int a, int b);

    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* rotateRightleft(AVLNode* y);
    AVLNode* rotateLeftright(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, CourseOffering* offering);
    CourseOffering* searchNode(AVLNode* node, int offeringId);
    void destroyTree(AVLNode* node);

public:
    CourseAVLTree();
    ~CourseAVLTree();

    void insert(CourseOffering* offering);
    CourseOffering* search(int offeringId);
    void clear();
};

#endif