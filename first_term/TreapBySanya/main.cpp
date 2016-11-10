#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include<queue>

using namespace std;

template <class T>
class Treap;


template <class T>
class Treapnote
{
    friend class Treap<T>;
    template <class S>
    friend ostream& operator<<(ostream& out, const Treapnote<S>& x);
private:
    T* key;
    int priority;
    Treapnote<T>* left;
    Treapnote<T>* right;
    Treapnote<T>* parent;
    int numberinsubtree; /* число вершин в поддереве, исключая данную*/
    void Merge(Treapnote<T>* const leftroot, Treapnote<T>* const rightroot, Treapnote<T>* const commonparent);
    void Delete();
    void Insert(Treapnote<T>* const x);
    Treapnote<T>* Union(Treapnote<T>* b);
    Treapnote<T>* Statistics(int n); /*узел по порядковому номеру (статистике)*/
    int Number(); /*возвращает номер вершины в дереве*/
    void Numberupdate();
    void Numberupdate1(const Treapnote<T>* limit);
    void Obhod(ostream & out);
    pair<Treapnote<T>*, Treapnote<T>*> Split (const Treapnote<T> * b);/* returns the roots of the left and the right trees after split*/
public:
    Treapnote(){ left=0; right=0; parent=0; numberinsubtree=0; key=0; priority=0;}
    Treapnote(const T& newkey, int prior){ left=0; right=0; parent=0; numberinsubtree=0; key=&(T)newkey; priority=prior;}
    Treapnote(T newkey){ key=new T; *key=newkey; left=0; right=0; parent=0; numberinsubtree=0; priority=rand()%1000;}
    ~Treapnote();
};



template <class T>
void Treapnote<T>::Obhod(ostream & out)
{
    out<<*this;
    out<<"left";
    if( left!=0 )
    {
        (*left).Obhod(out);
    }
    out<<"right";
    if( right!=0 )
    {
        (*right).Obhod(out);
    }
}




template <class T>
void Treapnote<T>::Numberupdate()
{
    numberinsubtree=0;
    if(left!=0)
    {
        numberinsubtree=numberinsubtree+(*left).numberinsubtree+1;
    }
    if(right!=0)
    {
        numberinsubtree=numberinsubtree+(*right).numberinsubtree+1;
    }
    if(parent!=0)
    {
        (*parent).Numberupdate();
    }
}


template <class T>
void Treapnote<T>::Numberupdate1(const Treapnote<T>* limit)
{
    numberinsubtree=0;
    if(left!=0)
    {
        numberinsubtree=numberinsubtree+(*left).numberinsubtree+1;
    }
    if(right!=0)
    {
        numberinsubtree=numberinsubtree+(*right).numberinsubtree+1;
    }
    if( (parent!=0) && (parent!=0) )
    {
        (*parent).Numberupdate1(limit);
    }
}



template <class T>
Treapnote<T>::~Treapnote()
{
    if(left!=0)
    {
        delete left;
    }
    if(right!=0)
    {
        delete right;
    }
    if(parent!=0)
    {
        (*parent).left=0; (*parent).right=0;
    }
    parent=0; numberinsubtree=0; delete key; priority=0;
}

template <class T>
void Treapnote<T>:: Delete()
{
    if(parent!=0)
    {
        if ( this==(*parent).left )
        {
            (*parent).left=0;
        }
        else
        {
            (*parent).right=0;
        }
    }
    (*this).Merge(left, right, parent);
    left=0; right=0; parent=0; delete this;
}

template <class T>
void Treapnote<T>::Merge (Treapnote<T>* const leftroot, Treapnote<T>* const rightroot, Treapnote<T>* const commonparent)
{
    if(leftroot==0)
    {
        if (rightroot==0)
        {
            if(commonparent!=0)
            {
                (*commonparent).Numberupdate();
            }
        }
        else
        {
            if (commonparent!=0)
            {
                if( *(*rightroot).key<*(*commonparent).key  )
                {
                    (*commonparent).left=rightroot;
                }
                else
                {
                    ( *commonparent).right=rightroot;
                }
            }
            (*rightroot).parent=commonparent;
            Treapnote<T>* x=(*rightroot).left;
            (*rightroot).left=0;
            Merge( leftroot, x, rightroot);
            x=0;
        }
    }
    else
    {
        if (rightroot==0)
        {
            if (commonparent!=0)
            {
                if( *(*leftroot).key<*(*commonparent).key  )
                {
                    (*commonparent).left=leftroot;
                }
                else
                {
                    (*commonparent).right=leftroot;
                }
            }
            (*leftroot).parent=commonparent;
            Treapnote<T>* x=(*leftroot).right;
            (*leftroot).right=0;
            Merge( x, rightroot, leftroot);
            x=0;
        }
        else
        {
            if ( (*leftroot).priority>(*rightroot).priority )
            {
                if (commonparent!=0)
                {
                    if( *(*leftroot).key<*(*commonparent).key  )
                    {
                        (*commonparent).left=leftroot;
                    }
                    else
                    {
                        (*commonparent).right=leftroot;
                    }
                }
                (*leftroot).parent=commonparent;
                Treapnote<T>* x=(*leftroot).right;
                (*leftroot).right=0;
                Merge( x, rightroot, leftroot);
                x=0;
            }
            else
            {
                if (commonparent!=0)
                {
                    if( *(*rightroot).key<*(*commonparent).key  )
                    {
                        (*commonparent).left=rightroot;
                    }
                    else
                    {
                        (*commonparent).right=rightroot;
                    }
                }
                (*rightroot).parent=commonparent;
                Treapnote<T>* x=(*rightroot).left;
                (*rightroot).left=0;
                Merge( leftroot, x, rightroot);
                x=0;
            }
        }
        
    }
}

