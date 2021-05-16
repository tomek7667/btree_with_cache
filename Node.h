//
// Created by Tomek on 16.05.2021.
//

#ifndef BTREE_WITH_CACHE_NODE_H
#define BTREE_WITH_CACHE_NODE_H


class Node {
public:
    Node(int MAX, int val, Node* root, Node* child);
    int *val, count;
    Node** link;
};


#endif //BTREE_WITH_CACHE_NODE_H
