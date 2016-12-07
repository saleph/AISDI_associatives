#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <list>
#include <functional>
#include "bst.h"

namespace aisdi {

    template<typename KeyType, typename ValueType>
    class HashMap {
        using node = typename BST<KeyType, ValueType>::BSTNode;
        const std::size_t LIST_SIZE = 7; // prime number closest to 1024
        std::vector<BST<KeyType, ValueType>> hashTable;
        mutable std::size_t size;
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

        HashMap()
            : hashTable(LIST_SIZE), size(0)
        { }

        HashMap(std::initializer_list<value_type> list)
            : HashMap()
        {
            for (auto&& pair : list)
                (*this)[std::move(pair.first)] = std::move(pair.second);
            updateSize();
        }

        HashMap(const HashMap& other)
            : hashTable(other.hashTable), size(other.size)
        { }

        HashMap(HashMap&& other)
            : hashTable(std::move(other.hashTable)), size(other.size)
        {
            other.clear();
        }

        HashMap& operator=(const HashMap& other) {
            if (this == &other) return *this;
            hashTable = other.hashTable;
            size = other.size;
            return *this;
        }

        HashMap& operator=(HashMap&& other) {
            if (this == &other) return *this;
            hashTable = std::move(other.hashTable);
            size = other.size;
            other.clear();
            return *this;
        }

        bool isEmpty() const {
            return !getSize();
        }

        template <typename Kk>
        mapped_type& operator[](Kk&& key) {
            std::size_t idx = std::hash<KeyType>()(key) % LIST_SIZE;
            auto& t = hashTable[idx].insert(std::forward<Kk>(key))->value.second;
            updateSize();
            return t;
        }

        const mapped_type& valueOf(const key_type& key) const {
            std::size_t idx = std::hash<KeyType>()(key) % LIST_SIZE;
            node *n = hashTable[idx].findNodeWithKey(key);
            if (!n) throw std::out_of_range("el doesn't exist");
            return n->value.second;
        }

        mapped_type& valueOf(const key_type& key) {
            std::size_t idx = std::hash<KeyType>()(key) % LIST_SIZE;
            node *n = hashTable[idx].findNodeWithKey(key);
            if (!n) throw std::out_of_range("el doesn't exist");
            return n->value.second;
        }

        const_iterator find(const key_type& key) const {
            std::size_t idx = std::hash<KeyType>()(key) % LIST_SIZE;
            node *n = hashTable[idx].findNodeWithKey(key);
            if (!n) return cend();
            auto it = hashTable.cbegin() + idx;
            return ConstIterator(*this,
                            it, // iterator for hashTable
                            &(*it), // pointer to the tree under it
                            n, // node with key
                            false); // isEnd
        }

        iterator find(const key_type& key) {
            std::size_t idx = std::hash<KeyType>()(key) % LIST_SIZE;
            node *n = hashTable[idx].findNodeWithKey(key);
            if (!n) return cend();
            auto it = hashTable.cbegin() + idx;
            return Iterator(*this,
                            it, // iterator for hashTable
                            &(*it), // pointer to the tree under it
                            n, // node with key
                            false); // isEnd
        }

        void remove(const key_type& key) {
            std::size_t idx = std::hash<KeyType>()(key) % LIST_SIZE;
            if (!hashTable[idx].deleteKey(key))
                throw std::out_of_range("delete unexisting item");
            updateSize();
        }

        void remove(const const_iterator& it) {
            if (it == cend())
                throw std::out_of_range("delete unexisting item");
            remove(it->first);
        }

        size_type getSize() const {
            return size;
        }

        void updateSize() {
            std::size_t s = 0;
            for(auto&& tree : hashTable) s += tree.getSize();
            size = s;
        }

        bool operator==(const HashMap& other) const {
            if (size != other.size) return false;
            for (auto i = 0u; i < LIST_SIZE; ++i) {
                if (hashTable[i] != other.hashTable[i])
                    return false;
            }
            return true;
        }

        bool operator!=(const HashMap& other) const {
            return !(*this == other);
        }

        void clear() {
            hashTable = std::vector<BST<KeyType, ValueType>>(LIST_SIZE);
            size = 0;
        }

