#include <vector>
#include <list>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <stack>

enum color{WHITE, GREY, BLACK};

template <typename T, typename S>
class vertex {
public:
    vertex();
    vertex(T val);
    void operator=(vertex& other);
    bool IsSink();
    
    bool exist_ = 0;
    T value_;
    std::list<std::pair<int, S> > adj_;
    int col_ = WHITE;
};

template <typename T, typename S, bool R = 0>
class graph {
public:
    graph();
    graph(int n);
    graph(const graph<T, S, R>& g);
    
    void Flip();
    graph<T, S, R> Flipped();
    void DeleteVertex(int x);
    void DeleteEdge(int x, int y);
    void AddVertex(int x, vertex<T, S>& v);
    void AddVertex(int x, T val);
    void AddEdge(int x, int y);
    void AddEdgeVal(T val1, T val2);
    bool ExistVal(T val);
    bool ExistEdge(int x, int y);
    bool IsSink(T val);
    bool IsSource(T val);
    
    void BFS(T val);
    void DFS(T val);
    std::vector<T> BFStoIt(T val);
    std::vector<T> DFStoIt(T val);
    void DFSToStrong(T val, std::set<std::pair<T, T> >& res);
    
    std::set<std::pair<T, T> > ToStrong();
    
    std::vector<int> GetVertexFrom(int x) const;
    std::vector<int> GetVertexTo(int x) const;
    std::vector<std::pair<int, int> > GetEdgeFrom(int x) const;
    std::vector<std::pair<int, int> > GetEdgeTo(int x) const;
    std::map<T, int> GetExisted();
    std::set<T> GetDepend();
    std::set<T> GetSources();
    std::set<T> GetSinks();
    
    //    class bfs_const_iterator {
    //    public:
    //        bfs_const_iterator(std::vector<T>& bfs_vertex, typename std::vector<T>::const_iterator it);
    //        bfs_const_iterator operator++();
    //        bfs_const_iterator operator++(int);
    //        T operator*() const;
    //        bool operator!=(const bfs_const_iterator other) const;
    //    private:
    //        std::vector<T> bfs_vertex_;
    //        typename std::vector<T>::const_iterator it_;
    //    };
    //
    //    bfs_const_iterator bfs_begin(T val) {
    //        std::vector<T> vec = BFStoIt(val);
    //        bfs_const_iterator it(vec, vec.cbegin());
    //        return it;
    //    }
    //
    //    bfs_const_iterator bfs_end(T val) {
    //        std::vector<T> vec = BFStoIt(val);
    //        bfs_const_iterator it(vec, vec.cend());
    //        return it;
    //    }
    
    template <typename TT, typename SS, bool RR>
    friend std::ostream& operator<<(std::ostream& stream, graph<TT, SS, RR>& g);
    template <typename TT, typename SS, bool RR>
    friend std::istream& operator>>(std::istream& stream, graph<TT, SS, RR>& g);
private:
    std::vector<vertex<T, S> > data_; // Main structure of graph
    std::map<T, int> existed_; // Map of existed T values with their ID
    std::set<T> dependence_; // Here I write names of classes which are in cyclic dependence
    int n_; // Quantity of vertexes
    bool oriented_;
};

template <typename T, typename S, bool R>
std::istream& operator>>(std::istream& stream, graph<T, S, R>& g);

template <typename T, typename S, bool R>
std::ostream& operator<<(std::ostream& stream, graph<T, S, R>& g);

#pragma mark Implementation

//template <typename T, typename S, bool R>
//graph<T, S, R>::bfs_const_iterator::bfs_const_iterator(std::vector<T>& bfs_vertex, typename std::vector<T>::const_iterator it): bfs_vertex_(bfs_vertex), it_(it) {
//
//}
//
//template <typename T, typename S, bool R>
//typename graph<T, S, R>::bfs_const_iterator graph<T, S, R>::bfs_const_iterator::operator++() {
//    ++it_;
//    return *this;
//}
//
//template <typename T, typename S, bool R>
//typename graph<T, S, R>::bfs_const_iterator graph<T, S, R>::bfs_const_iterator::operator++(int) {
//    ++it_;
//    bfs_const_iterator it2 = *this;
//    return it2;
//}
//
//template <typename T, typename S, bool R>
//T graph<T, S, R>::bfs_const_iterator::operator*() const {
//    return *it_;
//}
//
//template <typename T, typename S, bool R>
//bool graph<T, S, R>::bfs_const_iterator::operator!=(const bfs_const_iterator other) const {
//    return it_ != other.it_;
//}

