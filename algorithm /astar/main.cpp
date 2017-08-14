//
//  Created by xijiajia on 2017/8/14.
//  Copyright © 2017年 xijiajia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

int points[5][5] = {{0,0,1,0,0},
                    {0,0,1,0,0},
                    {0,0,0,0,0},
                    {0,0,1,0,0}};

class Point {
public:
    int x = 0, y = 0;
    int f = 0, g = 0, h = 0;
    int flag = 0;
    Point* parent = nullptr;//注意赋值，后果产生野指针，影响路径判断
    
    Point(int _x, int _y) {
        x = _x;
        y = _y;
        flag = points[x][y];
    }
    
    void count(int cost, Point* end, Point* par) {
        int h_num = abs(x - end->x);
        int v_num = abs(y - end->y);
        parent = par;
        g = parent->g + cost;
        h = (h_num + v_num) * 10;
        f = g + h;
    }
};

Point* startPoint = new Point(0, 0);
Point* endPoint = new Point(0, 4);

//注意！如果存放的是对象erase时会自动销毁从而影响结果，如果存放的是指针不会自动销毁(自己手动销毁)
vector<Point*> openList;
vector<Point*> closeList;

bool comparePoints(Point* p1, Point* p2) {
    return p1->f < p2->f;
}

bool isInList(vector<Point*>list, int x, int y) {
    for (int i=0; i<list.size(); i++) {
        if (list[i]->x == x && list[i]->y == y) {
            return true;
        }
    }
    return false;
}

void findPath(Point* point) {
    int arr[3] = {-1, 0, 1};
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (i == 1 && j == 1)
                continue;
            int _x = point->x + arr[i];
            int _y = point->y + arr[j];
            if (_x>=0 && _x<5 && _y>=0 && _y<5) {
                //注意：这里还有一种情况，考虑权重，next point 当前点到达指next point的G值比next point目前的G值小，需要修改next point的G值，同时parent指向当前point
                if (points[_x][_y] == 1 || isInList(openList, _x, _y) || isInList(closeList, _x, _y)) {
                    continue;
                }
                Point* p = new Point(_x, _y);
                int cost = 15;
                if (arr[i] == 0 || arr[j] == 0)
                    cost = 10;
                p->count(cost, endPoint, point);
                openList.push_back(p);
            }
        }
    }
    if (openList.empty()) {
        printf("not found endPoint!\n");
        return;
    }
    if (isInList(openList, endPoint->x, endPoint->y)) {
        printf("find endPoint! (%d,%d)", endPoint->x, endPoint->y);
        while (point) {
            printf("-->(%d,%d)", point->x, point->y);
            point = point->parent;
        }
        printf("\n");
    } else {
        closeList.push_back(point);
        for (int i=0; i<openList.size(); i++) {
            if (point->x == openList[i]->x && point->y == openList[i]->y) {
                openList.erase(openList.begin() + i);
                break;
            }
        }
        sort(openList.begin(), openList.end(), comparePoints);
        findPath(openList[0]);
    }
}

int main() {
    findPath(startPoint);
    return 0;
}
