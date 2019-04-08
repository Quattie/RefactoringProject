//
//  store.cpp
//  Refactoring
//
//  Created by Alexander Sloan on 4/7/19.
//  Copyright © 2019 Alexander Sloan. All rights reserved.
//

#include "store.hpp"

void Store::addEvent(Event event) {
    eventQueue.push(event);
}

double smallestIndex(const array<double, 6> &array) {
    int index = 0;
    for(int i = 1; i < array.size(); i++) {
        if(array.at(index) > array.at(i))
            index = i;
    }
    return index;
}

// run the simulation
void Store::simulate() {
    while (!eventQueue.empty()) {
        Event nextEvent = eventQueue.top();
        currentTime = nextEvent.eventTime;
        if (currentTime > simLength) {
            break;
        }
        eventQueue.pop();
        int shortestLine;
        switch (nextEvent.event) {
            case ARRIVAL:
                shortestLine = smallestIndex(cashiers);
                nextEvent.line = shortestLine;
                nextEvent.totalServiceTime = cashiers.at(shortestLine) + nextEvent.duration;
                cashiers.at(shortestLine) += nextEvent.duration;
                nextEvent.eventTime = currentTime + cashiers.at(shortestLine);
                nextEvent.event = DEPARTURE;
                addEvent(nextEvent);
                break;
            case DEPARTURE:
                cashiers.at(nextEvent.line) -= nextEvent.duration;
                serviceTimes.push_back(nextEvent.totalServiceTime);
                break;
        }
    }
}
