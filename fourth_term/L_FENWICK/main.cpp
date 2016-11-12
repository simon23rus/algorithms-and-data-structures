//
//  main.cpp
//  L_FENWICK
//
//  Created by Simon Fedotov on 07.04.16.
//  Copyright © 2016 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>


class Fenwick {
private:
    std::vector<int> tree;
public:
    int sum(int position);
    int sumOnSegment(int left, int right);
    void update(int position, int delta);
    Fenwick(int size);
    std::vector<int> getTree() const;
};

std::vector<int> Fenwick::getTree() const {
    return tree;
}

Fenwick::Fenwick(int size) {
    tree.resize(size);
}

int Fenwick::sum(int position) {
    int result = 0;
    if(position < 0)
        return 0;
    for(; position >= 0;  position = (position & (position + 1)) - 1)
        result += tree[position];
    return result;
}

int Fenwick::sumOnSegment(int left, int right) {
    return sum(right) - sum(left - 1);
}

void Fenwick::update(int position, int delta) {
    for(; position < tree.size(); position |= position + 1)
        tree[position] += delta;
}

int main()
{
    freopen("permutation.in","r", stdin);
    freopen("permutation.out","w", stdout);
    int numberOfElements, numberOfQueries;
    scanf("%d %d" , &numberOfElements , &numberOfQueries);
    std::vector<std::pair<int,int>> permutation(numberOfElements);
    std::vector<int> answers(numberOfQueries);
    std::vector<char> visited(numberOfQueries);
    Fenwick tree = Fenwick(numberOfElements);
    
    for(int i = 0; i < numberOfElements; ++i) {
        int elem;
        scanf("%d" , &elem);
        permutation[i] = {elem, i};
    }
    
    std::vector<std::pair<std::pair<int, int>,std::pair<int, int>>> queries(2 * numberOfQueries);
    int x, y , k , l;
    for(int i = 0; i < numberOfQueries; ++i) {
        scanf("%d %d %d %d", &x, &y, &k, &l);
        --x; --y;
        queries[i * 2] = {{k - 1, i},{x, y}};
        queries[i * 2 + 1] = {{l, i},{x, y}};
    }
    std::sort(permutation.begin(), permutation.end());
    std::sort(queries.begin(), queries.end());
    
    int counter = 0;
    for(int i = 0; i < queries.size(); ++i) {
        int val = queries[i].first.first;
        int queryState = queries[i].first.second;
        int x =  queries[i].second.first;
        int y =  queries[i].second.second;
        for(int j = counter; j < numberOfElements; ++j) {
            if(permutation[j].first > val)
                break;
            tree.update(permutation[j].second , 1);
            ++counter;
        }
        int res = tree.sumOnSegment(x, y);
        if(!visited[queryState]) {
            answers[queryState] -= res;
            visited[queryState] = true;
        }
        else {
            answers[queryState] += res;
        }
    }
    for(auto ans : answers)
        printf("%d\n", ans);
    return 0;
}