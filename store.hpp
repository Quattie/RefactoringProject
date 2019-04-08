//
//  store.hpp
//  Refactoring
//
//  Created by Alexander Sloan on 4/7/19.
//  Copyright © 2019 Alexander Sloan. All rights reserved.
//

#ifndef store_hpp
#define store_hpp

#include <string>
#include <vector>
#include <queue>
#include <array>
#include <stdio.h>
#include "event.hpp"

using namespace std;

class Store {
private:
    priority_queue<Event,vector<Event>,compareEventTime> eventQueue;
    array<double, 6> cashiers;
    double currentTime = 0;
public:
    void addEvent(Event event);
    void simulate();
    vector<double> serviceTimes;
};

void addEvent(Event event);

double smallestIndex(const array<double, 6> &array);

void simulate();

#endif /* store_hpp */
