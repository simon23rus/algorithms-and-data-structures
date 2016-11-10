//
//  main.cpp
//  stack
//
//  Created by Semyon Fedotov on 25.09.14.
//  Copyright (c) 2014 mipt. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

struct stack
{
    vector <int> a;
};

string push(stack &s, int x)
{
    s.a.push_back(x);
    return "ok";
}

string pop(stack &s)
{
    if(s.a.size() == 0) return "error";
    s.a.pop_back();
    return "fine";
}

string back(stack &s)
{
    if(s.a.size() == 0) return "error";
    return "fine";
}

int size(stack &s)
{
    return s.a.size();
}

string clear(stack &s)
{
    s.a.clear();
    return "ok";
}

string exit(stack &s)
{
    return "bye";
}

int main() {
    stack st;
    int pushik;
    string s;
    while (s != "exit")
    {
        cin >> s;
        if(s == "pop")
        {
            if(pop(st) == "error") cout << "error" << endl;
            else cout << st.a[st.a.size()] << endl;
        }
        if(s == "push")
        {
            cin >> pushik;
            cout << push(st, pushik) << endl;
        }
        if(s == "back")
        {
            if(back(st) == "error") cout << "error" << endl;
            else cout << st.a[st.a.size() - 1] << endl;
        }
        if(s == "size") cout << size(st) << endl;
        if(s == "clear") cout << clear(st) << endl;
        if(s == "exit") cout << exit(st);
    }
    return 0;
}
