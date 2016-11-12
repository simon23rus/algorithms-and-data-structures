//
//  main.cpp
//  contestA
//
//  Created by Simon Fedotov on 27.02.16.
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

std::ifstream in;
std::ofstream out;

struct Point2DUPD {
    long long x, y;
    long long squaredLength() const {
        return x * x + y * y;
    }
    Point2DUPD operator -(const Point2DUPD &other) {
        return {(long long)(x - other.x), (long long)(y - other.y)};
    }
};

struct Point2D {
    long double x;
    long double y;
    
    long double length() const {
        return sqrt(x * x + y * y);
    }
    
    long double squaredLength() const {
        return x * x + y * y;
    }
    
    friend std::ostream & operator <<(std::ostream & out, const Point2D &elem);
    
    Point2D operator -(const Point2D &other) {
        return {x - other.x, y - other.y};
    }
    Point2D operator +(const Point2D &other) {
        return {x + other.x, y + other.y};
    }
    
    bool operator ==(const Point2D &other) {
        return (x == other.x) && (y == other.y);
    }
    
    bool operator <(const Point2D &other) const {
        return (x < other.x) || (x == other.x && y < other.y);

    }
    
    Point2D operator *(const long double &coeff) {
        return {coeff * x, coeff * y};
    }
};

std::ostream & operator <<(std::ostream & out, const Point2D & elem) {
    out <<  "[" << elem.x << ";" << elem.y << "]\n";
    return out;
}

Point2D normalize(const Point2D &first, const Point2D &second) {
    Point2D res = {second.x - first.x, second.y - first.y};
    return res * (1 / res.length());
}

Point2D normal(const Point2D &first, const Point2D &second) {
    Point2D res = {(second.y - first.y), (first.x - second.x)};
    return res * (1 / res.length());
}


long double dot(Point2D first, Point2D second) {
    return first.x * second.y - first.y * second.x;
}

long double scalar(Point2D first, Point2D second) {
    return first.x * second.x + first.y * second.y;
}

bool isOnLine(Point2D first, Point2D second, Point2D dot) {
    auto a = (first - dot).length() + (second - dot).length();
    auto b = (second - first).length();
    return (std::abs((first - dot).length() + (second - dot).length() - (second - first).length()) < 0.000001);
    
    
}

long double distance(long double dotX, long double dotY, long double startX, long double startY, long double endX, long double endY) {
    Point2D main = {dotX, dotY};
    Point2D first = {startX, startY};
    Point2D second = {endX, endY};
    long double res = std::abs(dot(second - first, main - first) / (second - first).length());
    Point2D onSegmentPos = main + normal(second, first) * res;
    Point2D onSegmentNeg = main - normal(second, first) * res;
    if(first == second) {
        res = (main - second).length();
    }
    else if(!(isOnLine(first, second, onSegmentPos) || isOnLine(first, second, onSegmentNeg))) {
        res = std::min((first - main).length(), (second - main).length());
        auto resa = (first - main).length();
        auto resb = (second - main).length();
        
    }
    else {
        res = std::abs(dot(second - first, main - first) / (second - first).length());
    }
    return res;
}

long double distanceUPD(Point2D main, Point2D first, Point2D second) {
    long double res = std::abs(dot(second - first, main - first) / (second - first).length());
    Point2D onSegmentPos = main + normal(second, first) * res;
    Point2D onSegmentNeg = main - normal(second, first) * res;
    if(first == second) {
        res = (main - second).length();
    }
    else if(!(isOnLine(first, second, onSegmentPos) || isOnLine(first, second, onSegmentNeg))) {
        res = std::min((first - main).length(), (second - main).length());
        auto resa = (first - main).length();
        auto resb = (second - main).length();
        
    }
    else {
        res = std::abs(dot(second - first, main - first) / (second - first).length());
    }
    return res;
}


