#ifndef BST_H
#define BST_H

#include <iostream>

template <typename KeyType, typename T>
class BST {
    struct BSTNode;
public:
    BST() = default;
    BST(const BST<KeyType, T>& other);
    BST(BST<KeyType, T>&& other);
    ~BST();
    BST<KeyType, T>& operator=(const BST<KeyType, T>& other);
    BST<KeyType, T>& operator=(BST<KeyType, T>&& other);
    bool operator==(const BST<KeyType, T>& other) const;

        template <typename Kk, typename Tt>
    BSTNode* insert(Kk&& key, Tt&& item);
    bool deleteKey(const KeyType& key);
    BSTNode* getFirstNode() const;
    BSTNode* getLastNode() const;
    bool isEmpty() const;
    BSTNode* findNodeWithKey(const KeyType& key) const;

    void print() const;

private:
    BSTNode *root = nullptr;
    void print(BSTNode *node) const;

    bool operatorEqualsHelper(BSTNode *currentFirst, BSTNode *currentSecond) const;
    void treeCopyingHelper(const BSTNode* otherTreeNode);
        template <typename Kk, typename Tt>
    BSTNode* insertHelper(BSTNode *current, Kk&& key, Tt&& item);
    bool deleteKeyHelper(BSTNode *current, const KeyType& key);
};

template <typename KeyType, typename T>
struct BST<KeyType, T>::BSTNode : public std::pair<const KeyType, T> {
    std::pair<const KeyType, T> value;
    BSTNode *left, *right, *parent;

    template <typename Kk, typename Tt>
    BSTNode(BSTNode *p, Kk&& k, Tt&& item)
        : value(std::forward<Kk>(k), std::forward<Tt>(item)),
               left(NULL), right(NULL), parent(p)
    { }

    ~BSTNode() {
        delete left;
        delete right;
    }
};

template <typename KeyType, typename T>
void BST<KeyType, T>::print() const {
    print(root);
}

template <typename KeyType, typename T>
void BST<KeyType, T>::print(BSTNode *node) const {
    if (!node) return;
    std::cout << node->value.first <<": "<< node->value.second << "\n";
    print(node->left);
    print(node->right);
}

template <typename KeyType, typename T>
BST<KeyType, T>::BST(const BST<KeyType, T>& other) {
    operator=(other);
}

template <typename KeyType, typename T>
BST<KeyType, T>::BST(BST<KeyType, T>&& other) {
    operator=(std::move(other));
}

template <typename KeyType, typename T>
BST<KeyType, T>::~BST() {
    delete root;
}

template <typename KeyType, typename T>
bool BST<KeyType, T>::operator==(const BST<KeyType, T>& other) const {
    return operatorEqualsHelper(root, other.root);
}

template <typename KeyType, typename T>
bool BST<KeyType, T>::operatorEqualsHelper(BSTNode *currentFirst, BSTNode *currentSecond) const {
    if (!currentFirst || !currentSecond) {
        if (!currentFirst && !currentSecond) return true;
        return false;
    }
    return (currentFirst->value == currentSecond->value) &&
            operatorEqualsHelper(currentFirst->left, currentSecond->left) &&
            operatorEqualsHelper(currentFirst->right, currentSecond->right);
}

template <typename KeyType, typename T>
BST<KeyType, T>& BST<KeyType, T>::operator=(const BST<KeyType, T>& other) {
    delete root;
    treeCopyingHelper(other.root);
}

template <typename KeyType, typename T>
void BST<KeyType, T>::treeCopyingHelper(const BSTNode *otherTreeNode) {
    if (!otherTreeNode) return;
    insert(otherTreeNode->value.first, otherTreeNode->value.second);
    treeCopyingHelper(otherTreeNode->left);
    treeCopyingHelper(otherTreeNode->right);
}

template <typename KeyType, typename T>
BST<KeyType, T>& BST<KeyType, T>::operator=(BST<KeyType, T>&& other) {
    delete root;
    root = other.root;
    other.root = nullptr;
}

template <typename KeyType, typename T>
template <typename Kk, typename Tt>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::insert(Kk&& key, Tt&& item) {
    if (!root) {
        root = new BSTNode(NULL, std::forward<Kk>(key), std::forward<Tt>(item));
        return root;
    }
    return insertHelper(root, std::forward<Kk>(key), std::forward<Tt>(item));
}

template <typename KeyType, typename T>
template <typename Kk, typename Tt>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::insertHelper(BSTNode *current, Kk&& key, Tt&& item) {
    BSTNode *node = current;
    // root exist here

    for (int k=0;;++k) {
        if (node->value.first == key) {
            return node;
        }
        if (node->value.first > key) {
            if (!node->left) {
                node->left = new BSTNode(node, std::forward<Kk>(key), std::forward<Tt>(item));
                return node->left;
            } else {
                node = node->left;
                continue;
            }
        } else {
            if (!node->right) {
                node->right = new BSTNode(node, std::forward<Kk>(key), std::forward<Tt>(item));
                return node->right;
            } else {
                node = node->right;
                continue;
            }
        }
    }
}

template <typename KeyType, typename T>
bool BST<KeyType, T>::deleteKey(const KeyType& key) {
    return deleteKeyHelper(root, key);
}

template <typename KeyType, typename T>
bool BST<KeyType, T>::deleteKeyHelper(BSTNode *current, const KeyType& key) {
    if (!current) return false;
    BSTNode *node = current;
    for(;node;) {
        if (node->value.first > key) node = node->left;
        else if (node->value.first < key) node = node->right;
        else break;
    }
    // if item wasn't found
    if (!node) return false;
    // here node is the one to delete

    if (!node->left || !node->right) {
        BSTNode* temp = node->left;
        if (node->right) temp = node->right;
        if (node->parent) {
            if (node->parent->left == node) {
                node->parent->left = temp;
            } else {
                node->parent->right = temp;
            }
        } else {
            root = temp;
        }
    } else {
        BSTNode* validSubs = node->right;
        while (validSubs->left) {
            validSubs = validSubs->left;
        }
        std::swap(node->value, validSubs->value);
        return deleteValueHelper(node->right, validSubs->value.first);
    }
    // here node is a leaf
    delete node;
    return true;
}

template <typename KeyType, typename T>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::getFirstNode() const {
    if (!root) return nullptr;
    BSTNode *node = root;
    while(node->left) node = node->left;
    BSTNode* findNodeWithKey(const KeyType& key);
    BSTNode* findNodeWithItem(const T& item);
    return node;
}

template <typename KeyType, typename T>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::getLastNode() const {
    if (!root) return nullptr;
    BSTNode *node = root;
    while(node->right) node = node->right;
    return node;
}

template <typename KeyType, typename T>
bool BST<KeyType, T>::isEmpty() const {
    return !root;
}


template <typename KeyType, typename T>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::findNodeWithKey(const KeyType& key) const {
    BSTNode *node = root;
    for(;node;) {
        if (node->value.first > key) node = node->left;
        else if (node->value.first < key) node = node->right;
        else break;
    }
    return node;
}


#endif // BST_H
