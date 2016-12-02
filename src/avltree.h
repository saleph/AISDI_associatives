#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>

template <class KeyType, typename T>
class AVLtree {
public:
    class AVLnode;
    AVLtree();
    ~AVLtree();
    template <typename Kk, typename Tt>
    typename AVLtree<KeyType, T>::AVLnode* insert(Kk&& key, Tt&& item);
    void deleteKey(const KeyType& key);
    void printBalance();

private:
    AVLnode *root;

    AVLnode* rotateLeft(AVLnode *a);
    AVLnode* rotateRight(AVLnode *a);
    AVLnode* rotateLeftThenRight(AVLnode *n);
    AVLnode* rotateRightThenLeft(AVLnode *n);
    void rebalance(AVLnode *n);
    int height(AVLnode *n);
    void setBalance(AVLnode *n);
    void printNodesBalance(AVLnode *n);
    void clearNode(AVLnode *n);
};

template <class KeyType, typename T>
struct AVLtree<KeyType, T>::AVLnode {
    const KeyType key;
    T item;
    int8_t balance;
    AVLnode *left, *right, *parent;

    template <typename Kk, typename Tt>
    AVLnode(Kk&& k, AVLnode *p, Tt&& item = Tt())
        : key(std::forward<Kk>(k)), balance(0), parent(p), left(NULL), right(NULL), item(std::forward<Tt>(item))
    { }

    ~AVLnode() {
        delete left;
        delete right;
    }
};

template <typename KeyType, typename T>
void AVLtree<KeyType, T>::rebalance(AVLnode *n) {
    setBalance(n);

    if (n->balance == -2) {
        if (height(n->left->left) >= height(n->left->right))
        n = rotateRight(n);
        else
        n = rotateLeftThenRight(n);
    }
    else if (n->balance == 2) {
        if (height(n->right->right) >= height(n->right->left))
        n = rotateLeft(n);
        else
        n = rotateRightThenLeft(n);
    }

    if (n->parent != NULL) {
        rebalance(n->parent);
    }
    else {
        root = n;
    }
}

template <typename KeyType, typename T>
typename AVLtree<KeyType, T>::AVLnode* AVLtree<KeyType, T>::rotateLeft(AVLnode *a) {
    AVLnode *b = a->right;
    b->parent = a->parent;
    a->right = b->left;

    if (a->right != NULL)
    a->right->parent = a;

    b->left = a;
    a->parent = b;

    if (b->parent != NULL) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);
    return b;
}

template <typename KeyType, typename T>
typename AVLtree<KeyType, T>::AVLnode* AVLtree<KeyType, T>::rotateRight(AVLnode *a) {
    AVLnode *b = a->left;
    b->parent = a->parent;
    a->left = b->right;

    if (a->left != NULL)
    a->left->parent = a;

    b->right = a;
    a->parent = b;

    if (b->parent != NULL) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);
    return b;
}

template <typename KeyType, typename T>
typename AVLtree<KeyType, T>::AVLnode* AVLtree<KeyType, T>::rotateLeftThenRight(AVLnode *n) {
    n->left = rotateLeft(n->left);
    return rotateRight(n);
}

template <typename KeyType, typename T>
typename AVLtree<KeyType, T>::AVLnode* AVLtree<KeyType, T>::rotateRightThenLeft(AVLnode *n) {
    n->right = rotateRight(n->right);
    return rotateLeft(n);
}

template <typename KeyType, typename T>
int AVLtree<KeyType, T>::height(AVLnode *n) {
    if (n == NULL)
    return -1;
    return 1 + std::max(height(n->left), height(n->right));
}

template <typename KeyType, typename T>
void AVLtree<KeyType, T>::setBalance(AVLnode *n) {
    n->balance = height(n->right) - height(n->left);
}

template <typename KeyType, typename T>
void AVLtree<KeyType, T>::printNodesBalance(AVLnode *n) {
    if (n != NULL) {
        printNodesBalance(n->left);
        std::cout << n->balance << " ";
        printNodesBalance(n->right);
    }
}

template <typename KeyType, typename T>
AVLtree<KeyType, T>::AVLtree(void) : root(NULL) {}

template <typename KeyType, typename T>
AVLtree<KeyType, T>::~AVLtree(void) {
    delete root;
}

template <typename KeyType, typename T>
template <typename Kk, typename Tt>
typename AVLtree<KeyType, T>::AVLnode* AVLtree<KeyType, T>::insert(Kk&& key, Tt&& item) {
    if (root == NULL) {
        root = new AVLnode(std::forward<Kk>(key), NULL, std::forward<Tt>(item));
        return root;
    }
    AVLnode
    *n = root,
    *parent;

    while (true) {
        if (n->key == key)
            return n;

        parent = n;

        bool goLeft = n->key > key;
        n = goLeft ? n->left : n->right;

        if (n == NULL) {
            if (goLeft) {
                n = parent->left = new AVLnode(std::forward<Kk>(key), parent, std::forward<Tt>(item));
            }
            else {
                n = parent->right = new AVLnode(std::forward<Kk>(key), parent, std::forward<Tt>(item));
            }

            rebalance(parent);
            return n;
        }
    }
}

template <typename KeyType, typename T>
void AVLtree<KeyType, T>::deleteKey(const KeyType& delKey) {
    if (root == NULL)
    return;

    AVLnode
    *n       = root,
    *parent  = root,
    *delNode = NULL,
    *child   = root;

    while (child != NULL) {
        parent = n;
        n = child;
        child = delKey >= n->key ? n->right : n->left;
        if (delKey == n->key)
        delNode = n;
    }

    if (delNode != NULL) {
        delNode->key = n->key;

        child = n->left != NULL ? n->left : n->right;

        if (root->key == delKey) {
            root = child;
        }
        else {
            if (parent->left == n) {
                parent->left = child;
            }
            else {
                parent->right = child;
            }

            rebalance(parent);
        }
    }
}

template <typename KeyType, typename T>
void AVLtree<KeyType, T>::printBalance() {
    printNodesBalance(root);
    std::cout << std::endl;
}



#endif // AVLTREE_H
