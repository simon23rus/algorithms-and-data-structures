#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cfloat>
#include <math.h>
#include <cstdlib>
#include <set>
#include <iomanip>

const long double  PI = acos(-1);

const long long MAXN = 100010;

long long n, cnt, mrk[MAXN * 2];

long long  res;

enum type_of_border {
    START = 0,
    FINISH = 1
};

struct Circles{
    long long x, y;
    long long index;
    long long r;
    Circles(long long x = 0, long long y = 0, long long r = 0, long long index = 0): x(x), y(y), r(r), index(index) {}
} circles[MAXN];

bool operator <(const Circles& first, const Circles& second) {
    return (first.y < second.y || first.y == second.y && first.r < second.r ||
            first.y == second.y && first.r == second.r && first.index < second.index);
}


long long dist(long long first_x, long long first_y, long long second_x, long long second_y){
    return (long long)(second_x - first_x) * (long long)(second_x - first_x)
    + (long long)(second_y - first_y) * (long long)(second_y - first_y);
}

struct Event {
    long long index;
    long long x, r;
    long long type;
    Event(long long index = 0, long long x = 0, long long type = false, long long r = 0): index(index), x(x), type(type), r(r) {}
} events[MAXN * 2];



bool operator <(const Event& first, const Event& second) {
    return (first.x < second.x) || (first.x == second.x && first.type > second.type) ||
    (first.x == second.x && first.type == second.type && first.r > second.r);
}

std::set <Circles> set_of_circles;

void init() {
    scanf("%d", &n);
    cnt = 0;
    res = 0;
    for(int i = 0; i < n; ++i){
        scanf("%I64d%I64d%I64d", &circles[i].x, &circles[i].y, &circles[i].r);
        circles[i].index = i;
        events[cnt] = Event(i, circles[i].x - circles[i].r, START, circles[i].r);
        mrk[i] = 0;
        ++cnt;
        events[cnt] = Event(i, circles[i].x + circles[i].r, FINISH, circles[i].r);
        ++cnt;
    }
    
}
int main (){
    freopen("circles.in", "rt", stdin);
    freopen("circles.out","wt", stdout);
    
    init();
    
    std::sort(events, events + cnt);
    
    for(int i = 0; i < cnt; ++i){
        Event tmp_event = events[i];
        
        if(events[i].type == FINISH){
            
            set_of_circles.erase(circles[tmp_event.index]);
        }
        else{
            if(!set_of_circles.empty()){
                auto tmp_circ = set_of_circles.lower_bound(circles[tmp_event.index]);
                
                long long cnt = 0;
                while(cnt < 20 && tmp_circ != set_of_circles.end()) {
                    cnt++;
                    ++tmp_circ;
                }
                if (tmp_circ == set_of_circles.end())
                    --tmp_circ;
                cnt = 0;
                while(cnt < 40) {
                    if (dist(tmp_circ->x, tmp_circ->y,
                             circles[tmp_event.index].x, circles[tmp_event.index].y) <=
                        (long long) tmp_circ->r * (long long)tmp_circ->r){
                        mrk[tmp_event.index] = 1;
                        break;
                    }
                    cnt++;
                    
                    if (tmp_circ == set_of_circles.begin())
                        break;
                    --tmp_circ;
                }
                
            }
            if (mrk[tmp_event.index] != 1) {
                res += (long long)tmp_event.r * (long long)tmp_event.r;
                set_of_circles.insert(circles[tmp_event.index]);
            }
        }
    }
    std::cout << std::fixed << std::setprecision(11) << res * PI << "\n";
}