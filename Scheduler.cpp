//
// Created by ben on 8/4/19.
//

#include "Scheduler.h"

IO_Request *dequeue_request(list <IO_Request *> IO_queue) {
    IO_Request *request = IO_queue.front();
    IO_queue.pop_front();
    return request;
}