Point2D segmentIntersection(long double firstA, long double firstB, long double firstC, long double secondA, long double secondB, long double secondC) {
    long double x, y;
    long double det1 = dot({firstA, firstB},{secondA, secondB});
    long double det2 = dot({-firstC, firstB}, {-secondC, secondB});
    long double det3 = dot({firstA, -firstC}, {secondA, -secondC});
    x = det2 / det1; y = det3 / det1;
//    if(firstB == 0) {
//        x = -(firstC / firstA);
//        y = -(secondA / secondB) * x - secondC / secondB;
//    }
//    else if(secondB == 0) {
//        x = -(secondC / secondA);
//        y = -(firstA / firstB) * x - firstC / firstB;
//    }
//    else if(firstA == 0) {
//        y = -(firstC / firstB);
//        x = -(secondB / secondA) * y - secondC / secondA;
//    }
//    else if(secondA == 0) {
//        y = -(secondC / secondB);
//        x = -(firstB / firstA) * y - firstC / firstA;
//    }
//    else {
//        x = (firstC / firstB - secondC / secondB) / (secondA / secondB - firstA / firstB);
//        y = -(firstA / firstB) * x - firstC / firstB;
//    }
    Point2D ans = {x, y};
//    std::cout << ans;
    return ans;
}

bool hasSegmentIntersection(long double x1, long double y1, long double x2, long double y2, long double a1, long double b1, long double a2,long double b2) {
    long double x1Coeff = (y2 - y1) / (x2 - x1);
    long double x2Coeff = (b2 - b1) / (a2 - a1);
    Point2D first = {x1, y1}, second = {x2,y2}, third = {a1,b1}, fourth = {a2,b2};
    if(dot({x2 - x1, y2 - y1}, {a2 - a1,b2 - b1}) == 0) {
        //параллельны
        if(isOnLine(first, second, third) || isOnLine(first, second, fourth) || isOnLine(third, fourth, first) || isOnLine(third, fourth, second))
            return true;
        else
            return false;
    }
    auto dot = segmentIntersection(y2 - y1, x1 - x2, x1 * y1 - x1 * y2 + x2 * y1 - x1 * y1, b2 - b1, a1 - a2, a1 * b1 - a1 * b2 + a2 * b1 - a1 * b1);
    if(isOnLine(first, second, dot) && isOnLine(third, fourth, dot))
        return true;
    else
        return false;
    
}

void roundIntersection(long double firstX, long double firstY, long double firstRadius, long double secondX, long double secondY, long double secondRadius) {
    std::cout.precision(20);
    out.precision(20);
    bool flag = false;
    if(firstRadius < secondRadius) {
        //first - bolshiy radius
        std::swap(firstRadius, secondRadius);
        std::swap(firstX, secondX);
        std::swap(firstY, secondY);
        flag = true;
    }
    Point2D first = {firstX, firstY};
    Point2D second = {secondX, secondY};
    long double centerDistance = (second - first).length();
    if(first == second && firstRadius == secondRadius) {
        //inf
        out << 3 << "\n";
        return;
    }
    else if(centerDistance > firstRadius + secondRadius || centerDistance + secondRadius < firstRadius) {
        //0 tochek
        out << 0 << "\n";
        return;
    }
    else if(centerDistance == firstRadius + secondRadius || centerDistance + secondRadius == firstRadius) {
        //1 tochka
        Point2D vector = normalize(first, second);
        Point2D dot = first + vector * firstRadius;
        out << 1 << "\n";
        out << dot.x << " " << dot.y << "\n";
        return;
    }
    else {
//        assert(false);
        //ERROR
        long double res;
        long double HP;
        Point2D perp2 = second + normal(first, second) * secondRadius;
        out << 2 << "\n";
        if(centerDistance < firstRadius) {
            if((perp2 - first).length() < firstRadius) {
//            assert(false); ERROR
                long double O2H = (firstRadius * firstRadius - secondRadius * secondRadius - centerDistance * centerDistance) / (2 * centerDistance);
                long double O1H = centerDistance + O2H;
                res = flag ? O2H : O1H;
                HP = sqrt(secondRadius * secondRadius - O2H * O2H);
                Point2D H = second + normalize(first, second) * O2H;
                Point2D interFirst = H + normal(first, second) * HP;
                Point2D interSecond = H - normal(first, second) * HP;
                out << H.x << " " << H.y << "\n";
                out << res << " " << HP << "\n";
                out << interFirst.x << " " << interFirst.y << "\n";
                out << interSecond.x << " " << interSecond.y << "\n";
                return;
            }
            else if((perp2 - first).length() == firstRadius) {
                res = flag ? 0 : centerDistance;
                Point2D perp3 = second - normal(first, second) * secondRadius;
                out << second.x << " " << second.y << "\n";
                out << res << " " << secondRadius << "\n";
                out << perp2.x << " " << perp2.y << "\n";
                out << perp3.x << " " << perp3.y << "\n";
            }
            else {
                long double O1HminusO2H = (firstRadius * firstRadius - secondRadius * secondRadius) / centerDistance;
                long double O1H = (O1HminusO2H + centerDistance) / 2;
                long double O2H = centerDistance - O1H;
                long double HP = sqrt(firstRadius * firstRadius - O1H * O1H);
                Point2D H = first + normalize(first, second) * O1H;
                Point2D interFirst, interSecond;
                interFirst = H + normal(first, second) * HP;
                interSecond = H - normal(first, second) * HP;
                res = flag ? O2H : O1H;
                out << H.x << " " << H.y << "\n";
                out << res << " " << HP << "\n";
                out << interFirst.x << " " << interFirst.y << "\n";
                out << interSecond.x << " " << interSecond.y << "\n";
            }
        }
        else {
            long double KL =  (firstRadius + secondRadius - centerDistance); //equals KH + LH
            long double O1HminusO2H = (firstRadius * firstRadius - secondRadius * secondRadius) / centerDistance;
            long double O1K = firstRadius - KL, O2L = secondRadius - KL;
            long double HLminusKH = O1K - O2L - O1HminusO2H;
            long double HL = (HLminusKH + KL) / 2;
            long double KH = KL - HL;
            long double O1H = firstRadius - HL;
            long double O2H = secondRadius - KH;
            HP = sqrt(firstRadius * firstRadius - O1H * O1H);
            Point2D H = first + normalize(first, second) * O1H;
            Point2D interFirst = H + normal(first, second) * HP;
            Point2D interSecond = H - normal(first, second) * HP;
            res = flag ? O2H : O1H;
            out << H.x << " " << H.y << "\n";
            out << res << " " << HP << "\n";
            out << interFirst.x << " " << interFirst.y << "\n";
            out << interSecond.x << " " << interSecond.y << "\n";
            return;
        }
    }
}

