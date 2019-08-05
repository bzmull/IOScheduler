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

    virtual IO_Request* get_next_request(list <IO_Request *> &IO_ready_queue) = 0;
};


class FIFO : public Scheduler {
public:

    IO_Request* get_next_request(list <IO_Request *> &IO_ready_queue);
};


class SSTF : public Scheduler{
public:
    IO_Request* get_next_request(list <IO_Request *> &IO_ready_queue);
};



class LOOK : public Scheduler{
public:
    IO_Request* get_next_request(list <IO_Request *> &IO_ready_queue) ;
};


class CLOOK : public Scheduler{
public:
    IO_Request* get_next_request(list <IO_Request *> &IO_ready_queue) ;
};


class FLOOK : public Scheduler{
public:
    IO_Request* get_next_request(list <IO_Request *> &IO_ready_queue) ;
};


#endif //IOSCHEDULER_SCHEDULER_H
