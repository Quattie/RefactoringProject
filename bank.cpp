//
//  bank.cpp
//  Refactoring
//
//  Created by Alexander Sloan on 4/7/19.
//  Copyright © 2019 Alexander Sloan. All rights reserved.
//

#include "bank.hpp"

void Bank::addEvent(Event & event) {
    eventQueue.push(event);
}

void Bank::addBankQueueCustomer(Event & event) {
    bankQueue.push(event);
    event.waitTime = currentTime;
}

void Bank::simulate() {
    while (!eventQueue.empty()) {
        Event nextEvent = eventQueue.top();
        currentTime = nextEvent.eventTime;
        if (currentTime > simLength) {
            break;
        }
        eventQueue.pop();
        switch (nextEvent.event) {
            case ARRIVAL:
                if (tellersAvailable) {
                    nextEvent.totalServiceTime = nextEvent.duration;
                    nextEvent.eventTime = currentTime + nextEvent.duration;
                    nextEvent.event = DEPARTURE;
                    addEvent(nextEvent);
                    tellersAvailable--;
                } else {
                    addBankQueueCustomer(nextEvent);
                }
                break;
            case DEPARTURE:
                if(!bankQueue.empty()) {
                    Event nextCustomer = bankQueue.front();
                    bankQueue.pop();
                    
                    nextCustomer.totalServiceTime = currentTime - nextCustomer.eventTime + nextCustomer.duration;
                    serviceTimes.push_back(nextEvent.totalServiceTime);
                    
                    nextCustomer.eventTime = currentTime + nextEvent.duration;
                    nextCustomer.event = DEPARTURE;
                    addEvent(nextCustomer);
                } else {
                    serviceTimes.push_back(nextEvent.totalServiceTime);
                    tellersAvailable++;
                }
                break;
        }
    }
}