bool IsConvex(std::vector<Point2D> &polygon) {
    bool res = false;
    for(int i = 0; i < polygon.size(); ++i) {
        Point2D first = polygon[i % polygon.size()] - polygon[(i + 1) % polygon.size()];
        Point2D second = polygon[(i + 2) % polygon.size()] - polygon[(i + 1) % polygon.size()];
        if(dot(first, second) < 0 && res) {
            return false;
        }
        if(dot(first, second) > 0 && i != 0 && !res)
            return false;
        else if(dot(first, second) > 0 && i == 0){
            res = true;
        }
    }
    return true;
}

long double polygonArea(std::vector<Point2D> &polygon) {
    long double res = 0;
    for (int i = 0; i < polygon.size(); i++) {
        Point2D first = i ? polygon[i - 1] : polygon.back();
        Point2D second = polygon[i];
        res += (first.x - second.x) * (first.y + second.y);
    }
    return std::abs(res) / 2;
}

void tangentLine(long double centerX, long double centerY, long double radius, long double dotX, long double dotY) {
    std::cout.precision(20);
    out.precision(20);
    Point2D center = {centerX, centerY};
    Point2D dot = {dotX, dotY};
    long double distance = (dotX - centerX) * (dotX - centerX) + (dotY - centerY) * (dotY - centerY);
    if(distance == radius * radius) {
        //точка лежит на окружности
        out << 1 << "\n";
        out << dotX << " " << dotY;
        return;
    }
    else if(distance < radius * radius) {
        //vnutri 0 tochek~
        out << 0;
        return;
    }
    else {
        distance = sqrt(distance);
        //snaruzhi . 2 tochki
        out << 2 << "\n";
        Point2D p0 = center, p1 = dot;
        long double p0p3 = (radius * radius) / distance;
        long double p1p3 = distance - p0p3;
        long double p3p2 = sqrt(radius * radius - p0p3 * p0p3);
        Point2D p3 = p0 + normalize(p0, p1) * p0p3;
        Point2D p2 = p3 + normal(p0, p1) * p3p2;
        Point2D p4 = p3 - normal(p0, p1) * p3p2;
        out << p3.x << " " << p3.y << "\n";
        out << p1p3 << " " << p3p2 << "\n";
        out << p4.x << " " << p4.y << "\n";
        out << p2.x << " " << p2.y << "\n";
        
    }
}














