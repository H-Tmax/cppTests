#ifndef DOUBLEDISPATCHTEST_EVENTTASK_H
#define DOUBLEDISPATCHTEST_EVENTTASK_H

#include <iostream>

class Event;
class EventA;
class EventB;
class EventC;

class Task {
public:
    void handleEvent(EventA *eA){
        std::cout << "Handling Event A" << std::endl;
        //Event handling logic comes here
    }
    void handleEvent(EventB *eB){
        std::cout << "Handling Event B" << std::endl;
        //Event handling logic comes here
    }
    void handleEvent(EventC *eC){
        std::cout << "Handling Event C" << std::endl;
        //Event handling logic comes here
    }
    //THIS SHOULD'T BE CALLED AS EVENT IS AN ABSTRACT CLASS
    void handleEvent(Event *e){
        std::cout << "Handling Default Event" << std::endl;
        //Event handling logic comes here
    }
};

class Event {
public:
    virtual void handle(Task *t) = 0;
protected:
    Event(char _type) : type(_type){}
    char type = 0;
};

class EventA : public Event {
public:
    EventA(char _type) : Event(_type){}
    void handle(Task *t){
        t->handleEvent(this);
    };

};

class EventB : public Event {
public:
    EventB(char _type) : Event(_type){}
    void handle(Task *t){
        t->handleEvent(this);
    };
};

class EventC : public Event {
public:
    EventC(char _type) : Event(_type){}
    void handle(Task *t){
        t->handleEvent(this);
    };
};







#endif //DOUBLEDISPATCHTEST_EVENTTASK_H
