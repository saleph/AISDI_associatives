#define DEBUG 1
#include <cstddef>
#include <cstdlib>
#include <string>
#include "TreeMap.h"
#include "avltree.h"
#include "bst.h"
#include <random>
#include <chrono>
#include <thread>

namespace {

    template<typename K, typename V>
    using Map = aisdi::TreeMap<K, V>;

//    void perfomTest() {
//        Map<int, std::string> map;
//        map[1] = "TODO";
//    }

} // namespace

int main() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1,100000);
    BST<int, int> t;
    for (int i = 0; i<20; ++i) {
        t.insert(distribution(generator), i);
    }
    BST<int,int>::BSTNode *node = t.getLastNode();
    try{
    while (true) {
        std::cout<< node->value.first <<": "<< node->value.second <<"\n";
        node = t.getPreviousNode(node);
        std::this_thread::sleep_for(std::chrono::microseconds(1));
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