bool cross(Point2D a, Point2D b, Point2D c) {
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) < 0;
}

bool antiCross(Point2D a, Point2D b, Point2D c) {
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y) > 0;
}

Point2D pivot;


bool polarOrder(Point2D &first, Point2D &second) {
    int sign;
    long double cross = dot(first - pivot, second - pivot);
    if(cross > 0)
        sign = -1;
    else if(cross < 0)
        sign = 1;
    else
        sign = 0;
    if(sign == 0)
        return (first - pivot).length() < (second - pivot).length();
    else if(sign == 1)
        return false;
    else
        return true;
    
}



int poIliprotiv(Point2D &first, Point2D &piv, Point2D &second) {
    long double cross =  dot(first - piv, second - piv);
    int sign;
    if(cross > 0)
        sign = -1;
    else if(cross < 0)
        sign = 1;
    else
        sign = 0;
    return sign;
}



//samuyu nizhnyuyu
int nizhe(std::vector<Point2D> &points) {
    Point2D nizhe = {INT64_MAX, INT64_MAX};
    int index = 0;
    for(int i = 0; i < points.size(); ++i) {
        if(points[i].y < nizhe.y || (points[i].y == nizhe.y && points[i].x < nizhe.x)) {
            nizhe = points[i];
            index = i;
        }
    }
    return index;
}

void convexHull(std::vector<Point2D> &points) {
    if (points.size() == 1) {
        std::cout << 1 << "\n";
        std::cout << points.front().x << " " << points.front().y << "\n";
        return;
    }
    if(points.size() == 2) {
        if(points.front() == points.back()) {
            std::cout << 1 << "\n";
            std::cout << points.front().x << " " << points.front().y << "\n";
        }
        std::cout << 2 << "\n";
        std::cout << points.front().x << " " << points.front().y << "\n";
        std::cout << points.back().x << " " << points.back().y << "\n";
        return;
    }
    
    int index = nizhe(points);
    pivot = points[index];
    std::sort(points.begin(), points.end(), &polarOrder);
    Point2D first = points[0],  second = points[1];
    std::vector<Point2D> ans;
    ans.push_back(first);
    ans.push_back(second);
    int position = 2;
    for(position; position < points.size(); ++position) {
        while(ans.size() >= 2 && poIliprotiv(ans[ans.size() - 2], ans[ans.size() - 1], points[position]) <= 0) {
            ans.pop_back();
        }
        if(!(points[position % points.size()] == ans.front()))
            ans.push_back(points[position % points.size()]);
    }
    std::cout << ans.size() << "\n";
    for(auto elem : ans) {
        long long a = elem.x;
        long long b = elem.y;
        printf("%Ld %Ld\n", a , b);
        //out << elem.x << " " << elem.y << "\n";
    }
}


enum EventKind { BEGIN = 0, END = 1 };

struct Event {
    long long x;
    long long radius;
    long long position;
    long long eventKind;
    
    bool operator <(const Event &other) const {
        return
            x < other.x
        || (x == other.x && eventKind > other.eventKind)
        || (x == other.x && eventKind == other.eventKind && radius > other.radius);
    }
    
    Event(long long x, long long radius, long long position, long long eventKind) : x(x), radius(radius), position(position), eventKind(eventKind) {}
};

struct Circle {
    long long x;
    long long y;
    long long radius;
    long long position;
    
    //
    bool operator <(const Circle &other) const {
        return
            (y < other.y
        || y == other.y && radius < other.radius
        || y == other.y && radius == other.radius && position < other.position);
    }
    
    Circle(long long x, long long y, long long radius, long long position) : x(x), y(y), radius(radius), position(position) {}
};