template <typename T, typename S>
vertex<T, S>::vertex() {
    
}

template <typename T, typename S>
vertex<T, S>::vertex(T val): value_(val) {
    
}

template <typename T, typename S>
bool vertex<T, S>::IsSink() {
    return adj_.size() == 0;
}

template <typename T, typename S>
void vertex<T, S>::operator=(vertex<T, S>& other) {
    exist_ = other.exist_;
    value_ = other.value_;
    adj_ = other.adj_;
    col_ = other.col_;
}

template <typename T, typename S, bool R>
graph<T, S, R>::graph(): oriented_(R) {
    
}

template <typename T, typename S, bool R>
graph<T, S, R>::graph(int n): n_(n), oriented_(R), data_(std::vector<vertex<T, S> >(n)) {
    
}

template <typename T, typename S, bool R>
graph<T, S, R>::graph(const graph<T, S, R>& g): data_(g.data_), n_(g.n_), oriented_(g.oriented_), existed_(g.existed_), dependence_(g.dependence_) {
    
}

template <typename T, typename S, bool R>
bool graph<T, S, R>::IsSink(T val) {
    return data_[existed_.find(val)->second].adj_.size() == 0;
}

template <typename T, typename S, bool R>
bool graph<T, S, R>::IsSource(T val) {
    graph<T, S, R> buf(this->Flipped());
    return buf.data_[buf.existed_.find(val)->second].adj_.size() == 0;
}

template <typename T, typename S, bool R>
void graph<T, S, R>::Flip() {
    if (oriented_ == 1) {
        std::vector<vertex<T, S> > data(n_);
        for (int i = 0; i < n_; ++i) {
            data[i].value_ = data_[i].value_;
        }
        for (int i = 0; i < n_; ++i) {
            for (auto it = data_[i].adj_.begin(); it != data_[i].adj_.end(); ++it) {
                data[it->first].adj_.push_back(std::pair<int, S>(i, it->second));
            }
        }
        data_ = data;
    }
}

template <typename T, typename S, bool R>
graph<T, S, R> graph<T, S, R>::Flipped() {
    graph<T, S, R> res(*this);
    if (res.oriented_ == 1) {
        std::vector<vertex<T, S> > data(n_);
        for (int i = 0; i < n_; ++i) {
            data[i].value_ = res.data_[i].value_;
        }
        for (int i = 0; i < n_; ++i) {
            for (auto it = res.data_[i].adj_.begin(); it != res.data_[i].adj_.end(); ++it) {
                data[it->first].adj_.push_back(std::pair<int, S>(i, it->second));
            }
        }
        res.data_ = data;
    }
    return res;
}

template <typename T, typename S, bool R>
void graph<T, S, R>::DeleteEdge(int x, int y) {
    if (oriented_ == 1) {
        auto it = data_[x].adj_.begin();
        while (it->first != y && it != data_[x].adj_.end()) ++it;
        if (it != data_[x].adj_.end()) data_[x].adj_.erase(it);
    }
    else {
        auto it = data_[x].adj_.begin();
        while (it->first != y && it != data_[x].adj_.end()) ++it;
        if (it != data_[x].adj_.end()) data_[x].adj_.erase(it);
        
        it = data_[y].adj_.begin();
        while (it->first != x && it != data_[y].adj_.end()) ++it;
        if (it != data_[y].adj_.end()) data_[y].adj_.erase(it);
    }
}

