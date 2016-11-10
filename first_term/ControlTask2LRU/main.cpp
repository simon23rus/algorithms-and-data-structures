//
//  main.cpp
//  ControlTask2
//
//  Created by Simon Fedotov on 15.12.14.
//  Copyright (c) 2014 Simon23Rus. All rights reserved.
//

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <vector>

template<typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>

void selectionSort(Iterator begin, Iterator end, const Comparator &comp = Comparator()) {
    if(end - begin == 0)
        return;
    Iterator FirstIt;
    for(FirstIt = begin; FirstIt < end; ++FirstIt) {
        Iterator SecondIt, minimal = FirstIt;
        for(SecondIt = FirstIt; SecondIt < end; ++SecondIt) {
            if(comp(*SecondIt , *minimal))
                minimal = SecondIt;
        }
        std::iter_swap(FirstIt, minimal);
        minimal = FirstIt;
    }
}

typedef std::string Key;

struct Value {
    Key key;
    Value(Key DatKey) {key = DatKey;}
};

Value Get(const Key &key) {
    std::vector<int> Slow;
    for(int i = 0; i < 12345; i++) {
        Slow.push_back(rand() % 12345 + 1);
    }
    selectionSort(Slow.begin(), Slow.end());
    return Value(key + key.substr(0, key.size()/4));
}

const size_t CACHED_SIZE = 5;
int DatTime = 0;


struct Cache {
    std::map<Key, Key> OurCache;
    std::map<Key, int> OurKeys;
    std::map<int, Key> OurTimes;
};

Value CachedGet(const Key &key) {
    ++DatTime;
    static Cache cache;
    
    auto OurFind = cache.OurKeys.find(key);
    
    if(OurFind != cache.OurKeys.end()) {
        auto TempKey = key;
        auto TempTime = OurFind -> second;
        cache.OurTimes.erase(TempTime);
        cache.OurKeys.erase(TempKey);
        std::pair<Key, int> ThisKey(TempKey, DatTime);
        std::pair<int, Key> ThisTime(DatTime, TempKey);
        cache.OurKeys.insert(ThisKey);
        cache.OurTimes.insert(ThisTime);
        auto OurResult = cache.OurCache.find(key);
        return OurResult -> second;
    }
    
    auto OurRes = Get(key);
    
    if(cache.OurCache.size() == CACHED_SIZE) {
        auto NeedToDeleteInTimes = cache.OurTimes.begin();
        auto NeedToDeleteinKeys = cache.OurKeys.find(NeedToDeleteInTimes -> second);
        cache.OurTimes.erase(NeedToDeleteInTimes);
        cache.OurKeys.erase(NeedToDeleteinKeys);
        cache.OurCache.erase(cache.OurCache.find(NeedToDeleteinKeys -> first));
    }
        std::pair<int, Key> TimePair(DatTime, key);
        cache.OurTimes.insert(TimePair);
        std::pair<Key, int> KeyPair(key, DatTime);
        cache.OurKeys.insert(KeyPair);
        std::pair<Key, Key> DatCache(key, OurRes.key);
        cache.OurCache.insert(DatCache);
    return OurRes;
    
}

int main() {
    int n;
    std::cin >> n;
    for(int i = 0; i < n; i++) {
        Key OurKey;
        std::cin >> OurKey;
        std::cout << CachedGet(OurKey).key;
        std::cout << "\n";
    }
    return 0;
}
