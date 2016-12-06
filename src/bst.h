#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>

template <typename KeyType, typename T>
class BST {
public:
    struct BSTNode;
private:
    BSTNode *root = nullptr;
    std::size_t size = 0;
public:
    struct BSTNode;

    BST() = default;
    BST(const BST<KeyType, T>& other);
    BST(BST<KeyType, T>&& other);
    ~BST();
    BST<KeyType, T>& operator=(const BST<KeyType, T>& other);
    BST<KeyType, T>& operator=(BST<KeyType, T>&& other);
    bool operator==(const BST<KeyType, T>& other) const;

        template <typename Kk, typename Tt>
    BSTNode* insert(Kk&& key, Tt&& item);
        template <typename Kk>
    BSTNode* insert(Kk&& key);
    bool deleteKey(const KeyType& key);
    BSTNode* getFirstNode() const;
    BSTNode* getLastNode() const;
    BSTNode* getNextNode(BSTNode *node) const;
    BSTNode* getPreviousNode(BSTNode *node) const;
    bool isEmpty() const;
    std::size_t getSize() const;
    BSTNode* findNodeWithKey(const KeyType& key) const;
    void deleteTree();

#ifdef DEBUG
    void print() const;
#endif

private:
#ifdef DEBUG
    void print(BSTNode *node) const;
#endif

    void treeCopyingHelper(const BSTNode* otherTreeNode);
        template <typename Kk, typename Tt>
    BSTNode* insertHelper(BSTNode *current, Kk&& key, Tt&& item);
    bool deleteKeyHelper(BSTNode *current, const KeyType& key);
    void deleteTreeHelper(BSTNode *current);
};

template <typename KeyType, typename T>
struct BST<KeyType, T>::BSTNode {
    std::pair<const KeyType, T> value;

    union {
        struct {
            BSTNode *left, *right, *parent;
        };
        BSTNode *nodePointers[3];
    };

    template <typename Kk, typename Tt>
    BSTNode(BSTNode *p, Kk&& k, Tt&& item)
        : value(std::forward<Kk>(k), std::forward<Tt>(item)),
               left(NULL), right(NULL), parent(p)
    { }

    ~BSTNode() { }
};

#ifdef DEBUG
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
#endif



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
    deleteTree();
}

template <typename KeyType, typename T>
bool BST<KeyType, T>::operator==(const BST<KeyType, T>& other) const {
    BSTNode *end1 = getLastNode();
    BSTNode *end2 = other.getLastNode();
    for (BSTNode *node1 = getFirstNode(), *node2 = other.getFirstNode();
         node1 != end1 && node2 != end2;
         node1 = getNextNode(node2), node2 = other.getNextNode(node2)) {
        if (node1->value != node2->value)
            return false;
    }
    return true;
}

template <typename KeyType, typename T>
BST<KeyType, T>& BST<KeyType, T>::operator=(const BST<KeyType, T>& other) {
    if (root == other.root) {
        return *this;
    }
    deleteTree();
    treeCopyingHelper(other.root);
    return *this;
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
    if (root == other.root) {
        return *this;
    }
    deleteTree();
    root = other.root;
    size = other.size;
    other.root = nullptr;
    other.size = 0;
    return *this;
}

template <typename KeyType, typename T>
template <typename Kk, typename Tt>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::insert(Kk&& key, Tt&& item) {
    if (!root) {
        root = new BSTNode(NULL, std::forward<Kk>(key), std::forward<Tt>(item));
        ++size;
        return root;
    }
    return insertHelper(root, std::forward<Kk>(key), std::forward<Tt>(item));
}

template <typename KeyType, typename T>
template <typename Kk>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::insert(Kk&& key) {
    return insert(key, T());
}

template <typename KeyType, typename T>
template <typename Kk, typename Tt>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::insertHelper(BSTNode *current, Kk&& key, Tt&& item) {
    BSTNode *node = current;
    // root exist here

    for (;;) {
        if (node->value.first == key) {
            return node;
        }
        if (node->value.first > key) {
            if (!node->left) {
                node->left = new BSTNode(node, key, item);
                ++size;
                return node->left;
            } else {
                node = node->left;
                continue;
            }
        } else {
            if (!node->right) {
                node->right = new BSTNode(node, std::forward<Kk>(key), std::forward<Tt>(item));
                ++size;
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

        // magical swap of 2 nodes without reassigning std::pair<CONST Key, T>
        BSTNode* temp[] = {node->nodePointers[0], node->nodePointers[1], node->nodePointers[2]};
        for(int i=0;i<3;++i) node->nodePointers[i] = validSubs->nodePointers[i];
        for(int i=0;i<3;++i) validSubs->nodePointers[i] = temp[i];

        return deleteKeyHelper(node->right, validSubs->value.first);
    }
    // here node is a leaf
    delete node;
    --size;
    return true;
}

template <typename KeyType, typename T>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::getFirstNode() const {
    if (!root) return nullptr;
    BSTNode *node = root;
    while(node->left) node = node->left;
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
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::getNextNode(BSTNode *node) const {
    if (!node) throw std::out_of_range("end of tree");
    if (node->right) {
        node = node->right;
        while (node->left) node = node->left;
        return node;
    }
    if (node == root) {
        throw std::out_of_range("end of tree");
    }
    if (node == node->parent->left) {
        return node->parent;
    }
    if (node == node->parent->right) {
        while (node->parent && node == node->parent->right) node = node->parent;
        if (node == root) throw std::out_of_range("end of tree");
        else return node->parent;
    }
    return nullptr;
}
 //TODO:
template <typename KeyType, typename T>
typename BST<KeyType, T>::BSTNode* BST<KeyType, T>::getPreviousNode(BSTNode *node) const {
    if (!node) throw std::out_of_range("end of tree");
    if (node->left) {
        node = node->left;
        while (node->right) node = node->right;
        return node;
    }
    if (node == root) {
        throw std::out_of_range("end of tree");
    }
    if (node == node->parent->right) {
        return node->parent;
    }
    if (node == node->parent->left) {
        while (node->parent && node == node->parent->left) node = node->parent;
        if (node == root) throw std::out_of_range("end of tree");
        else return node->parent;
    }
    return nullptr;
}

template <typename KeyType, typename T>
bool BST<KeyType, T>::isEmpty() const {
    return !size;
}

template <typename KeyType, typename T>
std::size_t BST<KeyType, T>::getSize() const {
    return size;
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

template <typename KeyType, typename T>
void BST<KeyType, T>::deleteTree() {
    deleteTreeHelper(root);
    size = 0;
    root = nullptr;
}

template <typename KeyType, typename T>
void BST<KeyType, T>::deleteTreeHelper(BSTNode *current) {
    if (!current) return;
    deleteTreeHelper(current->left);
    deleteTreeHelper(current->right);
    current->left = current->right = current->parent = nullptr;
    delete current;
}



#endif // BST_H
