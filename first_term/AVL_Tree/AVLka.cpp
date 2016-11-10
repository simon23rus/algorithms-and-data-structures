//
//  main.cpp
//  AVL_Tree
//
//  Created by Simon Fedotov on 22.11.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <cstddef>

template  <typename OurType, typename Comparator = std::less<OurType>>

struct Node {
    OurType key;
    size_t height;
    size_t NodeNumber;
    Node* parent;
    Node* leftChild;
    Node* rightChild;
    Node(OurType k) {
        key = k;
        height = 1;
        NodeNumber = 1;
        parent = NULL;
        leftChild = NULL;
        rightChild = NULL;
    }
    ~Node () {
        if(this -> leftChild)
            delete leftChild;
        if(this -> rightChild)
            delete rightChild;
        if(this -> parent) {
            if(this -> parent -> leftChild == this)
                this -> parent -> leftChild = NULL;
            else
                this -> parent -> rightChild = NULL;
        }
        this -> parent = NULL;
    }
};

template  <typename OurType, typename Comparator = std::less<OurType>>

size_t height(Node <OurType, Comparator> * DatNode) {
    return DatNode ? DatNode -> height : 0;
}

template  <typename OurType, typename Comparator = std::less<OurType>>

void RepairNodeNumber(Node <OurType, Comparator> * DatNode) {
    unsigned int result = 1;
    if(DatNode -> leftChild)
        result += DatNode -> leftChild -> NodeNumber;
    if(DatNode -> rightChild)
        result += DatNode -> rightChild -> NodeNumber;
    DatNode -> NodeNumber = result;
}


template  <typename OurType, typename Comparator = std::less<OurType>>

ptrdiff_t BalanceDifference (Node <OurType, Comparator> * DatNode) {
    return (ptrdiff_t)height(DatNode -> leftChild) - (ptrdiff_t)height(DatNode -> rightChild);
}

template  <typename OurType, typename Comparator = std::less<OurType>>

void RepairHeight (Node <OurType, Comparator>* DatNode) {
    size_t left = height(DatNode -> leftChild);
    size_t right = height(DatNode ->rightChild);
    
    DatNode -> height = std::max(left, right) + 1;
}



template  <typename OurType, typename Comparator = std::less<OurType>>

void RotationLeftVoid(Node <OurType, Comparator>* DatNode) {
    Node <OurType, Comparator>* DatNodeRightSon = DatNode -> rightChild;
    if(DatNode -> parent) {
        if(DatNode -> parent -> leftChild == DatNode)
            DatNode -> parent -> leftChild = DatNodeRightSon;
        else
            DatNode -> parent -> rightChild = DatNodeRightSon;
    }
    DatNode -> rightChild = DatNodeRightSon -> leftChild;
    if(DatNode -> rightChild)
        DatNode -> rightChild -> parent = DatNode;
    
    DatNodeRightSon -> parent = DatNode -> parent;
    DatNode -> parent = DatNodeRightSon;
    DatNodeRightSon -> leftChild = DatNode;
    RepairHeight(DatNode);
    RepairNodeNumber(DatNode);
    RepairHeight(DatNodeRightSon);
    RepairNodeNumber(DatNodeRightSon);
    if(DatNodeRightSon -> parent) {
        RepairHeight(DatNodeRightSon -> parent);
        RepairNodeNumber(DatNodeRightSon -> parent);
    }
}



template  <typename OurType, typename Comparator = std::less<OurType>>

void RotationRightVoid(Node <OurType, Comparator>* DatNode) {
    Node <OurType, Comparator>* DatNodeLeftSon= DatNode -> leftChild;
    if(DatNode -> parent) {
        if(DatNode -> parent -> leftChild == DatNode)
            DatNode -> parent -> leftChild = DatNodeLeftSon;
        else
            DatNode -> parent -> rightChild = DatNodeLeftSon;
    }
    DatNode -> leftChild = DatNodeLeftSon -> rightChild;
    if(DatNode -> leftChild)
        DatNode -> leftChild -> parent = DatNode;
    
    DatNodeLeftSon -> parent = DatNode -> parent;
    DatNode -> parent = DatNodeLeftSon;
    DatNodeLeftSon -> rightChild = DatNode;
    RepairHeight(DatNode);
    RepairNodeNumber(DatNode);
    RepairHeight(DatNodeLeftSon);
    RepairNodeNumber(DatNodeLeftSon);
    if(DatNodeLeftSon -> parent) {
        RepairHeight(DatNodeLeftSon -> parent);
        RepairNodeNumber(DatNodeLeftSon -> parent);
    }
}