void CircleArea(std::vector<Circle> &circles, std::vector<Event> &events) {
    std::vector<char> visitedCircles(circles.size(), false);
    std::set<Circle> orderedCircles;
    std::sort(events.begin(), events.end());
    long long answer = 0;
    for(auto event : events) {
        auto currentCircle = circles[event.position];
        if(event.eventKind == EventKind::END)
            orderedCircles.erase(currentCircle);
        else {
            if(!orderedCircles.empty()){
                auto nearbyCircle = orderedCircles.lower_bound(currentCircle);
                
                for(int j = 0; j < 20; ++j) {
                    if(nearbyCircle == orderedCircles.end()) {
                        --nearbyCircle;
                        break;
                    }
                    ++nearbyCircle;
                }
                long long nearCircleSquaredRadius = (long long)(nearbyCircle -> radius) * (long long)(nearbyCircle -> radius);
                for(int j = 0; j < 40; ++j) {
                    if((nearbyCircle -> x - currentCircle.x) * (nearbyCircle -> x - currentCircle.x) + (nearbyCircle -> y - currentCircle.y) * (nearbyCircle -> y - currentCircle.y) <= (nearbyCircle -> radius) * (nearbyCircle -> radius)) {
                        visitedCircles[event.position] = true;
                        break;
                    }
                    if(nearbyCircle == orderedCircles.begin())
                        break;
                    --nearbyCircle;
                }
            }
            if (!visitedCircles[event.position]) {
                orderedCircles.insert(currentCircle);
                answer += event.radius * event.radius;
            }
        }
    }
    std::cout << answer * M_PI << "\n";
}



long double angle(Point2D dot) {
    dot = dot * (1. / dot.length());
    if(dot.x >= 0 && dot.y >= 0)
        return asin(dot.y);
    else if(dot.x >= 0 && dot.y <= 0)
        return 2 * M_PI + asin(dot.y);
    else if(dot.x <= 0 && dot.y >= 0)
        return acos(dot.x);
    else if(dot.x <= 0 && dot.y <= 0)
        return M_PI + acos(-dot.x);
    else
        return 0;
}

bool comparer(const Point2D &first, const Point2D &second) {
    return angle(first) < angle(second);
}