template <typename T, typename S, bool R>
void graph<T, S, R>::DeleteVertex(int x) {
    data_[x].first = NULL;
    data_[x].adj_.clear();
    for (auto itdata = data_.begin(); itdata != data_.end(); ++itdata) {
        for (auto itlist = itdata->adj_.begin(); itlist != itdata->adj_.end(); ++itlist) {
            if (itlist->first == x) itdata->adj_.erase(itlist);
        }
    }
}

template <typename T, typename S, bool R>
std::map<T, int> graph<T, S, R>::GetExisted() {
    return existed_;
}

template <typename T, typename S, bool R>
std::set<T> graph<T, S, R>::GetDepend() {
    return dependence_;
}

template <typename T, typename S, bool R>
std::set<T> graph<T, S, R>::GetSources() {
    std::set<T> sources;
    for (auto it : existed_) {
        if (this->IsSource(it.first)) {
            sources.insert(it.first);
        }
    }
    return sources;
}

template <typename T, typename S, bool R>
std::set<T> graph<T, S, R>::GetSinks() {
    std::set<T> sinks;
    for (auto it : existed_) {
        if (this->IsSink(it.first)) {
            sinks.insert(it.first);
        }
    }
    return sinks;
}

template <typename T, typename S, bool R>
void graph<T, S, R>::AddVertex(int x, vertex<T, S>& v) {
    if (!ExistVal(v.value_)) {
        if (n_ < x + 1) {
            data_.resize(1 + (3 * x)/2);
            n_ = 1 + (3 * x)/2;
            v.exist_ = 1;
            data_[x] = v;
            existed_.push(std::pair<T, int>(v.value_, x));
        }
        else {
            v.exist_ = 1;
            data_[x] = v;
            existed_.push(std::pair<T, int>(v.value_, x));
        }
    }
}

template <typename T, typename S, bool R>
void graph<T, S, R>::AddVertex(int x, T val) {
    if (!ExistVal(val)) {
        if (n_ < x + 1) {
            data_.resize(1 + (3 * x)/2);
            n_ = 1 + (3 * x)/2;
            vertex<T, S> v(val);
            v.exist_ = 1;
            data_[x] = v;
            existed_.insert(std::pair<T, int>(val, x));
        }
        else {
            vertex<T, S> v(val);
            v.exist_ = 1;
            data_[x] = v;
            existed_.insert(std::pair<T, int>(val, x));
        }
    }
}

template <typename T, typename S, bool R>
bool graph<T, S, R>::ExistVal(T val) {
    return existed_.find(val) != existed_.end();
}

template <typename T, typename S, bool R>
bool graph<T, S, R>::ExistEdge(int x, int y) {
    auto it = data_[x].adj_.begin();
    for (; it != data_[x].adj_.end(); ++it) {
        if (it->first == y) break;
    }
    return it != data_[x].adj_.end();
}

template <typename T, typename S, bool R>
void graph<T, S, R>::AddEdge(int x, int y) {
    if (!ExistEdge(x, y)) {
        if (oriented_ == 1) {
            data_[x].adj_.push_back(std::pair<int, int>(y, 0));
        }
        else {
            data_[x].adj_.push_back(std::pair<int, int>(y, 0));
            data_[y].adj_.push_back(std::pair<int, int>(x, 0));
        }
    }
}

template <typename T, typename S, bool R>
void graph<T, S, R>::AddEdgeVal(T val1, T val2) {
    int x = existed_.find(val1)->second;
    int y = existed_.find(val2)->second;
    
    if (!ExistEdge(x, y)) {
        if (oriented_ == 1) {
            data_[x].adj_.push_back(std::pair<int, int>(y, 0));
        }
        else {
            data_[x].adj_.push_back(std::pair<int, int>(y, 0));
            data_[y].adj_.push_back(std::pair<int, int>(x, 0));
        }
    }
}

template <typename T, typename S, bool R>
std::vector<std::pair<int, int> > graph<T, S, R>::GetEdgeFrom(int x) const {
    std::vector<std::pair<int, int> > res;
    for (auto it = data_[x].adj_.begin(); it != data_[x].adj_.end(); ++it) {
        res.push_back(std::pair<int, int>(x, it->first));
    }
    return res;
}

