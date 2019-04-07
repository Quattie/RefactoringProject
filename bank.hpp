//
//  bank.hpp
//  Refactoring
//
//  Created by Alexander Sloan on 4/7/19.
//  Copyright © 2019 Alexander Sloan. All rights reserved.
//

#ifndef bank_hpp
#define bank_hpp

#include <string>
#include <vector>
#include <queue>
#include <array>
#include <stdio.h>
#include "event.hpp"

using namespace std;

class Bank {
private:
    priority_queue<Event,vector<Event>,compareEventTime> eventQueue;
    int tellersAvailable = 6;
    queue<Event> bankQueue;
    double currentTime = 0;
    
public:
    void addEvent(Event & event);
    void addBankQueueCustomer(Event & event);
    void simulate();
    vector<double> serviceTimes;
};

void addEvent(Event & event);

void addBankQueueCustomer(Event & event);

void simulate();

#endif /* bank_hpp */