template <class T>
void Treapnote<T>::Insert(Treapnote<T>* const x)
{
    if( (*x).priority<priority )
    {
        if( *(*x).key < *key )
        {
            if ( left==0 )
            {
                left=x; (*x).parent=this; (*x).numberinsubtree=0; (*x).Numberupdate();
            }
            else
            {
                (*left).Insert(x);
            }
        }
        else
        {
            if ( right==0 )
            {
                right=x; (*x).parent=this; (*x).numberinsubtree=0; (*x).Numberupdate();
            }
            else
            {
                (*right).Insert(x);
            }
        }
    }
    else
    {
        (*x).parent=parent;
        if(parent!=0)
        {
            if(this==(*parent).left )
            {
                (*parent).left=x;
            }
            else
            {
                (*parent).right=x;
            }
        }
        parent=0;
        pair<Treapnote<T>*, Treapnote<T>*> Pair=Split(x);
        (*x).left=Pair.first; (*x).right=Pair.second;
        if( (*x).left!=0)
        {
            (*(*x).left).parent=x;
        }
        if( (*x).right!=0)
        {
            (*(*x).right).parent=x;
        }
        (*x).Numberupdate();
    }
}

template <class T>
Treapnote<T>* Treapnote<T>:: Union(Treapnote<T>* b)
{
    if(b==0)
    {
        return this;
    }
    else
    {
        if( priority < (*b).priority )
        {
            return ( (*b).Union(this) );
        }
        else
        {
            Treapnote<T>* leftroot1=left;
            Treapnote<T>* rightroot1=right;
            left=0; right=0;
            if(leftroot1!=0)
            {
                (*leftroot1).parent=0;
            }
            if(rightroot1!=0)
            {
                (*rightroot1).parent=0;
            }
            pair<Treapnote<T>*, Treapnote<T>*> Pair=(*b).Split(this);
            Treapnote<T>* leftroot2=Pair.first;
            Treapnote<T>* rightroot2=Pair.second;
            if(leftroot2!=0)
            {
                (*leftroot2).parent=0;
            }
            if(rightroot2!=0)
            {
                (*rightroot2).parent=0;
            }
            Treapnote<T>* leftroot3=0;
            if(leftroot1==0)
            {
                leftroot3=leftroot2;
            }
            else
            {
                leftroot3=(*leftroot1).Union(leftroot2);
            }
            Treapnote<T>* rightroot3=0;
            if(rightroot1==0)
            {
                rightroot3=rightroot2;
            }
            else
            {
                rightroot3=(*rightroot1).Union(rightroot2);
            }
            leftroot1=0; leftroot2=0; rightroot1=0; rightroot2=0;
            left=leftroot3; right=rightroot3;
            leftroot3=0; rightroot3=0;
            if(left!=0)
            {
                (*left).parent=this;
            }
            if(right!=0)
            {
                (*right).parent=this;
            }
            Numberupdate();
            return this;
        }
    }
}


