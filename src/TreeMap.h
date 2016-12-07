#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "bst.h"

namespace aisdi {
template<typename KeyType, typename ValueType>
class HashMap;

template<typename KeyType, typename ValueType>
class TreeMap {
    friend class HashMap<KeyType, ValueType>;
    BST<KeyType, ValueType> tree;
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class ConstIterator;

    class Iterator;

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    TreeMap() { }

    TreeMap(std::initializer_list<value_type> list) {
        for (auto&& pair : list) {
            tree.insert(std::move(pair.first), std::move(pair.second));
        }
    }

    TreeMap(const TreeMap& other)
        : tree(other.tree)
    { }

    TreeMap(TreeMap&& other)
        : tree(std::move(other.tree))
    { }

    TreeMap& operator=(const TreeMap& other) {
        tree = other.tree;
        return *this;
    }

    TreeMap& operator=(TreeMap&& other) {
        tree = std::move(other.tree);
        return *this;
    }

    bool isEmpty() const {
        return tree.isEmpty();
    }

    template <typename Kk>
    mapped_type& operator[](Kk&& key) {
        return tree.insert(std::forward<Kk>(key))->value.second;
    }

    const mapped_type& valueOf(const key_type& key) const {
        auto node = tree.findNodeWithKey(key);
        if (!node) throw std::out_of_range("item doesn't exist");
        return node->value.second;
    }

    mapped_type& valueOf(const key_type& key) {
        auto node = tree.findNodeWithKey(key);
        if (!node) throw std::out_of_range("item doesn't exist");
        return node->value.second;
    }

    const_iterator find(const key_type& key) const {
        auto node = tree.findNodeWithKey(key);
        if (!node) return ConstIterator(cend());
        return ConstIterator(&tree, node, false);
    }

    iterator find(const key_type& key) {
        auto node = tree.findNodeWithKey(key);
        if (!node) return Iterator(end());
        return Iterator(&tree, node, false);
    }

    void remove(const key_type& key) {
        if (!tree.deleteKey(key)) throw std::out_of_range("delete unexistent item");
    }

    void remove(const const_iterator& it) {
        if (it.isEnd || !tree.deleteKey(it.node->value.first)) throw std::out_of_range("delete unexistent item");
    }

    size_type getSize() const {
        return tree.getSize();
    }

    bool operator==(const TreeMap& other) const {
        return tree == other.tree;
    }

    bool operator!=(const TreeMap& other) const {
        return !(*this == other);
    }

    void clear() {
        tree.clear();
    }

    iterator begin() {
        auto node = tree.getFirstNode();
        return Iterator(&tree, node, !static_cast<bool>(node));
    }

    iterator end() {
        return Iterator(&tree, tree.getLastNode(), true);
    }

    const_iterator cbegin() const {
        auto node = tree.getFirstNode();
        return ConstIterator(&tree, node, !static_cast<bool>(node));
    }

    const_iterator cend() const {
        return ConstIterator(&tree, tree.getLastNode(), true);
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }
};

template<typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator {
    friend class TreeMap;
    const BST<KeyType, ValueType> *tree;
    typename BST<KeyType, ValueType>::BSTNode *node;
    bool isEnd;
public:
    using reference = typename TreeMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename TreeMap::value_type;
    using pointer = const typename TreeMap::value_type*;

    explicit ConstIterator(const BST<KeyType, ValueType> *t,
                           typename BST<KeyType, ValueType>::BSTNode *n,
                           bool end)
        : tree(t), node(n), isEnd(end)
    { }

    ConstIterator(const ConstIterator& other)
        : tree(other.tree), node(other.node), isEnd(other.isEnd)
    { }

    ConstIterator& operator=(ConstIterator&& other) {
        tree = other.tree;
        node = other.node;
        isEnd = other.isEnd;
        return *this;
    }

    ConstIterator& operator++() {
        try {
            node = tree->getNextNode(node);
        } catch (std::out_of_range& e) {
            if (isEnd) throw;
            else
                isEnd = true;
        }
        return *this;
    }

    ConstIterator operator++(int) {
        ConstIterator t(*this);
        operator ++();
        return t;
    }

    ConstIterator& operator--() {
        if(isEnd && node) isEnd = false;
        else node = tree->getPreviousNode(node);
        return *this;
    }

    ConstIterator operator--(int) {
        ConstIterator t(*this);
        operator --();
        return t;
    }

    reference operator*() const {
        if(isEnd) throw std::out_of_range("dereference of end()");
        return node->value;
    }

    pointer operator->() const {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const {
        return node == other.node && isEnd == other.isEnd;
    }

    bool operator!=(const ConstIterator& other) const {
        return !(*this == other);
    }
};

template<typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator {
public:
    using reference = typename TreeMap::reference;
    using pointer = typename TreeMap::value_type*;

    explicit Iterator(const BST<KeyType, ValueType> *t,
                      typename BST<KeyType, ValueType>::BSTNode *n,
                      bool end)
        : ConstIterator(t, n, end)
    { }

    Iterator(const ConstIterator& other)
            : ConstIterator(other) { }

    Iterator& operator++() {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int) {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--() {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int) {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    pointer operator->() const {
        return &this->operator*();
    }

    reference operator*() const {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }


};

}

#endif /* AISDI_MAPS_MAP_H */