template  <typename OurType, typename Comparator = std::less<OurType>>

void GrandLeftRotation (Node <OurType, Comparator>* DatNode) {
    RotationRightVoid(DatNode -> rightChild);
    RotationLeftVoid(DatNode);
}

template  <typename OurType, typename Comparator = std::less<OurType>>

void GrandRightRotation (Node <OurType, Comparator>* DatNode) {
    RotationLeftVoid(DatNode -> leftChild);
    RotationRightVoid(DatNode);
}


template  <typename OurType, typename Comparator = std::less<OurType>>

void BalanceNode(Node <OurType, Comparator>* DatNode) {
    if(BalanceDifference(DatNode) == 2) {
        if(BalanceDifference(DatNode -> leftChild) < 0) {
            GrandRightRotation(DatNode);
        }
        else
            RotationRightVoid(DatNode);
    }
    if(BalanceDifference(DatNode) == -2) {
        if(BalanceDifference(DatNode -> rightChild) > 0)
            GrandLeftRotation(DatNode);
        else
            RotationLeftVoid(DatNode);
    }
    
    
}


template  <typename OurType, typename Comparator = std::less<OurType>>

const OurType &GetValue (Node <OurType, Comparator>* DatNode)  {
    return DatNode -> key;
}


template  <typename OurType, typename Comparator = std::less<OurType>>


Node<OurType> * AddNewNode(Node <OurType, Comparator>* &DatRoot, OurType OurKey) {
    Node<OurType> * ToReturn;
    bool End = false;
    while (!End) {
        if(OurKey < DatRoot -> key) {
            if(DatRoot -> leftChild) {
                DatRoot = DatRoot -> leftChild;
            }
            else {
                ToReturn =  new Node<OurType>(OurKey);
                ToReturn -> parent = DatRoot;
                DatRoot -> leftChild = ToReturn;
                End = true;
                break;
            }
        }
        else {
            if(DatRoot -> rightChild ) {
                DatRoot = DatRoot -> rightChild;
            }
            else {
                ToReturn =  new Node<OurType>(OurKey);
                ToReturn -> parent = DatRoot;
                DatRoot -> rightChild = ToReturn;
                End = true;
                break;
            }
        }
      
    }
    RepairHeight(DatRoot);
    RepairNodeNumber(DatRoot);
    while ((DatRoot -> parent)) {
        DatRoot = DatRoot -> parent;
        RepairHeight(DatRoot);
        RepairNodeNumber(DatRoot);
        BalanceNode(DatRoot);
    }
    return ToReturn;
}

template  <typename OurType, typename Comparator = std::less<OurType>>

Node<OurType> * InsertNode(Node <OurType>* &DatRoot, OurType OurKey) {
    if(DatRoot == NULL) {
        DatRoot = new Node <OurType> (OurKey);
        return DatRoot;
        
    }
    else {
        Node<OurType> * ToReturn = AddNewNode(DatRoot, OurKey);
        return ToReturn;
    }
}


template  <typename OurType, typename Comparator = std::less<OurType>>

Node <OurType>* FindMin (Node <OurType>* DatRoot) {
    while (DatRoot -> leftChild)
        DatRoot = DatRoot -> leftChild;
    return DatRoot;
}

template  <typename OurType, typename Comparator = std::less<OurType>>