void weekend(std::vector<Point2D> &firstPoligon, std::vector<Point2D> &secondPoligon) {
    std::vector<Point2D> firstArr, secondArr;
    Point2D first = {0, 0}, second = {0, 0}, third = {0, 0};
    for(auto dot : firstPoligon)
        first = first + dot;
    for(auto dot : secondPoligon)
        second = second + dot;
    first = first * (1. / firstPoligon.size());
    second = second * (1. / secondPoligon.size());
    for(int i = 0; i < firstPoligon.size(); ++i)
        firstArr.push_back(firstPoligon[(i + 1) % firstPoligon.size()] - firstPoligon[i]);
    
    for(int i = 0; i < secondPoligon.size(); ++i)
        secondArr.push_back(secondPoligon[(i + 1) % secondPoligon.size()] - secondPoligon[i]);
    std::sort(firstArr.begin(), firstArr.end(), &comparer);
    std::sort(secondArr.begin(), secondArr.end(), &comparer);
    std::vector<Point2D> polygon(firstArr.size() + secondArr.size());
    std::merge(firstArr.begin(), firstArr.end(), secondArr.begin(), secondArr.end(), polygon.begin(), &comparer);
    std::vector<Point2D> sum;
    Point2D currentDot = {0, 0};
    for(auto dot : polygon) {
        sum.push_back(currentDot);
        currentDot = currentDot + dot;
    }
    for(auto dot : sum)
        third = third + dot;
    third = third * (1. / sum.size());
    Point2D diff = first * (-1) - second - third;
    for(auto &dot : sum)
        dot = dot + diff;
    //finding min;
    long double minDist = INT64_MAX;
    int minimalIndex;
    for(int i = 0; i < sum.size(); ++i) {
        long double distance = distanceUPD({0, 0}, sum[i], sum[(i + 1) % sum.size()]);
        if(distance < minDist) {
            minDist = distance;
            minimalIndex = i;
        }
    }
    Point2D minimal;
    long double firstCoeff = sum[minimalIndex].y - sum[(minimalIndex + 1) % sum.size()].y;
    long double secondCoeff = sum[(minimalIndex + 1) % sum.size()].x - sum[minimalIndex].x;
    Point2D minVector = {firstCoeff, secondCoeff};
    long double toNormalize = sqrt(firstCoeff * firstCoeff + secondCoeff * secondCoeff);
    minimal = minVector * (minDist / (2 *toNormalize));
    for(auto &dot : secondPoligon)
        dot = dot * (-1);
    
    std::vector<std::pair<long double, int>> firstDistances;
    std::vector<std::pair<long double, int>> secondDistances;
    for(int i = 0; i < firstPoligon.size(); ++i) {
        firstDistances.push_back({std::abs(scalar(minimal, firstPoligon[i])),i});
    }
    for(int i = 0; i < secondPoligon.size(); ++i) {
        secondDistances.push_back({std::abs(scalar(minimal, secondPoligon[i])),i});
    }
    std::sort(firstDistances.begin(), firstDistances.end());
    std::sort(secondDistances.begin(), secondDistances.end());
    
    long double normalizedFirstCoeff = firstCoeff / toNormalize;
    long double normalizedSecondCoeff = secondCoeff / toNormalize;
    //теперь рассмотрим элементы с мин дистанцией
    std::vector<Point2D> minimalDistances(8);
    minimalDistances[0] = firstPoligon[firstDistances[0].second] + minimal;
    minimalDistances[1] = firstPoligon[firstDistances[0].second] - minimal;
    minimalDistances[2] = firstPoligon[firstDistances[firstDistances.size() - 1].second] + minimal;
    minimalDistances[3] = firstPoligon[firstDistances[firstDistances.size() - 1].second] - minimal;
    minimalDistances[4] = secondPoligon[secondDistances[0].second] + minimal;
    minimalDistances[5] = secondPoligon[secondDistances[0].second] - minimal;
    minimalDistances[6] = secondPoligon[secondDistances[secondDistances.size() - 1].second] + minimal;
    minimalDistances[7] = secondPoligon[secondDistances[secondDistances.size() - 1].second] - minimal;
    
    std::vector<long double> resultingThirdCoeff;
    int answerIndex = -1;
    bool check = false;
    for(int i = 0; i < minimalDistances.size(); ++i) {
        check = true;
        resultingThirdCoeff.push_back(minimalDistances[i].x * (-normalizedFirstCoeff) + minimalDistances[i].y * (-normalizedSecondCoeff));
        
        if(std::abs(normalizedFirstCoeff * firstPoligon[0].x + normalizedSecondCoeff * firstPoligon[0].y + resultingThirdCoeff[i]) < 1e-12) {
            check = false;
            continue;
        }
        
        long double sign = normalizedFirstCoeff * firstPoligon[0].x + normalizedSecondCoeff * firstPoligon[0].y + resultingThirdCoeff[i] > 0 ? 1 : -1;
        
        for(int j = 0; j < firstPoligon.size(); ++j) {
            long double currentFirstCoeff = normalizedFirstCoeff * firstPoligon[j].x;
            long double currentSecondCoeff = normalizedSecondCoeff * firstPoligon[j].y;
            long double currentCoeff = resultingThirdCoeff[i] + currentFirstCoeff + currentSecondCoeff;
            if(std::abs(currentCoeff) < 1e-12 || currentCoeff * sign < 0) {
                check = false;
                break;
            }
        }
        if(check == false)
            continue;
        
        //analogichno dlya vtorogo poligona
        if(std::abs(normalizedFirstCoeff * secondPoligon[0].x + normalizedSecondCoeff * secondPoligon[0].y + resultingThirdCoeff[i]) < 1e-12) {
            check = false;
            continue;
        }
    
        long double signToCompare = sign;
        sign = normalizedFirstCoeff * secondPoligon[0].x + normalizedSecondCoeff * secondPoligon[0].y + resultingThirdCoeff[i] > 0 ? 1 : -1;
        if(std::abs(sign - signToCompare) < 1e-12){
            check = false;
            continue;
        }
        
        for(int j = 0; j < secondPoligon.size(); ++j) {
            long double currentCoeff = normalizedFirstCoeff * secondPoligon[j].x + normalizedSecondCoeff * secondPoligon[j].y + resultingThirdCoeff[i];
            if(std::abs(currentCoeff) < 1e-12 || currentCoeff * sign < 0) {
                check = false;
                break;
            }

        }
        
        if(check) {
            answerIndex = i;
            break;
        }
    }
    
    std::cout << normalizedFirstCoeff << " " << normalizedSecondCoeff << " " << resultingThirdCoeff[answerIndex];
    
}

