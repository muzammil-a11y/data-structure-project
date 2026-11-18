
#ifndef NODE_H
#define NODE_H

template<typename T>
class Node{
    T data;
    Node *next;
    Node *prev;

    public:
        Node(T d){
            data = d;
            next = prev = nullptr;
        }

        void setNext(Node *n){
            next = n;
        }

        void setPrev(Node *p){
            prev = p;
        }

        Node *getNext(){
            return next;
        }

        Node *getPrev(){
            return prev;
        }

        void setData(T d){
            data = d;
        }

        T getData(){
            return data;
        }
};

#endif
