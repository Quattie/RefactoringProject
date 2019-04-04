#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <array>
#include <iomanip>

using namespace std;

enum EventType {
    ARRIVAL, DEPARTURE
};

typedef struct {
    double eventTime;
    double duration;
    int line;
    double waitTime;
    double totalServiceTime = 0;
    EventType event;

} Event;

struct compareEventTime {
    bool operator()(const Event &lhs, const Event &rhs) const {
        return lhs.eventTime > rhs.eventTime;
    }
};

const int simLength = 43200;

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

double fRand(double fMin, double fMax){
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

double printPercentiles(vector<double> &waitTimes, string name) {

    sort(waitTimes.begin(), waitTimes.end());

    int bank10th = waitTimes.size() * .1;
    int bank50th = waitTimes.size() * .5;
    int bank90th = waitTimes.size() * .9;

    double bank10 = waitTimes.at(bank10th);
    double bank50 = waitTimes.at(bank50th);
    double bank90 = waitTimes.at(bank90th);

    double average;

    for (double d : waitTimes) {
        average += d;
    }
    average = average / waitTimes.size();

    std::cout << name << " service times in minutes: \n";
    cout << "10th %ile " << setprecision(3) << bank10 << "\n";
    cout << "50th %ile " << setprecision(3) << bank50 << "\n";
    cout << "90th %ile " << setprecision(3) << bank90 << "\n";
    cout << "Average Time: " << setprecision(3) << average << endl;

    return bank90;
}


int main(int argc, char *argv[]) {

    //Customers arriving per minute
    double arrivalRate = atof(argv[1]);
    double maxCustServTime = atof(argv[2]);
    unsigned int seed = stoi(argv[3]);
    assert(arrivalRate && maxCustServTime && seed > 0);
    if (argc != 4) {
        cout << "Parameters are ArrivalRate MaxCustomerServiceTime Seed" << endl;
        exit(0);
    }
    srand(seed);

    Bank bankSim;
    Store storeSim;

    //Gets the percentage chance of a customer coming every second
    double chance = arrivalRate / 60;
    chance *= 100;

    for (int i = 0; i < simLength; i++) {
        double st = fRand(0, maxCustServTime);
        if (fRand(0, 100) < chance) {
            Event myEvent;
            myEvent.duration = st;
            myEvent.eventTime = i / 60;
            myEvent.event = ARRIVAL;
            bankSim.addEvent(myEvent);
            storeSim.addEvent(myEvent);
        }
    }

    bankSim.simulate();
    storeSim.simulate();

    int customerCount = (arrivalRate * 60) * 12;
    cout << "Forcasted Total Customers: " << customerCount << endl;
    cout << "Simulated Total Customers: " << bankSim.serviceTimes.size() << endl;

    double bank90 = printPercentiles(bankSim.serviceTimes, "Bank");
    double store90 = printPercentiles(storeSim.serviceTimes, "Super Market");

//    cout << bank90 << "\t" << store90 << endl;
    return 0;
}
