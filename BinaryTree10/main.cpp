#include <iostream>
#include <fstream>
#include "stack"

using namespace std;

template <class T>
class Tree {

private:
    class ListElement;

    class Node {
    public:
        T value;
        Node* left;
        Node* right;
        Node* parent;
        int height;
        int depth;
        int MSL;
        ListElement* listElement;

        Node(T value) : value(value), left(NULL), parent(NULL), right(NULL), height(NULL), depth(NULL), MSL(0) {}
    };

    class ListElement {
    public:
        Node* node;
        ListElement* left;
        ListElement* right;
        ListElement(Node* node) : node(node), left(NULL), right(NULL) {}
        void addAfter(ListElement* listElement) {
            this->left = listElement;
            this->right = listElement->right;
            if (listElement->right != NULL)
                listElement->right->left = this;
            listElement->right = this;
        }
        void addBefor(ListElement* listElement) {
            this->right = listElement;
            this->left = listElement->left;
            if (listElement->left != NULL)
                listElement->left->right = this;
            listElement->left = this;
        }
        void remove() {
            if (this->left != NULL)
                this->left->right = this->right;
            if(this->right != NULL)
                this->right->left = this->left;
            delete this;
        }

    };

    Node* root;
    Node* rootForDelete = NULL;
    int rootCount = 0;

    void wolkForNote() {
        postOrderWolk();
    }

    void postOrderWolk() {
        stack<Node*> stackFirst;
        stack<Node*> stackSecond;
        Node* node;

        stackFirst.push(root);

        while (!stackFirst.empty()) {
            node = stackFirst.top();
            stackFirst.pop();
            stackSecond.push(node);
            if (node->left != NULL)
                stackFirst.push(node->left);
            if (node->right != NULL)
                stackFirst.push(node->right);
        }

        while (!stackSecond.empty()) {
            note(stackSecond.top());
            stackSecond.pop();
        }
    }

    void note(Node* node) {
        if (node->left == NULL && node->right == NULL) {
            node->height = 0;
            node->MSL = 0;
        }
        else {
            if (node->left != NULL && node->right != NULL) {
                if (node->left->height > node->right->height)
                    node->height = node->left->height + 1;
                else {
                    if (node->left->height < node->right->height)
                        node->height = node->right->height + 1;
                    else //node->left->height == node->right->height
                        node->height = node->left->height + 1;
                }
                node->MSL = node->left->height + node->right->height + 2;
            }
            else {
                if (node->left != NULL) {
                    node->height = node->left->height + 1;
                    node->MSL = node->left->height + 1;
                }
                else {//node->right != NULL
                    node->height = node->right->height + 1;
                    node->MSL = node->right->height + 1;
                }
            }
        }
        if (rootForDelete == NULL) {
            rootForDelete = node;
            rootCount = 0;
        } else {
            if (rootForDelete->MSL < node->MSL) {
                rootForDelete = node;
                rootCount = 0;
            }
        }
        if (rootForDelete->MSL == node->MSL) {
            ++rootCount;
            if (rootForDelete->depth < node->depth)
                rootForDelete = node;
        }
    }

    void wolkForDelete(Node* node) {
        if(rootCount == 1) {
            ++rootCount;
            if(node->left != NULL)
                wolkForDelete(node->left);
            if(node->right != NULL)
                wolkForDelete(node->right);
        }
        else {
            if (node->left != NULL || node->right != NULL) {
                if (node->left != NULL && node->right != NULL) {
                    if (node->left->height > node->right->height)
                        wolkForDelete(node->left);
                    else {
                        if (node->left->height < node->right->height)
                            wolkForDelete(node->right);
                    }
                } else {
                    if (node->left != NULL)
                        wolkForDelete(node->left);
                    else //node->right != NULL
                        wolkForDelete(node->right);
                }
            }
        }
        rightDelete(node);
    }

public:

    Tree() : root(NULL) {}

    Tree(T value) : root(new Node(value)) {}

    void add(T value) {
        Node* parent;
        Node* node = root;

        if (root == NULL) {
            root = new Node(value);
            root->depth = 0;
            root->listElement = new ListElement(root);
        }
        else {
            while (node != NULL) {
                parent = node;
                if (value < node->value) {
                    node = node->left;
                    if (node == NULL) {
                        parent->left = new Node(value);
                        parent->left->depth = parent->depth + 1;
                        parent->left->listElement = new ListElement(parent->left);
                        parent->left->listElement->addBefor(parent->listElement);
                        parent->left->parent = parent;
                        break;
                    }
                }
                else {
                    if (node->value != value) {
                        node = node->right;
                        if (node == NULL) {
                            parent->right = new Node(value);
                            parent->right->depth = parent->depth + 1;
                            parent->right->listElement = new ListElement(parent->right);
                            parent->right->listElement->addAfter(parent->listElement);
                            parent->right->parent = parent;
                            break;
                        }
                    }
                    else
                        break;
                }
            }
        }
    }