template <typename T, typename S, bool R>
std::vector<int> graph<T, S, R>::GetVertexFrom(int x) const {
    std::vector<int> res;
    for (auto it = data_[x].adj_.begin(); it != data_[x].adj_.end(); ++it) {
        res.push_back(it->first);
    }
    return res;
}

template <typename T, typename S, bool R>
std::vector<int> graph<T, S, R>::GetVertexTo(int x) const {
    if (oriented_ == 1) {
        std::vector<int> res;
        graph<T, S, R> g(*this);
        g.Flip();
        for (auto it = g.data_[x].adj_.begin(); it != g.data_[x].adj_.end(); ++it) {
            res.push_back(it->first);
        }
        return res;
    }
    else {
        std::vector<int> res;
        for (auto it = data_[x].adj_.begin(); it != data_[x].adj_.end(); ++it) {
            res.push_back(it->first);
        }
        return res;
    }
}

template <typename T, typename S, bool R>
std::vector<std::pair<int, int> > graph<T, S, R>::GetEdgeTo(int x) const {
    if (oriented_ == 1) {
        std::vector<std::pair<int, int> > res;
        graph<T, S, R> g(*this);
        g.Flip();
        for (auto it = g.data_[x].adj_.begin(); it != g.data_[x].adj_.end(); ++it) {
            res.push_back(std::pair<int, int>(it->first, x));
        }
        return res;
    }
    else {
        std::vector<std::pair<int, int> > res;
        for (auto it = data_[x].adj_.begin(); it != data_[x].adj_.end(); ++it) {
            res.push_back(std::pair<int, int>(it->first, x));
        }
        return res;
    }
}

template <typename T, typename S, bool R>
void graph<T, S, R>::BFS(T val) {
    std::deque<vertex<T, S> > deq;
    for (int i = 0; i < n_; ++i) {
        data_[i].col_ = WHITE;
    }
    int x = existed_.find(val)->second;
    data_[x].col_ = GREY;
    deq.push_back(data_[x]);
    while (!deq.empty()) {
        vertex<T, S> u = deq.front();
        deq.pop_front();
        std::cout << u.value_ << std::endl;
        for (auto it : u.adj_) {
            vertex<T, S> v = data_[it.first];
            if (v.col_ == WHITE) {
                v.col_ = GREY;
                data_[it.first].col_ = GREY;
                deq.push_back(v);
            }
        }
        if (u.value_ != val) {
            u.col_ = BLACK;
        }
    }
}

template <typename T, typename S, bool R>
std::vector<T> graph<T, S, R>::BFStoIt(T val) {
    std::vector<T> res;
    std::deque<vertex<T, S> > deq;
    for (int i = 0; i < n_; ++i) {
        data_[i].col_ = WHITE;
    }
    int x = existed_.find(val)->second;
    data_[x].col_ = GREY;
    deq.push_back(data_[x]);
    while (!deq.empty()) {
        vertex<T, S> u = deq.front();
        res.push_back(u.value_);
        deq.pop_front();
        for (auto it : u.adj_) {
            vertex<T, S> v = data_[it.first];
            if (v.col_ == WHITE) {
                v.col_ = GREY;
                data_[it.first].col_ = GREY;
                deq.push_back(v);
            }
        }
        if (u.value_ != val) u.col_ = BLACK;
    }
    return res;
}

template <typename T, typename S, bool R>
void graph<T, S, R>::DFS(T val) {
    std::cout << "DFS:" << std::endl;
    std::stack<vertex<T, S> > stk;
    for (int i = 0; i < n_; ++i) {
        data_[i].col_ = WHITE;
    }
    int x = existed_.find(val)->second;
    data_[x].col_ = BLACK;
    stk.push(data_[x]);
    while (!stk.empty()) {
        vertex<T, S> u = stk.top();
        stk.pop();
        for (auto it : u.adj_) {
            vertex<T, S> v = data_[it.first];
            if (v.col_ == WHITE) {
                v.col_ = BLACK;
                stk.push(v);
                data_[it.first].col_ = BLACK;
            }
        }
    }
    std::cout << "END DFS" << std::endl;
}

