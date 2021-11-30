#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include <vector>
#include "boost/sort/sort.hpp"


void FillArr(std::array<std::array<int, 6>, 10516> &database) {
    std::string line;
    std::ifstream file(R"(C:\Users\tosha\CLionProjects\SORTME\base.txt)");
    for(int i = 0; i < 10515;) {
        std::getline(file, line);
        std::vector<std::string> SplitVec;
        std::vector<std::string> SplitDate;
        boost::split( SplitVec, line, boost::algorithm::is_any_of(","),
                      boost::algorithm::token_compress_on);
        boost::split( SplitDate, SplitVec[0], boost::algorithm::is_any_of("-"),
                      boost::algorithm::token_compress_on);
        database[i][0] = std::stoi(SplitDate[0] + SplitDate[1] + SplitDate[2]);
        for (int j = 1; j < 5;) {

            database[i][j] = static_cast<int>(std::stof(SplitVec[j]) * 100);
            j++;
        }
        database[i][5] = std::stoi(SplitVec[6]);
        i++;
    }
}

int sign(int num) {
    if (num > 0)
        return -1;
    return 1;
}

int compare(const void* x1, const void * x2) {
        auto n1 = const_cast<int*>(reinterpret_cast<const int*>(x1)),
        n2 = const_cast<int*>(reinterpret_cast<const int*>(x2));
        std::array<int, 6> a = {};
        std::array<int, 6> b = {};
        for (int i = 0; i < 6; i++) {
            a[i] = *(n1 + i);
            b[i] = *(n2 + i);
        }
        if (a[2] != b[2])
            return sign(a[2] - b[2]);
        if (a[1] != b[1])
            return sign(a[1] - b[1]);
        if (a[4] != b[4])
            return sign(a[4] - b[4]);
        if (a[3] != b[3])
            return sign(a[3] - b[3]);
        return 0;
}

bool comp(const std::array<int, 6> a, const std::array<int, 6> b) {
    if (a[2] != b[2])
        return a[2] > b[2];
    if (a[1] != b[1])
        return a[1] > b[1];
    if (a[4] != b[4])
        return a[4] > b[4];
    if (a[3] != b[3])
        return a[3] > b[3];
    return false;
}

void block_vs_qsort(std::array<std::array<int, 6>, 10516> &database, int quantity) {
    auto a = database, b = database;
    std::cout << "sorting " << quantity << " elems" << std::endl;
    //qsort
    auto start_time = clock();
    qsort(a.data(), quantity - 1, 24, compare);
    std::cout << "qsort time: " << clock() - start_time << "ms" << std::endl;
    //block_indirect_sort
    start_time = clock();
    boost::sort::block_indirect_sort(b.begin(), b.begin() + quantity - 1, comp);
    std::cout << "block_indirect_sort time: " << clock() - start_time << "ms" << std::endl;
}

int main() {
    std::array<std::array<int, 6>, 10516> database = {};
    auto start_time = clock();
    FillArr(database);
    std::cout << "array ready in " << clock() - start_time << "ms" << std::endl;
    std::cout << "Input number of elems (up to 10516)" << std::endl;
    int q = 0;
    std::cin >> q;
    block_vs_qsort(database, q);

    //elems: 100  500  2000  5000  10516 (full)
    //qsort:  1    1    2     8     20
    //b_i_s:  0    1    2     5     12

    return 0;
}