template <class T>
pair<Treapnote<T>*, Treapnote<T>*> Treapnote<T>::Split (const Treapnote<T>  * b)
{
    bool direction=false; vector<Treapnote<T>*> lefts;  vector<Treapnote<T>*> rights;  Treapnote<T> * temp=this;
    Treapnote<T> * templeft=0; Treapnote<T> * tempright=0;
    Treapnote<T> * Left=0; Treapnote<T> * Right =0;
    if( !(*key<*(*b).key ) )
    {
        direction=true;
        rights.push_back(temp);
    }
    else
    {
        lefts.push_back(temp);
        
    }
    while(temp!=0)
    {
        if(direction==true)
        {
            temp=(*temp).left;
            if( temp!=0  )
            {
                if( *(*temp).key<*(*b).key)
                {
                    direction=false; lefts.push_back(temp);
                }
            }
        }
        else
        {
            temp=(*temp).right;
            if( temp!=0  )
            {
                if( !(*(*temp).key<*(*b).key)  )
                {
                    direction=true; rights.push_back(temp);
                }
            }
        }
    }
    while( ( lefts.size()>0 )|| ( rights.size()>0 ) )
    {
        if(direction==true)
        {
            tempright=*(rights.end()-1);
            if(lefts.size()>0)
            {
                templeft=lefts[lefts.size()-1];
                if(tempright->parent!=0)
                {
                    tempright->parent->right=0;
                }
                temp=tempright->parent;
                if( (*templeft).parent!=0 )
                {
                    templeft->parent->left=tempright;
                }
                tempright->parent=templeft->parent;
                templeft->parent=0;
                if(temp!=0){(*temp).Numberupdate();}
                if(rights.size() == 1)
                {
                    (*tempright).Numberupdate();
                }
                else
                {
                    (*tempright).Numberupdate1( *(rights.end()-2) );
                }
                temp=0;
            }
            else
            {
                (*tempright).Numberupdate();
                Right=tempright;
            }
            direction=false;
            if(rights.size()==1)
            {
                Right=rights[0];
            }
            rights.pop_back();
            templeft=0; tempright=0;
        }
        else
        {
            templeft=*(lefts.end()-1);
            if(rights.size()>0)
            {
                tempright=rights[rights.size()-1];
                if(templeft->parent!=0)
                {
                    templeft->parent->left=0;
                }
                temp=templeft->parent;
                if( (*tempright).parent!=0 )
                {
                    tempright->parent->right=templeft;
                }
                templeft->parent=tempright->parent;
                tempright->parent=0;
                if(temp!=0){(*temp).Numberupdate();}
                if(lefts.size() == 1)
                {
                    (*templeft).Numberupdate();
                }
                else
                {
                    (*templeft).Numberupdate1( *(lefts.end()-2) );
                }
                temp=0;
            }
            else
            {
                (*templeft).Numberupdate();
                Left=templeft;
            }
            direction=true;
            if(lefts.size()==1)
            {
                Left=lefts[0];
            }
            lefts.pop_back();
            templeft=0; tempright=0;
        }
    }
    return pair<Treapnote<T>*, Treapnote<T>*>(Left, Right);
}


template <class T>
int Treapnote<T>::Number()
{
    int number=numberinsubtree;
    if(right!=0)
    {
        number=number-(*right).numberinsubtree;
    }
    Treapnote<T> * temp=this;
    while (temp!=0)
    {
        if( (*temp).parent!=0 )
        {
            
            if( temp ==  ( *(*temp).parent ).right )
            {
                number=number+ ( *(*temp).parent ).numberinsubtree - (*temp).numberinsubtree;
            }
            temp=(*temp).parent;
        }
        else
        {
            temp=0;
        }
    }
    temp=0;
    return number;
}


template <class T>
Treapnote<T>* Treapnote<T>::Statistics(int n)
{
    if( (n<0)||(n>numberinsubtree)   )
    {
        return 0;
    }
    int t;
    if(right==0)
    {
        t=numberinsubtree;
    }
    else
    {
        t=numberinsubtree-(*right).numberinsubtree-1;
    }
    if( n==t)
    {
        return this;
    }
    if( n<t )
    {
        return (*left).Statistics(n);
    }
    else
    {
        return( (*right).Statistics(n-t-1)  );
    }
}


template <class T>
class Treap
{
private: Treapnote <T>* head;
public: void Insert (const T& key, int& prior);
    void Insert(const T& key);
    void Insert(Treapnote <T>* const trepnote);
    void Delete (const T& key);
    void Delete (Treapnote<T>* key);
    void Union (Treap<T> &b);
    Treapnote <T>* Searchkey(const T& searchkey);
    Treap(){ srand(time(0) ); head=0;}
    Treap(const vector<T> & x);
    ~Treap();
    int Number(T& element);
    T Statistics(int k, ostream & out);
    void Headcheck();
    void Obhod(ostream & out);
};


template <class T>
int Treap<T>::Number(T& element)
{
    Treapnote<T>* A=Searchkey(element);
    if( A!=0)
    {
        int x=(*A ).Number() ;
        A=0;
        return x;
    }
    else
    {
        return 0;
    }
}

template <class T>
T Treap<T>:: Statistics(int k, ostream & out)
{
    Treapnote<T>* A=0;
    if(head!=0)
    {
        A=(*head).Statistics(k-1);
    }
    if(A!=0)
    {
        out<< *(*A).key<<endl; return 0;
    }
    else
    {
        out<<"NONE"<<endl; return 0;
    }
}