int main(int argc, const char * argv[]) {

    
    //    std::ios_base::sync_with_stdio(false);
    std::cout.precision(20);
    //    std::ifstream in;
//    std::ofstream out;
    
    
    freopen("circles.in", "r", stdin);
    freopen("circles.out", "w", stdout);
//////
    // insert code here...
    long double dotX, dotY;
    long double startX, startY, endX, endY;
//    in >> dotX >> dotY >> startX >> startY >> endX >> endY;
//    out << std::setprecision(10) << distance(dotX, dotY, startX, startY, endX, endY);
    long double firstA,  firstB,  firstC,  secondA,  secondB, secondC;
//    in >> firstA >> firstB >> firstC >> secondA >> secondB >> secondC;
//    auto ans = segmentIntersection(firstA, firstB, firstC, secondA, secondB, secondC);
//    out << std::setprecision(10) << ans.x << " " << ans.y;
    long double x1, x2, y1, y2, a1, a2, b1, b2;
//    in >> x1 >> y1 >> x2 >> y2 >> a1 >> b1 >> a2 >> b2;
//    if(hasSegmentIntersection(x1, y1, x2, y2, a1, b1, a2, b2))
//       out << "YES";
//    else
//       out << "NO";
    
    long double cenX, cenY, rad, doX, doY;
//    in >> cenX >> cenY >> rad >> doX >> doY;
//    tangentLine(cenX, cenY, rad, doX, doY);
    int testNumber;
//    in >> testNumber;
//    long long double firstX,  firstY,  firstRadius,  secondX,  secondY,  secondRadius;
//    for(int i = 0; i < testNumber; ++i) {
//        in >> firstX >> firstY >> firstRadius >> secondX >> secondY >> secondRadius;
//        roundIntersection(firstX, firstY, firstRadius, secondX, secondY, secondRadius);
//    }

//    std::cin >> x1 >> y1 >> x2 >> y2 >> a1 >> b1 >> a2 >> b2;
//    if(hasSegmentIntersection(x1, y1, x2, y2, a1, b1, a2, b2)) {
//        std::cout << "Yes";
//    }
//    else
//        std::cout << "No";
//    std::cout << ans.x << " " << ans.y;
    //    std::cout << first;
//    int numberOfEdges;
//    std::cin >> numberOfEdges;
//    scanf("%d", &numberOfEdges);
//    std::vector<Point2D> polygon;
    
    
    
    //Sum Minkovsogo A + (-B)
    int firstSize, secondSize;
//    std::vector<Point2D> firstPoligon, secondPoligon;
//    scanf("%d", &firstSize);
////    std::cin >> firstSize;
//    for(int i = 0; i < firstSize; ++i) {
//        long double x, y;
//        scanf("%Lf %Lf", &x, &y);
////        std::cin >> x >> y;
//        firstPoligon.push_back({x, y});
//    }
//    std::cin >> secondSize;
//    for(int i = 0; i < secondSize; ++i) {
//        long double x, y;
//        scanf("%Lf %Lf", &x, &y);
////        std::cin >> x >> y;
//        secondPoligon.push_back({-x, -y});
//    }
//    
//    weekend(firstPoligon, secondPoligon);
//    
    
    int numberOfCircles;
    std::cin >> numberOfCircles;
    std::vector<Circle> circles;
    std::vector<Event> events;
    for(int i = 0; i < numberOfCircles; ++i) {
        long long x, y;
        long long radius;
        std::cin >> x >> y >> radius;
//        scanf("%Lf %Lf", &x , &y);
        circles.push_back({x, y, radius, i});
        events.push_back({x - radius, radius, i, EventKind::BEGIN});
        events.push_back({x + radius, radius, i, EventKind::END});
    }
    CircleArea(circles, events);
//    std::cout << CircleArea(circles, events);
    
    
    
//    std::set<Point2D> setina(points.begin(), points.end());
//    std::vector<Point2D> poents(setina.begin(), setina.end());
//    convexHull(poents);
//    convex_hull(poents);
    
//    std::cout << points.size() << "\n";
//    if(points.size() % 2 == 1)
//        std::reverse(points.begin(), points.end());
//    for(auto elem : points) {
//        std::cout << elem.x << " " << elem.y << "\n";
//    }
//    out << polygonArea(polygon);
//    if(IsConvex(polygon))
//        out << "YES";
//    else
//        out << "NO";
//    std::cout << first.x << " " << first.y << " dd " << second.x << " " << second.y << "\n";
//    std::cout << (second - first).x << " " << (second - first).y << "\n";
//    std::cout << dot(second - first, main - first) << "\n";
   
   
    
    
    
    
    
    /*
     4
     2 2 2
     2 2 1
     5 2 1
     5 5 2
    */
    return 0;
}