        iterator begin() {
            auto it = hashTable.begin();
            for (; it != hashTable.end(); ++it)
                if (!it->isEmpty()) break;
            if (it != hashTable.end())
                return Iterator(*this,
                                it, // iterator for hashTable
                                &(*it), // pointer to the tree under it
                                it->getFirstNode(), // first node in tree
                                false); // isEnd
            return Iterator(*this,
                            it, // iterator for hashTable
                            &(*--it), // pointer to the tree under last position in hashTable
                            nullptr, // because any node in the tree doesn't exist
                            true); // isEnd
        }

        iterator end() {
            auto it = --hashTable.end();
            for (; it != hashTable.begin(); --it)
                if (!it->isEmpty()) break;
            //return Iterator(*this, it, it->end(), true);
            return Iterator(*this,
                            it, // iterator for hashTable
                            &(*it), // pointer to the tree under last position in hashTable
                            nullptr, // because any node in the tree doesn't exist
                            true); // isEnd
        }

        const_iterator cbegin() const {
            auto it = hashTable.begin();
            for (; it != hashTable.end(); ++it)
                if (!it->isEmpty()) break;
            if (it != hashTable.end())
                return ConstIterator(*this,
                                it, // iterator for hashTable
                                &(*it), // pointer to the tree under it
                                it->getFirstNode(), // first node in tree
                                false); // isEnd
            return ConstIterator(*this,
                            it, // iterator for hashTable
                            &(*--it), // pointer to the tree under last position in hashTable
                            nullptr, // because any node in the tree doesn't exist
                            true); // isEnd
        }

        const_iterator cend() const {
            auto it = --hashTable.end();
            for (; it != hashTable.begin(); --it)
                if (!it->isEmpty()) break;
            //return Iterator(*this, it, it->end(), true);
            return ConstIterator(*this,
                            it, // iterator for hashTable
                            &(*it), // pointer to the tree under last position in hashTable
                            nullptr, // because any node in the tree doesn't exist
                            true); // isEnd
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }


    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::ConstIterator {
        friend class HashMap<KeyType, ValueType>;

        using BSTNode = HashMap<KeyType, ValueType>::node;
        const HashMap<KeyType, ValueType>& map;
        typename std::vector<BST<KeyType, ValueType>>::const_iterator vecIt;
        const BST<KeyType, ValueType> *tree;
        BSTNode *node;
        bool end;
    public:
        using reference = typename HashMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename HashMap::value_type;
        using pointer = const typename HashMap::value_type*;

        explicit ConstIterator(const HashMap<KeyType, ValueType>& m,
                               typename std::vector<BST<KeyType, ValueType>>::const_iterator v,
                               const BST<KeyType, ValueType> *t,
                               BSTNode *n,
                               bool e)
            : map(m), vecIt(v), tree(t), node(n), end(e)
        { }

        ConstIterator(const ConstIterator& other)
            : map(other.map), vecIt(other.vecIt), tree(other.tree), node(other.node), end(other.end)
        { }

        ConstIterator& operator++() {
            if (end) throw std::out_of_range("");
            try {
                //if (!tree) throw std::out_of_range("");
                node = tree->getNextNode(node);
            } catch (std::out_of_range& e) {
                ++vecIt;
                while (vecIt != map.hashTable.end() && vecIt->isEmpty()) ++vecIt;
                if (vecIt == map.hashTable.end()) {
                    --vecIt;
                    end = true;
                }
                tree = &*vecIt;
                node = tree->getFirstNode();
            }
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator i(*this);
            operator++();
            return i;
        }

        ConstIterator& operator--() {
            try {
                if (end || !tree) throw std::out_of_range("");
                node = tree->getPreviousNode(node);
            } catch (std::out_of_range& e) {
                --vecIt;
                while (vecIt != map.hashTable.begin() && vecIt->isEmpty()) --vecIt;
                tree = &*vecIt;
                node = tree->getLastNode();
                if (vecIt == map.hashTable.begin() && vecIt->isEmpty()) throw;
            }
            end = false;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator i(*this);
            operator--();
            return i;
        }

        reference operator*() const {
            if (end) throw std::out_of_range("deref end");
            return node->value;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        bool operator==(const ConstIterator& other) const {
            return (end == other.end && node == other.node);
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }
    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename HashMap::reference;
        using pointer = typename HashMap::value_type*;

        explicit Iterator(const HashMap<KeyType, ValueType>& m,
                          typename std::vector<BST<KeyType, ValueType>>::const_iterator v,
                          const BST<KeyType, ValueType> *t,
                          node *n,
                          bool e)
            : ConstIterator(m, v, t, n, e)
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

#endif /* AISDI_MAPS_HASHMAP_H */