template <typename T, typename S, bool R>
void graph<T, S, R>::DFSToStrong(T val, std::set<std::pair<T, T> >& res) {
    std::stack<vertex<T, S> > stk;
    for (int i = 0; i < n_; ++i) {
        data_[i].col_ = WHITE;
    }
    int x = existed_.find(val)->second;
    data_[x].col_ = BLACK;
    stk.push(data_[x]);
    while (!stk.empty()) {
        vertex<T, S> u = stk.top();
        stk.pop();
        if (u.IsSink()) {
            res.insert(std::make_pair(u.value_, val));
        }
        for (auto it : u.adj_) {
            vertex<T, S> v = data_[it.first];
            if (v.col_ == WHITE) {
                v.col_ = BLACK;
                stk.push(v);
                data_[it.first].col_ = BLACK;
            }
        }
    }
}

template <typename T, typename S, bool R>
std::set<std::pair<T, T> > graph<T, S, R>::ToStrong() {
    std::set<std::pair<T, T> > res;
    auto src = this->GetSources();
    for (auto it : src) {
        this->DFSToStrong(it, res);
    }
    return res;
}

template <typename T, typename S, bool R>
std::vector<T> graph<T, S, R>::DFStoIt(T val) {
    std::vector<T> res;
    std::stack<vertex<T, S> > stk;
    for (int i = 0; i < n_; ++i) {
        data_[i].col_ = WHITE;
    }
    int x = existed_.find(val)->second;
    data_[x].col_ = BLACK;
    stk.push(data_[x]);
    while (!stk.empty()) {
        vertex<T, S> u = stk.top();
        res.push_back(u.value_);
        stk.pop();
        for (auto it : u.adj_) {
            vertex<T, S> v = data_[it.first];
            if (v.col_ == WHITE) {
                v.col_ = BLACK;
                stk.push(v);
                data_[it.first].col_ = GREY;
            }
        }
    }
    return res;
}

template <typename T, typename S, bool R>
std::istream& operator>>(std::istream& stream, graph<T, S, R>& g) {
    if (g.oriented_ == 1) {
        int count;
        stream >> count;
        int myid = 0;
        for (int i = 0; i < count; ++i) {
            T vert1;
            stream >> vert1;
            T vert2;
            stream >> vert2;
            g.AddVertex(myid, vert1);
            ++myid;
            g.AddVertex(myid, vert2);
            ++myid;
            g.AddEdgeVal(vert1, vert2);
        }
        return stream;
    }
    else {
        int count;
        stream >> count;
        int myid = 0;
        for (int i = 0; i < count; ++i) {
            T vert1;
            stream >> vert1;
            T vert2;
            stream >> vert2;
            g.AddVertex(myid, vert1);
            ++myid;
            g.AddVertex(myid, vert2);
            ++myid;
            g.AddEdgeVal(vert1, vert2);
            g.AddEdgeVal(vert2, vert1);
        }
        return stream;
    }
}

template <typename T, typename S, bool R>
std::ostream& operator<<(std::ostream& stream, graph<T, S, R>& g) {
    for (int i = 0; i < g.n_; ++i) {
        for (auto it = g.data_[i].adj_.begin(); it != g.data_[i].adj_.end(); ++it) {
            stream << g.data_[i].value_ << " -> " << g.data_[it->first].value_ << std::endl;
        }
    }
    return stream;
}


#include <iostream>
#include <vector>

int main() {
    int vertexes;
    std::cin >> vertexes;
    graph<int, int, 1> g(vertexes);
    
    std::cin >> g;
    
    //    std::cout << g;
    
    auto a = g.ToStrong();
    
    std::cout << a.size() << std::endl;
    for (auto it : a) {
        std::cout << it.first << ' ' << it.second << std::endl;
    }
    
    return 0;
}