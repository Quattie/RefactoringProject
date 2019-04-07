
#ifndef event_hpp
#define event_hpp

const int simLength = 43200;

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

#endif
