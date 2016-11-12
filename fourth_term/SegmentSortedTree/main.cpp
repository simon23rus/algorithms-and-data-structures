//
//  main.cpp
//  L
//
//  Created by Simon Fedotov on 28.03.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <set>

#define root 0
#define _CRT_DISABLE_PERFCRIT_LOCKS

class SegmentSortedTree {
private:
    std::vector<std::vector<int>> tree;
    std::vector<int> permutation;
public:
    SegmentSortedTree(const std::vector<int> &elems);
    void buildTree(int node, int left, int right);
    int query(int node, int firstIndex, int secondIndex, int leftBound, int rightBound, int left, int right);
    inline int getSize() const;
};

inline int SegmentSortedTree::getSize() const {
    return permutation.size();
}

SegmentSortedTree::SegmentSortedTree(const std::vector<int> &elems) {
    permutation = elems;
    int log = (int)log2(elems.size());
    int nearestPow = pow(2, log);
    int newSize = elems.size() == nearestPow ? elems.size() : pow(2, log + 1);
    permutation.resize(newSize);
    tree.resize(pow(2, log2(newSize) + 1) - 1);
    buildTree(0, 0, permutation.size() - 1);
}

void SegmentSortedTree::buildTree(int node, int left, int right) {
    if(left == right)
        tree[node] = {permutation[left]};
    else {
        int middle = (left + right) >> 1;
        buildTree( (node << 1) + 1, left, middle);
        buildTree( (node << 1) + 2, middle + 1, right);
        tree[node].resize( (tree[(node << 1) + 1] ).size()<<1);
        std::merge(tree[(node << 1) + 1].begin(), tree[(node << 1) + 1].end(), tree[(node << 1)  + 2].begin(), tree[(node << 1) + 2].end(), tree[node].begin());
    }
}

int SegmentSortedTree::query(int node, int firstIndex, int secondIndex, int leftBound, int rightBound, int left, int right) {
    if(left > right || firstIndex > secondIndex)
        return 0;
    if(firstIndex == left && secondIndex == right) {
        std::vector<int>::iterator lowerBound = std::lower_bound(tree[node].begin(), tree[node].end(), leftBound);
        std::vector<int>::iterator upperBound = std::upper_bound(tree[node].begin(), tree[node].end(), rightBound);
        if(lowerBound != tree[node].end())
            return upperBound - lowerBound;
        return 0;
    }
    int middle = (left + right) >> 1;
    return
    query( (node <<1) + 1, firstIndex, std::min(secondIndex, middle), leftBound, rightBound, left, middle)
    +
    query( (node << 1) + 2, std::max(firstIndex, middle + 1), secondIndex, leftBound, rightBound, middle + 1, right);
}


int ReadInt() {
    char c;
    int n = 0;
    while (true) {
        c = getchar();
        if ('0' <= c && c <= '9') {
            n = 10 * n + (c - '0');
        } else {
            break;
        }
    }
    return n;
}

char s[12];

void WriteInt(int n) {
    int len = 0;
    do {
        s[len] = (char)('0' + n % 10);
        ++len;
        n /= 10;
    } while (n != 0);
    for (int i = len - 1; i >= 0; --i) {
        putchar(s[i]);
    }
}


int main(int argc, const char * argv[]) {
    int range, numberOfQueries;
    std::ios_base::sync_with_stdio(false);
    freopen("/Users/semenfedotov/Desktop/4SemestrCoding/L/L/in.txt", "r", stdin);
    freopen("/Users/semenfedotov/Desktop/4SemestrCoding/L/L/out.txt", "w", stdout);
    
    //    std::cin >> range >> numberOfQueries;
//    scanf("%d %d\n", &range, &numberOfQueries);
    auto start = std::chrono::steady_clock::now();
    range = ReadInt();
    numberOfQueries = ReadInt();
    std::vector<int> permutation;
    permutation.resize(range);
    for(int i = 0; i < range; ++i) {
//        std::cin >> elem;
//       
    int j = 0;
//    while ((ch = getchar()) != ' ' && ch != '\n') {
//        if (ch > '9' || ch < '0')
//            continue;
//        j *= 10;
//        j += ch - '0';
//    }
//        scanf("%d", &j);
//        std::cout << j << " ";
//        int elem;
        permutation[i] = ReadInt();
        
    }
    SegmentSortedTree tree(permutation);
    int size = tree.getSize();
//    std::vector<int> data(4);
    std::vector<std::vector<int>> queris(numberOfQueries);
    for(int j = 0; j < numberOfQueries; ++j) {
        int firstIndex = ReadInt(), secondIndex = ReadInt(), leftBound = ReadInt(), rightBound = ReadInt();
        
//        std::cin >> firstIndex >> secondIndex >> leftBound >> rightBound;
//        scanf("%d %d %d %d", &firstIndex, &secondIndex, &leftBound, &rightBound);
        
//        for(int k = 0; k < 4; ++k) {
//            int i = 0;
//            int ch;
//            while ((ch = getchar()) != ' ' && ch != '\n') {
//                if (ch > '9' || ch < '0')
//                    continue;
//                i *= 10;
//                i += ch - '0';
//                data[k] = i;
//            }
//        }
        
        
//        for(auto &elem : data)
//            std::cout << elem << " ";
//        std::cout << " \n";
//        std::cout << tree.query(root, firstIndex - 1, secondIndex - 1, leftBound, rightBound, 0, tree.getSize() - 1) << "\n";
//        printf("%d\n", tree.query(root, firstIndex - 1, secondIndex - 1, leftBound, rightBound, 0, size - 1));
        
        
        
        
        
        WriteInt(tree.query(root, firstIndex - 1, secondIndex - 1, leftBound, rightBound, 0, size - 1));
        putchar('\n');
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "\n\n\nAZAZA   " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001;
    return 0;
}
