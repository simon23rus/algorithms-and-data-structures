//
//  main.h
//  Ukkonen
//
//  Created by Simon Fedotov on 14.11.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//



#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>
#include <assert.h>

static const short ALPHABET_SIZE = 26;

struct Node;
struct Position;

struct Position {
    Node *finish;
    int distToFinish;
    char firstSymbol;
    
    Position(Node *finish, int distToFinish, const char &firstLetter) : finish(finish), distToFinish(distToFinish), firstSymbol(firstLetter) {}
    Position() = default;
    int getIndex();
    int edgeLength();
    int getOffset();
};

struct Edge;

struct Node {
    Node *link;
    Node *parent;
    std::map<char, Position> children;
    int parentEdgeSubstringStart;
    int parentEdgeSubstringFinish;
    
    Position getPosition(char letter);
    Node(const int &startSubstr = 0, const int &finishSubstr = 0, Node* parent = nullptr)
    : parentEdgeSubstringStart(startSubstr), parentEdgeSubstringFinish(finishSubstr), parent(parent), link(nullptr) {}
};

class SuffixTree {
private:
    std::string ourString;
    Node *fakeRoot;
    Position curPos;
    int lettersAdded;
    
    void deleteNode(Node* toDelete);
    void AddLetter(char letter, Position &position);
    bool CanGo(char toGo, Position &position);
    Node* SplitEdge(char letter, Position &position, bool needNewLeaf = true);
    Node* GetLink(char letter, Position &position);
public:
    Node *root;
    long long findDifferentSubstrings(Node *toCount);
    void PrintTree(Node *toPrint);
    void AddString(const std::string &toAdd);
    SuffixTree();
    ~SuffixTree() {
        delete fakeRoot;
        deleteNode(root);
    }
};



