#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using namespace std;

class Node {
    public:
        int value;
        string color;
        Node* parent;
        Node* left;
        Node* right;

        Node(int val) : value(val), color("red"), left(nullptr), right(nullptr), parent(nullptr) {} 
};

class RBTree {
    public:
        Node* root;

        RBTree() : root(nullptr) {}
         
         //Rotate direction functions
         void rotateLeft(Node* x) {
          Node* y = x->right;
          x->right = y->left;
          
          if (y->left != nullptr) {
            y->left->parent = x;
          }
          y->parent = x->parent;
          if (x->parent == nullptr) {
            root = y;
          } else if (x == x->parent->left) {
             x->parent->left = y;
          } else {
             x->parent->right = y;
          }

          y->left = x;
          x->parent = y;
        }

        void rotateRight(Node* x) {
             Node* y = x->left;
             x->left = y->right;
          
            if (y->right!= nullptr) {
            y->right->parent = x;
                }
             y->parent = x->parent;
            if (x->parent == nullptr) {
                root = y;
             } else if (x == x->parent->right) {
             x->parent->right = y;
             } else {
             x->parent->left = y;
             }

          y->right = x;
          x->parent = y;
        }

        Node* minimum(Node* n) {
            while (n->left != nullptr) {
                n = n->left;
            }
           return n;
        }

