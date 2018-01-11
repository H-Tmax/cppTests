#include "EventTask.h"

int main() {

    Task *t = new Task();

    //Cannot instantiate an abstract class
    //Event *e = new Event('e');

    EventA *eA = new EventA('a');
    EventB *eB = new EventB('b');
    EventC *eC = new EventC('c');
    Event *parent_A = new EventA('a');
    Event *parent_B = new EventB('b');
    Event *parent_C = new EventC('c');

    eA->handle(t);
    eB->handle(t);
    eC->handle(t);

    //Polymorphic behavior as Event is an abstract class
    parent_A->handle(t);
    parent_B->handle(t);
    parent_C->handle(t);


    return 0;
}