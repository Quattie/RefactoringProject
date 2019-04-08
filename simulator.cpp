#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <array>
#include <iomanip>

#include "bank.hpp"
#include "store.hpp"


using namespace std;

double getRandomNumber(double fMin, double fMax){
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
        double st = getRandomNumber(0, maxCustServTime);
        if (getRandomNumber(0, 100) < chance) {
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
