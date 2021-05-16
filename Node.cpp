//
// Created by Tomek on 16.05.2021.
//

#include "Node.h"

Node::Node(int MAX) {
    this->link = new Node*[MAX+1];
    this->val = new int[MAX+1];
}

Node::Node(int MAX, int val, Node *root, Node *child) {
    this->link = new Node*[MAX+1];
    this->val = new int[MAX+1];
    this->val[1] = val;
    this->count = 1;
    this->link[0] = root;
    this->link[1] = child;

}
