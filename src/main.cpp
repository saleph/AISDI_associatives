#define DEBUG 1
#include <cstddef>
#include <cstdlib>
#include <string>
#include "TreeMap.h"
#include "avltree.h"
#include "bst.h"

namespace {

    template<typename K, typename V>
    using Map = aisdi::TreeMap<K, V>;

//    void perfomTest() {
//        Map<int, std::string> map;
//        map[1] = "TODO";
//    }

} // namespace

int main() {
    BST<int, int> t;
    for (int i = 0; i<10; ++i) {
        t.insert(i+200,i);
        t.insert(i,i);
    }
    BST<int,int>::BSTNode *node = t.getFirstNode();
    try{
    while (true) {
        std::cout<< node->value.first <<": "<< node->value.second <<"\n";
        node = t.getNextNode(node);
    }
    } catch(std::out_of_range &e) {
        std::cout<<e.what()<< " end of tree\n";

    }

    std::cout << "###############a\n";
    t.print();
//    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
//    for (std::size_t i = 0; i < repeatCount; ++i)
//        perfomTest();
    return 0;
}
