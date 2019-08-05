//
// Created by ben on 8/4/19.
//

#include "Scheduler.h"


IO_Request* FIFO::get_next_request(list <IO_Request *> &IO_ready_queue) {
    IO_Request *request = IO_ready_queue.front();
    IO_ready_queue.pop_front();
    return request;
}


IO_Request *SSTF::get_next_request(list<IO_Request *> &IO_ready_queue) {

}

IO_Request *LOOK::get_next_request(list<IO_Request *> &IO_ready_queue) {

}

IO_Request *CLOOK::get_next_request(list<IO_Request *> &IO_ready_queue) {

}

IO_Request *FLOOK::get_next_request(list<IO_Request *> &IO_ready_queue) {

}