#include <cstddef>
#include <string>
#include <random>
#include <fstream>
#include <map>
#include <unordered_map>

#include "HashMap.h"
#include "Benchmark.h"
#include "TreeMap.h"


template<class Collection, int N>
void randomInsert(int n) {
    Collection map;
    std::mt19937 device;
    std::uniform_int_distribution<int> distribution(0, n);
    for (int i = 0; i < n; ++i) {
        map[distribution(device)] = i;
    }
}


int main(int argc, char** argv) {
    (void) argc;
    (void) argv;
    std::ofstream f("hash15693.txt");

    using Map = aisdi::HashMap<int, int>;
    using Tree = aisdi::TreeMap<int, int>;

    bm::BenchmarkSuite randomInsertSuite("Random Insert buckets: 15693");

    auto cases = {1000, 2000, 5000, 8000, 10000, 20000,
                  50000
                 , 80000,100000, 200000,
                  500000, 800000, 1000000,
//                  5000000, 8000000, 10000000
                 };

    randomInsertSuite.addBenchmark(bm::Benchmark("HashMap", randomInsert<Map, 52342>, cases))
                     .addBenchmark(bm::Benchmark("TreeMap", randomInsert<Tree, 52342>, cases));

    randomInsertSuite.run().exportCSV(f);
    f.close();
}
