#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include "bst.h"

namespace aisdi {

template<typename KeyType, typename ValueType>
class TreeMap {
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

    mapped_type& operator[](const key_type& key) {
        return tree.findNodeWithKey(key)->value.second;
    }

    const mapped_type& valueOf(const key_type& key) const {
        return tree.findNodeWithKey(key)->value.second;
    }

    mapped_type& valueOf(const key_type& key) {
        return tree.findNodeWithKey(key)->value.second;
    }

    const_iterator find(const key_type& key) const {
        (void) key;
        throw std::runtime_error("TODO");
    }

    iterator find(const key_type& key) {
        (void) key;
        throw std::runtime_error("TODO");
    }

    void remove(const key_type& key) {
        tree.deleteKey(key);
    }

    void remove(const const_iterator& it) {
        (void) it;
        throw std::runtime_error("TODO");
    }

    size_type getSize() const {
        throw std::runtime_error("TODO");
    }

    bool operator==(const TreeMap& other) const {
        return tree == other.tree;
    }

    bool operator!=(const TreeMap& other) const {
        return !(*this == other);
    }

    iterator begin() {
        throw std::runtime_error("TODO");
    }

    iterator end() {
        throw std::runtime_error("TODO");
    }

    const_iterator cbegin() const {
        throw std::runtime_error("TODO");
    }

    const_iterator cend() const {
        throw std::runtime_error("TODO");
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
public:
    using reference = typename TreeMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename TreeMap::value_type;
    using pointer = const typename TreeMap::value_type*;

    explicit ConstIterator() { }

    ConstIterator(const ConstIterator& other) {
        (void) other;
        throw std::runtime_error("TODO");
    }

    ConstIterator& operator++() {
        throw std::runtime_error("TODO");
    }

    ConstIterator operator++(int) {
        throw std::runtime_error("TODO");
    }

    ConstIterator& operator--() {
        throw std::runtime_error("TODO");
    }

    ConstIterator operator--(int) {
        throw std::runtime_error("TODO");
    }

    reference operator*() const {
        throw std::runtime_error("TODO");
    }

    pointer operator->() const {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const {
        (void) other;
        throw std::runtime_error("TODO");
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

    explicit Iterator() { }

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
