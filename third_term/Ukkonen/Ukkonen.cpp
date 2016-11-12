//
//  main.cpp
//  Ukkonen
//
//  Created by Simon Fedotov on 14.11.15.
//  Copyright © 2015 Simon23Rus. All rights reserved.
//


#include "Ukkonen.h"

Position Node::getPosition(char letter) {
    if(this -> link == this)
        return {this, 0, letter};
    return parent -> children.find(letter) -> second;
}

int Position::getIndex() {
    return finish -> parentEdgeSubstringFinish - distToFinish;
}

int Position::edgeLength() {
    if(finish -> parentEdgeSubstringFinish == 0)
        return 1;
    return finish -> parentEdgeSubstringFinish - finish -> parentEdgeSubstringStart;
}

int Position::getOffset() {
    return this -> edgeLength() - this -> distToFinish;
}

SuffixTree::SuffixTree() {
    fakeRoot = new Node(-1, 0, fakeRoot);
    root  = new Node;
    curPos = Position(root, 0, 'a');
    fakeRoot -> link = fakeRoot;
    fakeRoot -> parent = fakeRoot;
    for(int i = 0; i < ALPHABET_SIZE; ++i) {
        Position newChild = Position(root, 0, (char)('a' + i));
        fakeRoot -> children.insert({(char)('a' + i), newChild});
    }
    root -> parent = fakeRoot;
    lettersAdded = 0;
    root -> link = fakeRoot;
}

void SuffixTree::deleteNode(Node* toDelete) {
    if(toDelete == nullptr)
        return;
    for(auto iter = toDelete -> children.begin(); iter != toDelete -> children.end(); ++iter)
        deleteNode((*iter).second.finish);
    delete toDelete;
}

bool SuffixTree::CanGo(char toGo, Position &position) {
    if(position.finish == fakeRoot)
        return true;
    if(position.distToFinish == 0)
        return position.finish -> children.count(toGo);
    return ourString[position.finish -> parentEdgeSubstringFinish - position.distToFinish] == toGo;
}

Node* SuffixTree::GetLink(char letter, Position &position) {
    if(position.distToFinish == 0 && position.finish -> children.size() == 0)
        return nullptr;
    if(position.distToFinish == 0 && position.finish -> link != nullptr)
        return position.finish -> link;

    Node *hisParent = position.finish -> parent;
    Node *hisParentLink = hisParent -> link;
    int lengthOfSegment = position.getOffset();
    int distance = 0;
    int currentIndex = position.finish -> parentEdgeSubstringStart;
    Position whereAmI = hisParentLink -> children.find(ourString[currentIndex]) -> second;
    whereAmI.firstSymbol = ourString[currentIndex];
    distance += whereAmI.edgeLength();
    currentIndex += whereAmI.edgeLength();
    while(distance < lengthOfSegment) {
        whereAmI = whereAmI.finish -> children.find(ourString[currentIndex]) -> second;
        distance += whereAmI.edgeLength();
        currentIndex += whereAmI.edgeLength();
    }
    if(distance == lengthOfSegment) {
        //my v vershine
        position = whereAmI;
        return whereAmI.finish;
    }
    else {
        //посреди ребра
        position = Position(whereAmI.finish, distance - lengthOfSegment, whereAmI.firstSymbol);
        return SplitEdge(letter, position, false);
    }
    
}

void SuffixTree::AddLetter(char letter, Position &position) {
        if(curPos.distToFinish == 0) {
            //мы в узле
                Node *newLeaf = new Node(lettersAdded, ourString.size(), curPos.finish);
                Position newLeafFinish = Position(newLeaf, 0, letter);
                curPos.finish -> children.insert({letter, newLeafFinish});
                Node* linkNode = GetLink(letter, curPos);
                if(linkNode == fakeRoot)
                    curPos = {linkNode, 0, ourString[0]};
                else
                    curPos = {linkNode, 0, ourString[linkNode -> parentEdgeSubstringStart]};
                newLeaf -> parent -> link = linkNode;
        }
        else {
            //посреди ребра
                Node *newNode = SplitEdge(letter, curPos);
                Node *link = GetLink(letter, curPos);
                newNode -> link = link;
                curPos = Position{link, 0, ourString[link -> parentEdgeSubstringStart]};
        }
}

Node* SuffixTree::SplitEdge(char letter, Position &position , bool needNewLeaf) {
    
    Node *newNodeOnEdge = new Node(position.finish -> parentEdgeSubstringStart, position.finish -> parentEdgeSubstringFinish - position.distToFinish, position.finish -> parent);
    if(needNewLeaf) {
        Node *newLeaf = new Node(lettersAdded, ourString.size(), newNodeOnEdge);
        Position newLeafPosition = Position(newLeaf, 0, letter);
        newNodeOnEdge -> children.insert({letter, newLeafPosition});
    }
    Position newNodeOnEdgePosition = Position(newNodeOnEdge, 0, position.firstSymbol);
    Position oldSon = (position.finish -> parent -> children.find(position.firstSymbol)) -> second;
    Position toPush = Position(oldSon.finish, oldSon.distToFinish, ourString[position.getIndex()]);
    position.finish -> parent -> children.erase(position.firstSymbol);
    position.finish -> parent -> children.insert({position.firstSymbol, newNodeOnEdgePosition});
    position.finish -> parentEdgeSubstringStart = position.getIndex();
    position.finish -> parent = newNodeOnEdge;
    position = newNodeOnEdgePosition;
    newNodeOnEdge -> children.insert({ourString[position.getIndex()], toPush});
    return newNodeOnEdge;
}


void SuffixTree::AddString(const std::string &toAdd) {
    ourString = toAdd;
    int index = 0;
    while(index < toAdd.length()) {
        while(!CanGo(toAdd[index], curPos)) {
            AddLetter(toAdd[index], curPos);
        }
        if(curPos.distToFinish == 0) {
            Node *son = (curPos.finish -> children.find(toAdd[index]) -> second).finish;
            if(son == root)
                curPos = Position(root, 0, toAdd[index]);
            else
                curPos = Position(son, son -> parentEdgeSubstringFinish - son -> parentEdgeSubstringStart - 1 , toAdd[index]);
        
        }
        else
            --curPos.distToFinish;

        ++index;
        ++lettersAdded;
    }
}


long long SuffixTree::findDifferentSubstrings(Node *toCount) {
    long long result = 0;
    result += toCount -> parentEdgeSubstringFinish - toCount -> parentEdgeSubstringStart;
    for(auto i = toCount -> children.begin(); i != toCount -> children.end(); ++i) {
        result += findDifferentSubstrings(i -> second.finish);
    }
    return result;
}


//вывод дерева
void SuffixTree::PrintTree(Node *toPrint) {
    std::cout << ourString[toPrint -> parentEdgeSubstringStart] << " " << toPrint -> parentEdgeSubstringStart << " " << toPrint -> parentEdgeSubstringFinish << "\nstart\n";
    for(auto i = toPrint -> children.begin(); i != toPrint -> children.end(); ++i) {
        PrintTree(i -> second.finish);
    }
    std::cout << "end\n";
}

int main(int argc, const char * argv[]) {
    std::ios_base::sync_with_stdio(false);
    int windowSize;
    std::string ourString;
    std::cin >> windowSize;
    std::cin >> ourString;
    int size = ourString.length();
    ourString += ourString.substr(0, windowSize);
    for(int i = 0; i < size; ++i) {
        SuffixTree myTree = SuffixTree();
        myTree.AddString(ourString.substr(i, windowSize));
        std::cout << myTree.findDifferentSubstrings(myTree.root) << " ";
    }
    return 0;
}