template <class T>
void Treap<T>::Union (Treap<T> &b)
{
    if(head!=0)
    {
        (*head).Union(b.head);
        Headcheck();
        b.head=0;
    }
    else
    {
        head=b.head;
        b.head=0;
    }
}


template <class T>
Treap<T>::Treap(const vector<T> & x)
{
    Treap();
    for(int i=0; i < x.size(); i++)
    {
        Insert(x[i]);
    }
}


template <class T>
Treap<T>::~Treap()
{
    if(head!=0)
    {
        delete head;
    }
    head=0;
}

template <class T>
void Treap<T>::Insert(const T& key, int& prior)
{
    Treapnote<T>* temp=new Treapnote<T>( (T)key, prior);
    Insert( temp);
}

template <class T>
void Treap<T>::Insert(const T& key)
{
    Treapnote<T>* temp=new Treapnote<T>( (T)key);
    Insert( temp);
}


template <class T>
void Treap<T>::Insert(Treapnote <T>* const treapnote)
{
    if(head!=0)
    {
        (*head).Insert(treapnote);
        Headcheck();
    }
    else
    {
        head=treapnote;
        (*head).left=0;
        (*head).right=0;
        (*head).parent=0;
        (*head).numberinsubtree=0;
    }
}



template <class T>
void Treap<T>::Delete(const T& deletekey)
{
    Treapnote<T>* searchresult=Searchkey(deletekey);
    if(searchresult==0)
    {
    }
    else
    {
        Delete(searchresult);
    }
    searchresult=0; delete searchresult;
}

template <class T>
void Treap<T>::Delete (Treapnote<T>* note)
{
    if(note==head)
    {
        if(  (*head).left!=0)
        {
            head=(*head).left;
        }
        else
        {
            if(  (*head).right!=0)
            {
                head=(*head).right;
            }
            else
            {
                head=0;
            }
        }
    }
    (*note).Delete();
    Headcheck();
    note=0; delete note;
}


template <class T>
void Treap<T>::Headcheck()
{
    if(head!=0)
    {
        while( (*head).parent!=0 )
        {
            head=(*head).parent;
        }
    }
}


template <class T>
void Treap<T>:: Obhod(ostream & out)
{
    if(head!=0)
    {
        (*head).Obhod(out);
    }
}

template <class T>
Treapnote <T>* Treap<T>::Searchkey(const T& searchkey)
{
    bool found=false;
    Treapnote<T>* temp=head;
    while( (temp!=0) && (found==false) )
    {
        if( *(*temp).key==searchkey)
        {
            found=true;
        }
        else
        {
            if ( searchkey<*(*temp).key)
            {
                temp=(*temp).left;
            }
            else
            {
                temp=(*temp).right;
            }
        }
    }
    return temp;
}

template <class T>
ostream& operator<<(ostream& out, const Treapnote<T>& x)
{
    out<<*x.key<<";"<<x.priority<<"number"<<x.numberinsubtree<<endl;
    return out;
}

int main()
{
    
    /*ifstream in;
     in.open("test22-00001.txt");
     std::ios_base::sync_with_stdio(false);
     std::string OurCommand = "";
     Treap<int> Tree;
     std::queue<int> OurQueue;
     int NTH;    int OurNum;
     in >> NTH;
     while(OurCommand != "STOP") {
     in >> OurCommand;
     if(OurCommand == "PUSH") {
     
     in >> OurNum;
     Tree.Insert(OurNum);
     OurQueue.push(OurNum);
     Tree.Statistics(NTH);}
     else if (OurCommand == "POP") {
     OurNum=OurQueue.front();
     Tree.Delete(OurNum);
     OurQueue.pop();
     Tree.Statistics(NTH);
     }
     } in.close();
     ifstream in; ofstream out, out1; in.open("test22-00001.txt");
     out.open("koko.txt"); out1.open("koku.txt");
     std::ios_base::sync_with_stdio(false);*/
    std::string OurCommand = "";
    Treap<int> Tree;
    std::queue<int> OurQueue;
    int NTH;   int OurNum;
    cin >> NTH;
    while(OurCommand != "STOP") {
        cin >> OurCommand;
        if(OurCommand == "PUSH") {
            cin >> OurNum;
            Tree.Insert(OurNum); /*Tree.Control();*/
            OurQueue.push(OurNum);
            /*out1<<"PRINT  ";out1<<endl;Tree.Obhod(out1); out1<<endl;*/
            Tree.Statistics(NTH,cout);
        }
        else if (OurCommand == "POP") {
            OurNum=OurQueue.front();
            Tree.Delete(OurNum); /*Tree.Control();*/
            OurQueue.pop();
            /*out1<<"PRINT"<<endl;Tree.Obhod(out1); out1<<endl;*/
            Tree.Statistics(NTH,cout);
        }
    }
    /* in.close(); out.close(); out1.close();*/
    return 0;
}