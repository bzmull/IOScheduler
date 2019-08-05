//
// Created by ben on 8/4/19.
//

#ifndef IOSCHEDULER_SCHEDULER_H
#define IOSCHEDULER_SCHEDULER_H

#include "IO_Request.h"

#include <list>

using namespace std;

class Scheduler {
public:
    list <IO_Request *> IO_ready_queue;

    virtual IO_Request* dequeue_request(list <IO_Request *> IO_queue);
    virtual void insert_request(list <IO_Request *> IO_queue, IO_Request *request);
};


class FIFO : Scheduler{

};


class SSTF : Scheduler{

};



class LOOK : Scheduler{

};


class CLOOK : Scheduler{

};


class FLOOK : Scheduler{

};


#endif //IOSCHEDULER_SCHEDULER_H