    void straightLeft() {
        stack<Node*> stackFirst;
        Node* node;
        stackFirst.push(root);

        while (!stackFirst.empty()) {
            node = stackFirst.top();
            stackFirst.pop();
            printf("%d\n", node->value);
            if (node->right != NULL)
                stackFirst.push(node->right);
            if (node->left != NULL)
                stackFirst.push(node->left);
        }
    }

    void rightDelete(Node* node) {
        if (node == root) {
            delForRoot();
        }
        else {
            if (node->parent->left != NULL && node->parent->right != NULL) {
                if (node->parent->left->value == node->value)
                    delForLeft(node);
                else
                    delForRight(node);
            }
            else {
                if (node->parent->left != NULL)
                    delForLeft(node);
                else
                    delForRight(node);
            }
        }
    }

    void delForRoot() {
        Node* node = root;
        if (node->right == NULL && node->left == NULL) {
            node->listElement->remove();
            delete node;
        }
        else {
            if (node->right == NULL) {
                node->listElement->remove();
                root = node->left;
                delete node;
            }
            else {
                if (node->left == NULL) {
                    node->listElement->remove();
                    root = node->right;
                    delete node;
                }
                else {
                    Node* nodeForInsert = node->listElement->right->node;
                    node->listElement->remove();
                    node->value = nodeForInsert->value;
                    if (node->right->left != NULL) {
                        nodeForInsert->parent->left = nodeForInsert->right;
                        if (nodeForInsert->right != NULL)
                            nodeForInsert->right->parent = nodeForInsert->parent;
                    }
                    else {
                        node->right = nodeForInsert->right;
                        if (node->right != NULL)
                            node->right->parent = node;
                    }
                    delete nodeForInsert;
                }
            }
        }
    }

    void delForLeft(Node* node) {
        Node* parent = node->parent;
        if (node->right == NULL && node->left == NULL) {
            node->listElement->remove();
            parent->left = NULL;
            delete node;
        }
        else {
            if (node->right == NULL) {
                node->listElement->remove();
                parent->left = node->left;
                node->left->parent = parent;
                delete node;
            }
            else {
                if (node->left == NULL) {
                    node->listElement->remove();
                    parent->left = node->right;
                    node->right->parent = parent;
                    delete node;
                }
                else {
                    Node* nodeForInsert = node->listElement->right->node;
                    node->listElement->remove();
                    node->value = nodeForInsert->value;
                    if (node->right->left != NULL) {
                        nodeForInsert->parent->left = nodeForInsert->right;
                        if (nodeForInsert->right != NULL)
                            nodeForInsert->right->parent = nodeForInsert->parent;
                    }
                    else {
                        node->right = nodeForInsert->right;
                        if (node->right != NULL)
                            node->right->parent = node;
                    }
                    delete nodeForInsert;
                }
            }
        }
    }

    void delForRight(Node* node) {
        Node* parent = node->parent;
        if (node->right == NULL && node->left == NULL) {
            node->listElement->remove();
            parent->right = NULL;
            delete node;
        }
        else {
            if (node->right == NULL) {
                node->listElement->remove();
                parent->right = node->left;
                node->left->parent = parent;
                delete node;
            }
            else {
                if (node->left == NULL) {
                    node->listElement->remove();
                    parent->right = node->right;
                    node->right->parent = parent;
                    delete node;
                }
                else {
                    Node* nodeForInsert = node->listElement->right->node;
                    node->listElement->remove();
                    node->value = nodeForInsert->value;
                    if (node->right->left != NULL) {
                        nodeForInsert->parent->left = nodeForInsert->right;
                        if (nodeForInsert->right != NULL)
                            nodeForInsert->right->parent = nodeForInsert->parent;
                    }
                    else {
                        node->right = nodeForInsert->right;
                        if (node->right != NULL)
                            node->right->parent = node;
                    }
                    delete nodeForInsert;
                }
            }
        }
    }

    void makeTask() {
        wolkForNote();
        wolkForDelete(rootForDelete);
    }

};


int main() {

    Tree<int>* tree = new Tree<int>();
    int a;

    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);

    while (scanf("%d", &a) != EOF)
        tree->add(a);

    //task 10
    tree->makeTask();
    tree->straightLeft();

    return 0;
}