        void fixInsert(Node* z) {
             while (z->parent != nullptr && z->parent->color == "red") {
            if (z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y != nullptr && y->color == "red") {
                    z->parent->color = "black";
                    y->color = "black";
                    z->parent->parent->color = "red";
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = "black";
                    z->parent->parent->color = "red";
                    rotateRight(z->parent->parent);
                }
            } else {
                Node* y = z->parent->parent->left;
                if (y != nullptr && y->color == "red") {
                    z->parent->color = "black";
                    y->color = "black";
                    z->parent->parent->color = "red";
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = "black";
                    z->parent->parent->color = "red";
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = "black";
        }

        void fixRemove(Node* x) {
            while (x != root && (x == nullptr || x->color == "black")) {
                if (x && x->parent && x->parent->left) {
                    Node* w = x->parent->right;

                    if (w && w->color == "red") {
                        w->color = "black";
                        x->parent->color = "red";
                        rotateLeft(x->parent);
                        w = x->parent->right;
                    } 

                    if ((w->left == nullptr || w->left->color == "black") && (w->right == nullptr || w->right->color == "black")) {
                         w->color = "red";
                         x = x->parent;
                    } else {
                        if (w->right == nullptr || w->right->color == "black") {
                        if (w->left)  { 
                            w->left->color = "black";
                             }
                        w->color = "red";
                        rotateRight(w);
                        w = x->parent->right;
                         }
                        w->color = x->parent->color;
                        x->parent->color = "black";
                        if (w->right) { 
                            w->right->color = "black";
                           }
                        rotateLeft(x->parent);
                       x = root;
                    }
                } else {
                    Node* w = x->parent->left;
                    if (w != nullptr && w->color == "red") {
                    w->color = "black";
                    x->parent->color = "red";
                    rotateRight(x->parent);
                    w = x->parent->left;
                     }
                    if ((w->right == nullptr || w->right->color == "black") && (w->left == nullptr|| w->left->color == "black")) {
                    w->color = "red";
                    x = x->parent;
                     } else {
                      if (w->left == nullptr || w->left->color == "black") {
                        if (w->right) {
                            w->right->color = "black";
                        } 
                        w->color = "red";
                        rotateLeft(w);
                        w = x->parent->left;
                       }
                        w->color = x->parent->color;
                        x->parent->color = "black";
                       if (w->left != nullptr) {
                         w->left->color = "black";
                       }
                       rotateRight(x->parent);
                       x = root;
                     }
                }
            }
            if (x != nullptr) {
                x->color = "black";
            }
        }
    

        void insert(int data) {
            Node* z = new Node(data);
            Node* y = nullptr;
            Node* x = root;

            while (x != nullptr) {
            y = x;
            if (z->value < x->value) { 
                x = x->left;
            }
            else { 
                x = x->right;
                 }
             }

             z->parent = y;
            if (y == nullptr) { 
            root = z;
             }
            else if (z->value < y->value) {
             y->left = z;
             }
            else { 
            y->right = z;
             }

             z->color = "red";
             fixInsert(z);
        }


        Node* remove(int value) {
            Node* z = search(value);
            
            if (z == nullptr) {
                return root;
            }

            Node* y = z;
            Node* x;

            if (z->left == nullptr || z->right == nullptr) {
                y = z->left;

                while (y->right != nullptr) {
                    y = y->right;
                }
            } 

            z->value = y->value;

            if (y->left != nullptr) {
                x = y->left;
            } else {
                x = y->right;
            }
           
           if (x != nullptr) {
               x->parent = y->parent;
           }

           if (y->parent == nullptr) {
               root = x;
           } else if (y == y->parent->left) {
            y->parent->left = x;
           } else {
            y->parent->right = x;
           }

           if (y != z) {
            z->value = y->value;
           }

           if (y->color == "black") {
             fixRemove(x);
           }

           delete y;
           return root;
        }

        Node* search(int value) {
            Node* current = root;
            while (current != nullptr) {
                if (value == current->value) {
                    return current;
                }

                if (value < current->value) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
           return nullptr;
        }

        void printRBTree(Node* root) {
            if (root == nullptr) {
                cout << "Unfortunately, the tree is empty." << endl;
            } else {
            cout << "How would you like to print the red-black tree? " << endl;
            cout << "A. In order" << endl;
            cout << "B. level order" << endl;
            cout << "C. pre-order" << endl;
            cout << "D. post-order" << endl;

            string choice;
            cin >> choice;

            if (choice == "A") {
                inOrderTraversal(root);
            } else if (choice == "B") {
                levelOrderTraversal(root);
            } else if (choice == "C") {
                preOrderTraversal(root);
            } else if (choice == "D") {
                postOrderTraversal(root);
            }
          }
        }

        void inOrderTraversal(Node* root) {
            if (root == nullptr) {
                return;
            } 

             inOrderTraversal(root->left);
             cout << "Node: " << root->value << ", Color: " << (root->color) << endl;
             inOrderTraversal(root->right);
        }

        void levelOrderTraversal(Node* root) {
            if (root == nullptr) {
                return;
            } 

             queue<Node*> q;
             q.push(root);
             while (!q.empty()) {
             Node* current = q.front();
             q.pop();
             cout << "Node: " << current->value << ", Color: " << (current->color) << endl;
             if (current->left != nullptr) q.push(current->left);
             if (current->right != nullptr) q.push(current->right);
             }
        }

        void preOrderTraversal(Node* root) {
            if (root == nullptr) {
                return;
            } 

            cout << "Node: " << root->value << ", Color: " << (root->color) << endl;
            preOrderTraversal(root->left);
            preOrderTraversal(root->right);
        }

        void postOrderTraversal(Node* root) {
            if (root == nullptr) {
                return;
            } 

            postOrderTraversal(root->left);
            postOrderTraversal(root->right);
             cout << "Node: " << root->value << ", Color: " << (root->color) << endl;
        }

};

int main() {
    RBTree* cutie = new RBTree();
    cout << "Insert values into the red-black tree: " << endl;
    int inputs;
    cin >> inputs;
    while (inputs != -1) {
        cutie->insert(inputs);
        cout << "You are still inserting...." << endl;
        cin >> inputs;
    }

    cout << "Congratulations! You've just made a red-black tree! Would you like to search for a number?" << endl;
    string response;
    cin >> response;

    if (response == "yes") {
        cout << "Enter the value you would like to look for" << endl;
        cin >> inputs;
        Node* result = cutie->search(inputs);
        if (result != nullptr) {
            cout << "Number found!" << endl;
        } else {
            cout << "Number not found." << endl;
        }
    }

    cout << "Would you like to print the tree?" << endl;
    cin >> response;
    if (response == "yes") {
        cutie->printRBTree(cutie->root);
    }
    return 0;
}