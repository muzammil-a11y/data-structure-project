#ifndef LINKEDLIST_CPP // changed: guard added so g++ *.cpp works
#define LINKEDLIST_CPP

#include "LinkedList.h"

template<typename T>
LL<T>::LL(){
    head = tail = nullptr;
}

template<typename T>
void LL<T>::addFront(T d){
    Node<T> *new_node = new Node<T>(d);

    if(head == nullptr){
        head = tail = new_node;
        return;
    }

    head->setPrev(new_node);
    new_node->setNext(head);
    head = new_node;
}

template<typename T>
void LL<T>::deleteFront(){
    if(head == nullptr){
        throw out_of_range("Linked List is Empty");
    }

    if(head == tail){
        delete head;
        head = tail = nullptr;
        return;
    }

    Node<T> *temp = head;
    head = head->getNext();
    head->setPrev(nullptr);
    delete temp;
}

template<typename T>
void LL<T>::insertEnd(T d){
    Node<T> *new_node = new Node<T>(d);

    if(head == nullptr){
        head = tail = new_node;
        return;
    }

    tail->setNext(new_node);
    new_node->setPrev(tail);
    tail = new_node;
}

template<typename T>
void LL<T>::deleteEnd(){
    if(head == nullptr){
        throw out_of_range("Linked List is Empty");
    }

    if(head == tail){
        delete head;
        head = tail = nullptr;
        cout << "Deleted Successfully" << endl;
        return;
    }

    Node<T> *temp = tail;
    tail = temp->getPrev();
    tail->setNext(nullptr);
    delete temp;

    cout << "Deleted Successfully" << endl;
}

template<typename T>
void LL<T>::display(){
    Node<T> *temp = head;

    while(temp != nullptr){
        cout << temp->getData() << " ";
        temp = temp->getNext();
    }

    cout << endl;
}

template<typename T>
T LL<T>::getFront(){
    if(head == nullptr){
        throw out_of_range("Linked List is Empty");
    }

    return head->getData();
}

template<typename T>
bool LL<T>::isEmpty(){
    return head == nullptr;
}

template<typename T>
LL<T>::~LL(){
    Node<T> *temp;

    while(head != nullptr){
        temp = head;
        head = head->getNext();
        delete temp;
    }

    tail = nullptr;
}

#endif
