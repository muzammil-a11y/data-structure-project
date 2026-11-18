#include "AVLTree.h"


CourseAVLTree::CourseAVLTree() {
    root = nullptr;
}

CourseAVLTree::~CourseAVLTree() {
    clear();
}


int CourseAVLTree::maxVal(int a, int b) {
    return (a > b) ? a : b;
}

int CourseAVLTree::getHeight(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->getHeight();
}

int CourseAVLTree::getBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->getLeft()) - getHeight(node->getRight());
}


AVLNode* CourseAVLTree::rotateRight(AVLNode* y) {
   AVLNode* temp=y;
   y=y->getLeft();
   temp->setLeft(y->getRight());
   y->setRight(temp);
   temp->setHeight(maxVal(getHeight(temp->getLeft()),getHeight(temp->getRight()))+1);
   y->setHeight(maxVal(getHeight(y->getLeft()),getHeight(y->getRight()))+1);
   return y;
}

AVLNode* CourseAVLTree::rotateLeft(AVLNode* x){
    AVLNode* temp=x;
    x=x->getRight();
    temp->setRight(x->getLeft());
    x->setLeft(temp);
    temp->setHeight(maxVal(getHeight(temp->getLeft()),getHeight(temp->getRight()))+1);
    x->setHeight(maxVal(getHeight(x->getLeft()),getHeight(x->getRight()))+1);
    return x;
}

AVLNode* CourseAVLTree::rotateLeftright(AVLNode* x){
    x->setLeft(rotateLeft(x->getLeft()));
    return rotateRight(x);
}
AVLNode* CourseAVLTree::rotateRightleft(AVLNode* x){
    x->setRight(rotateRight(x->getRight()));
    return rotateLeft(x);
}

AVLNode* CourseAVLTree::insertNode(AVLNode* node, CourseOffering* offering) {
    if (node == nullptr) {
        return new AVLNode(offering);
    }

    int newid = offering->getId();
    int currentid = node->getData()->getId();


    if (newid < currentid) {
        node->setLeft(insertNode(node->getLeft(), offering));
    }
    else if (newid > currentid) {
        node->setRight(insertNode(node->getRight(), offering));
    }
    else {
        return node;
    }


    node->setHeight(maxVal(getHeight(node->getLeft()), getHeight(node->getRight())) + 1);


    int balancef = getBalance(node);

    if (balancef > 1 && getBalance(node->getLeft()) >= 0) {
        return rotateRight(node);
    }
    else if (balancef > 1 && getBalance(node->getLeft()) < 0) {
        return rotateLeftright(node);
    }
    else if (balancef < -1 && getBalance(node->getRight()) <= 0) {
        return rotateLeft(node);
    }
    else if (balancef < -1 && getBalance(node->getRight()) > 0) {
        return rotateRightleft(node);
    }

    return node;
}


CourseOffering* CourseAVLTree::searchNode(AVLNode* node, int offeringId) {
    if (node == nullptr) return nullptr;

    int nodeKey = node->getData()->getId();
    if (offeringId == nodeKey) {
        return node->getData();
    } else if (offeringId < nodeKey) {
        return searchNode(node->getLeft(), offeringId);
    } else {
        return searchNode(node->getRight(), offeringId);
    }
}


void CourseAVLTree::destroyTree(AVLNode* node) {
    if (node == nullptr) return;
    destroyTree(node->getLeft());
    destroyTree(node->getRight());
    delete node;
}

void CourseAVLTree::insert(CourseOffering* offering) {
    root = insertNode(root, offering);
}

CourseOffering* CourseAVLTree::search(int offeringId) {
    return searchNode(root, offeringId);
}

void CourseAVLTree::clear() {
    destroyTree(root);
    root = nullptr;
}