void DeleteMin (Node <OurType>* &OurRoot, Node <OurType>* DatNode) {
    if(DatNode -> rightChild) {
        if(DatNode -> parent) {
            if(DatNode -> parent -> leftChild == DatNode)
                DatNode -> parent -> leftChild = DatNode -> rightChild;
            else
                DatNode -> parent -> rightChild = DatNode -> rightChild;
            DatNode -> rightChild -> parent = DatNode -> parent;
            Node <OurType>* ToRepair = DatNode -> parent;
            DatNode -> parent = NULL;
            DatNode -> leftChild = NULL;
            DatNode -> rightChild = NULL;
            delete DatNode;
            while (ToRepair) {
                RepairHeight(ToRepair);
                RepairNodeNumber(ToRepair);
                BalanceNode(ToRepair);
                if(!(ToRepair -> parent))
                    break;
                ToRepair = ToRepair -> parent;
            }
            OurRoot = ToRepair;
        }
        else {
            DatNode -> leftChild = NULL;
            DatNode -> rightChild = NULL;
            OurRoot = NULL;
            delete DatNode;
        }
    }
    else {
        if(DatNode -> parent) {
            Node <OurType>* ToRepair = DatNode -> parent;
            DatNode -> leftChild = NULL;
            DatNode -> rightChild = NULL;
            delete DatNode;
            while (ToRepair) {
                RepairHeight(ToRepair);
                RepairNodeNumber(ToRepair);
                BalanceNode(ToRepair);
                if(!(ToRepair -> parent))
                    break;
                ToRepair = ToRepair -> parent;
            }
            OurRoot = ToRepair;
        }
        else {
            OurRoot = NULL;
            delete DatNode;
        }
        
    }
}



template  <typename OurType, typename Comparator = std::less<OurType>>

void DeleteNode (Node <OurType>* &OurRoot, Node<OurType>* DatNode) {
    if((!(DatNode -> rightChild)) && (!(DatNode -> leftChild))) {
        if(DatNode -> parent) {
            Node <OurType>* ToRepair = DatNode -> parent;
            delete DatNode;
            while(ToRepair) {
                RepairHeight(ToRepair);
                RepairNodeNumber(ToRepair);
                BalanceNode(ToRepair);
                if(!(ToRepair -> parent))
                    break;
                ToRepair = ToRepair -> parent;
            }
            OurRoot = ToRepair;
        }
        else {
            OurRoot = NULL;
            delete DatNode;
        }
    }
    
    else if((DatNode -> leftChild) && (!(DatNode -> rightChild))) {
        if(DatNode -> parent) {
            if(DatNode -> parent -> leftChild == DatNode)
                DatNode -> parent -> leftChild = DatNode -> leftChild;
            else
                DatNode -> parent -> rightChild = DatNode -> leftChild;
            DatNode -> leftChild -> parent = DatNode -> parent;
            Node <OurType>* ToRepair = DatNode -> parent;
            DatNode -> parent = NULL;
            DatNode -> leftChild = NULL;
            delete DatNode;
            while (ToRepair) {
                RepairHeight(ToRepair);
                RepairNodeNumber(ToRepair);
                BalanceNode(ToRepair);
                if(!(ToRepair -> parent))
                    break;
                ToRepair = ToRepair -> parent;
            }
            OurRoot = ToRepair;
        }
        else {
            OurRoot = DatNode -> leftChild;
            DatNode -> leftChild = NULL;
            delete DatNode;
            OurRoot -> parent = NULL;
        }
    }
    else if((DatNode -> rightChild) && (!(DatNode -> leftChild))) {
        if(DatNode -> parent) {
            if(DatNode -> parent -> leftChild == DatNode)
                DatNode -> parent -> leftChild = DatNode -> rightChild;
            else
                DatNode -> parent -> rightChild = DatNode -> rightChild;
            DatNode -> rightChild -> parent = DatNode -> parent;
            Node <OurType>* ToRepair = DatNode -> parent;
            DatNode -> parent = NULL;
            DatNode -> rightChild = NULL;
            delete DatNode;
            while (ToRepair) {
                RepairHeight(ToRepair);
                RepairNodeNumber(ToRepair);
                BalanceNode(ToRepair);
                if(!(ToRepair -> parent))
                    break;
                ToRepair = ToRepair -> parent;
            }
            OurRoot = ToRepair;
        }
        else {
            OurRoot = DatNode -> rightChild;
            DatNode -> rightChild = NULL;
            delete DatNode;
            OurRoot -> parent = NULL;
        }
    }
    else {
        Node <OurType>* DatMin = FindMin(DatNode -> rightChild);
        if(DatNode == OurRoot)
            OurRoot = DatMin;
        Node <OurType>* tmp = new Node<OurType>(DatMin -> key);
        tmp -> height = DatMin -> height;
        tmp -> parent = DatMin -> parent;
        tmp -> leftChild = DatMin -> leftChild;
        tmp -> rightChild = DatMin -> rightChild;
        std::swap(DatMin -> NodeNumber, DatNode -> NodeNumber);
        if(DatMin -> leftChild)
            DatMin -> leftChild -> parent = DatNode;
        if(DatMin -> rightChild)
            DatMin -> rightChild -> parent = DatNode;
        if(DatNode -> leftChild)
            DatNode -> leftChild -> parent = DatMin;
        if(DatNode -> rightChild) {
            if(DatNode -> rightChild != DatNode)
                DatNode -> rightChild -> parent = DatMin;
            else  {
                DatNode -> rightChild -> parent = NULL;
                DatNode -> rightChild = NULL;
            }
        }
        DatMin -> leftChild = DatNode -> leftChild;
        DatMin -> rightChild = DatNode -> rightChild;
        DatNode -> leftChild = tmp -> leftChild;
        DatNode -> rightChild = tmp -> rightChild;
        if(DatNode -> parent) {
            if(DatNode -> parent -> leftChild == DatNode)
                DatNode -> parent -> leftChild = DatMin;
            else
                DatNode -> parent -> rightChild = DatMin;
        }
        if(DatMin -> parent) {
            if(DatMin -> parent -> leftChild == DatMin)
                DatMin -> parent -> leftChild = DatNode;
            else
                DatMin -> parent -> rightChild = DatNode;
        }
        DatMin -> parent = DatNode -> parent;
        if(DatNode == tmp -> parent) {
            DatNode -> parent = DatMin;
        }
        else
            DatNode -> parent = tmp -> parent;
        DeleteMin(OurRoot, DatNode);
    }
}

