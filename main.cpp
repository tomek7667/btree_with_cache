#include <iostream>
#include <cstring>

using namespace std;
#define COMMAND_SIZE 1


#define MAX 3
#define MIN 2

struct Node {
    int val[MAX + 1], count;
    Node* link[MAX + 1];
};

// Create a node
Node* createNode(int val, Node* child, Node* root) {
    Node* newNode;
    newNode = (Node*)malloc(sizeof(Node));
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// Insert node
void insertNode(int val, int pos, Node* node,
                Node* child) {
    int j = node->count;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

// Split node
void splitNode(int val, int* pval, int pos, Node* node,
               Node* child, Node** newNode) {
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = (Node*)malloc(sizeof(Node));
    j = median + 1;
    while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;

    if (pos <= MIN) {
        insertNode(val, pos, node, child);
    } else {
        insertNode(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// Set the value
int setValue(int val, int* pval,
             Node* node, Node** child) {
    int pos;
    if (!node) {
        *pval = val;
        *child = NULL;
        return 1;
    }

    if (val < node->val[1]) {
        pos = 0;
    } else {
        for (pos = node->count; (val < node->val[pos] && pos > 1); pos--);
        if (val == node->val[pos]) {
            cout << "no duplicates\n";
            return 0;
        }
    }
    if (setValue(val, pval, node->link[pos], child)) {
        if (node->count < MAX) {
            insertNode(*pval, pos, node, *child);
        } else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Copy the successor
void copySuccessor(Node* node_del, int pos) {
    Node* dummy;
    dummy = node_del->link[pos];

    for (; dummy->link[0] != NULL;)
        dummy = dummy->link[0];
    node_del->val[pos] = dummy->val[1];
}

// Remove the value
void removeVal(Node* node_del, int pos) {
    int i = pos + 1;
    while (i <= node_del->count) {
        node_del->val[i - 1] = node_del->val[i];
        node_del->link[i - 1] = node_del->link[i];
        i++;
    }
    node_del->count--;
}


// Do right shift
void rightShift(Node* node_del, int pos) {
    Node* x = node_del->link[pos];
    int j = x->count;

    while (j > 0) {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
    }
    x->val[1] = node_del->val[pos];
    x->link[1] = x->link[0];
    x->count++;

    x = node_del->link[pos - 1];
    node_del->val[pos] = x->val[x->count];
    node_del->link[pos] = x->link[x->count];
    x->count--;
    return;
}

// Do left shift
void leftShift(Node* node_del, int pos) {
    int j = 1;
    Node* x = node_del->link[pos - 1];

    x->count++;
    x->val[x->count] = node_del->val[pos];
    x->link[x->count] = node_del->link[pos]->link[0];

    x = node_del->link[pos];
    node_del->val[pos] = x->val[1];
    x->link[0] = x->link[1];
    x->count--;

    while (j <= x->count) {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
    }
    return;
}

// Insert the value
void insert(int val, Node** root) {
    int flag, i;
    Node* child;

    flag = setValue(val, &i, *root, &child);
    if (flag) *root = createNode(i, child, *root);
}


// Merge the nodes
void mergeNodes(Node* node_del, int pos) {
    int j = 1;
    Node* x1 = node_del->link[pos], * x2 = node_del->link[pos - 1];

    x2->count++;
    x2->val[x2->count] = node_del->val[pos];
    x2->link[x2->count] = node_del->link[0];

    while (j <= x1->count) {
        x2->count++;
        x2->val[x2->count] = x1->val[j];
        x2->link[x2->count] = x1->link[j];
        j++;
    }

    j = pos;
    while (j < node_del->count) {
        node_del->val[j] = node_del->val[j + 1];
        node_del->link[j] = node_del->link[j + 1];
        j++;
    }
    node_del->count--;
    free(x1);
}


// Adjust the node
void adjustNode(Node* node_del, int pos) {
    if (!pos) {
        if (node_del->link[1]->count > MIN) {
            leftShift(node_del, 1);
        } else {
            mergeNodes(node_del, 1);
        }
    } else {
        if (node_del->count != pos) {
            if (node_del->link[pos - 1]->count > MIN) {
                rightShift(node_del, pos);
            } else {
                if (node_del->link[pos + 1]->count > MIN) {
                    leftShift(node_del, pos + 1);
                } else {
                    mergeNodes(node_del, pos);
                }
            }
        } else {
            if (node_del->link[pos - 1]->count > MIN)
                rightShift(node_del, pos);
            else
                mergeNodes(node_del, pos);
        }
    }
}


int delValFromNode(int val, Node* node_del) {
    int pos, flag = 0;
    if (node_del) {
        if (val < node_del->val[1]) {
            pos = 0;
            flag = 0;
        } else {
            for (pos = node_del->count; (val < node_del->val[pos] && pos > 1); pos--)
                ;
            if (val == node_del->val[pos]) {
                flag = 1;
            } else {
                flag = 0;
            }
        }
        if (flag) {
            if (node_del->link[pos - 1]) {
                copySuccessor(node_del, pos);
                flag = delValFromNode(node_del->val[pos], node_del->link[pos]);
                if (flag == 0) {

                    // no value in tree
                }
            } else {
                removeVal(node_del, pos);
            }
        } else {
            flag = delValFromNode(val, node_del->link[pos]);
        }
        if (node_del->link[pos]) {
            if (node_del->link[pos]->count < MIN)
                adjustNode(node_del, pos);
        }
    }
    return flag;
}

// Delete operaiton
void deleteN(int item, Node** myNode) {
    struct Node* tmp;
    if (!delValFromNode(item, (*myNode))) {
        return;
    } else {
        if ((*myNode)->count == 0) {
            tmp = (*myNode);
            (*myNode) = (*myNode)->link[0];
            free(tmp);
        }
    }
    return;
}

// Search node
void search(int val, int* pos, Node* myNode, bool * found) {
    if (!myNode) {
        return;
    }
    if (val < myNode->val[1]) {
        *pos = 0;
    } else {
        for (*pos = myNode->count; (val < myNode->val[*pos] && *pos > 1); (*pos)--);
        if (val == myNode->val[*pos]) {
            cout << val << " +\n";
            (*found) = true;
            return;
        }
    }
    search(val, pos, myNode->link[*pos], found);
    return;
}

// Traverse then nodes
void revSwitch(Node* myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->count; i++) {
            revSwitch(myNode->link[i]);
            cout << myNode->val[i + 1] << " ";
        }
        revSwitch(myNode->link[i]);
    }
}

int main() {
    char choice[COMMAND_SIZE];
    int val, ch;
    Node* root = NULL;
    while (cin >> choice) {
        if (!strcmp(choice, "I")) {
            cin >> val;
            // create tree of odrer 'val'
        } else if (!strcmp(choice, "A")) {
            cin >> val;
            insert(val, &root);
        } else if (!strcmp(choice, "?")) {
            cin >> val;
            bool found = false;
            search(val, &ch, root, &found);
            if (!found) cout << val << " -\n";
        } else if (!strcmp(choice, "P")) {
            revSwitch(root);
            cout << endl;
        } else if (!strcmp(choice, "L")) {
            // TODO
        } else if (!strcmp(choice, "S")) {
            // TODO
        } else if (!strcmp(choice, "R")) {
            cin >> val;
            deleteN(val, &root);
        } else if (!strcmp(choice, "#")) {
            ;;
        } else if (!strcmp(choice, "X")) {
            exit(1);
        } else if (!strcmp(choice, "C")) {
            // TODO
        } else {
            cout << "Nie znam komendy : " << choice << endl;
        }
    }


    return 0;
}