template  <typename OurType, typename Comparator = std::less<OurType>>

OurType NthElement(Node<OurType>* DatRoot, size_t nth) {
        if(DatRoot -> leftChild) {
        while (DatRoot -> leftChild -> NodeNumber != nth - 1) {
            if(DatRoot -> leftChild -> NodeNumber >= nth)
                DatRoot = DatRoot -> leftChild;
            else {
                nth -= DatRoot -> leftChild -> NodeNumber + 1;
                DatRoot = DatRoot -> rightChild;
            }
            if(nth == 1 && !(DatRoot -> leftChild))
                return DatRoot -> key;
            if(nth == 2 && !(DatRoot -> leftChild))
                return DatRoot -> rightChild -> key;
        }
        return DatRoot -> key;
    }
    else {
        if(nth == 1)
            return DatRoot -> key;
        
        return DatRoot -> rightChild -> key;
    }
}


template  <typename OurType, typename Comparator = std::less<OurType>>

void PrintTree(Node<OurType>* Root) {
    if(Root) {
        std::cout<<Root -> key << " " << Root -> NodeNumber  << " \n";
        std::cout << "left ";
        if(Root -> leftChild)
            PrintTree(Root -> leftChild);
        std::cout << "right ";
        if(Root -> rightChild)
            PrintTree(Root -> rightChild);
    }
    else
        std::cout << "NULL \n";
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::string OurCommand = "";
    Node<int, std::less<int>>* Root = NULL;
    std::queue<Node<int>*> OurQueue;
    size_t NTH;
    std:: cin >> NTH;
    while(OurCommand != "STOP") {
        std:: cin >> OurCommand;
        if(OurCommand == "PUSH") {
            int OurNum;
            std:: cin >> OurNum;
            OurQueue.push(InsertNode(Root, OurNum));
            if(OurQueue.size() >= NTH)
                std:: cout << NthElement(Root, NTH) << "\n";
            else
                std:: cout << "NONE \n";
        }
        else if (OurCommand == "PRINT") {
            std:: cout << "\n";
            PrintTree(Root);
        }
        else if (OurCommand == "POP") {
            DeleteNode(Root, OurQueue.front());
            OurQueue.pop();
            if(OurQueue.size() >= NTH)
                std:: cout << NthElement(Root, NTH) << "\n";
            else
                std:: cout << "NONE \n";
        }
    }
    delete Root;
    return 0;
}
