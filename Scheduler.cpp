//
// Created by ben on 8/4/19.
//

#include <cstdlib>
#include <cstdio>
#include "Scheduler.h"


Scheduler::Scheduler() {
    active_request = nullptr;
    CURRENT_TRACK = 0;
    head_direction = 0;
    change_direction = true;
}



IO_Request* FIFO::get_next_request(list <IO_Request *> &IO_ready_queue) {
    IO_Request *request = IO_ready_queue.front();
    IO_ready_queue.pop_front();
    return request;
}


IO_Request *SSTF::get_next_request(list<IO_Request *> &IO_ready_queue) {
    IO_Request *request = IO_ready_queue.front();
    list <IO_Request *>::iterator iter;
//    list <IO_Request *>::iterator temp_iter = IO_ready_queue.begin();
    for (iter=IO_ready_queue.begin(); iter!=IO_ready_queue.end(); ++iter) {
        int current_seek_time = request->track_num - CURRENT_TRACK;
        int new_seek_time = (*iter)->track_num - CURRENT_TRACK;
        if (abs(new_seek_time) < abs(current_seek_time) && !request->is_finished) {
            request = *iter;
//            temp_iter = iter;
        }
    }
//    IO_ready_queue.erase(temp_iter);
    IO_ready_queue.remove(request);
    return request;
}

IO_Request *LOOK::get_next_request(list<IO_Request *> &IO_ready_queue) {
    seek_options.clear();
    bool initiate_break = false;

    list<IO_Request *>::iterator iter;
    IO_Request *request = IO_ready_queue.front();

    if (head_direction==0) {
        if (request->track_num < CURRENT_TRACK) {
            head_direction = -1;
        }
        else if (request->track_num > CURRENT_TRACK) {
            head_direction = 1;
        }
    }
    if (q_option) {
        printf("Get: (");
    }
    int current_seek_time = 1000000;
    for (int i=0; i<2; i++) {

        if (head_direction == -1) {
            list<IO_Request *>::iterator iter;
            for (iter = IO_ready_queue.begin(); iter != IO_ready_queue.end(); ++iter) {
                if ((*iter)->track_num <= CURRENT_TRACK) {
                    change_direction = false;
//                    current_seek_time = request->track_num - CURRENT_TRACK;
                    int new_seek_time = (*iter)->track_num - CURRENT_TRACK;
                    if (q_option) {
                            printf("%d:%d ", (*iter)->IO_num, abs(new_seek_time));
                        }
                    if (abs(new_seek_time) < abs(current_seek_time) && !request->is_finished) {
                        request = *iter;
                        current_seek_time = new_seek_time;
                    }
                    initiate_break = true;
                }
            }
        }
        else if (head_direction == 1) {
            list<IO_Request *>::iterator iter;
            for (iter = IO_ready_queue.begin(); iter != IO_ready_queue.end(); ++iter) {
                if ((*iter)->track_num >= CURRENT_TRACK) {
                    change_direction = false;
                    int new_seek_time = (*iter)->track_num - CURRENT_TRACK;
                    if (q_option) {
                        printf("%d:%d ", (*iter)->IO_num, abs(new_seek_time));
                    }
                    if (abs(new_seek_time) < abs(current_seek_time) && !request->is_finished) {
                        request = *iter;
                        current_seek_time = new_seek_time;
                    }
                    initiate_break = true;
                }
            }
        }
        if (initiate_break) {
            break;
        }
        if (change_direction) {
            head_direction = head_direction * -1;
//            if (q_option) {
//                printf("Get: () --> change direction to %d\n", head_direction);
//            }
        }
    }
    IO_ready_queue.remove(request);
    if (q_option) {
        printf(") --> %d dir=%d\n", request->IO_num, head_direction);
    }
    if (q_option && change_direction) {
        printf("Get: () --> change direction to %d\n", head_direction);
    }
    change_direction = true;
    return request;
}

IO_Request *CLOOK::get_next_request(list<IO_Request *> &IO_ready_queue) {
    IO_Request *request = IO_ready_queue.front();
    if (q_option) {
        printf("Get: (");
    }
    int current_seek_time = 1000000;
    head_direction = 1;
    bool start_over = true;
    if (head_direction == 1) {
        list<IO_Request *>::iterator iter;
        for (iter = IO_ready_queue.begin(); iter != IO_ready_queue.end(); ++iter) {
            if ((*iter)->track_num >= CURRENT_TRACK) {
                change_direction = false;
                int new_seek_time = (*iter)->track_num - CURRENT_TRACK;
                if (q_option) {
                    printf("%d:%d ", (*iter)->IO_num, abs(new_seek_time));
                }
                if (abs(new_seek_time) < abs(current_seek_time) && !request->is_finished) {
                    request = *iter;
                    current_seek_time = new_seek_time;
                }
                start_over = false;
            }
        }
    }
    if (start_over) {
        current_seek_time = 0;
        list<IO_Request *>::iterator iter;
        for (iter = IO_ready_queue.begin(); iter != IO_ready_queue.end(); ++iter) {
            if ((*iter)->track_num <= CURRENT_TRACK) {
                change_direction = false;
//                    current_seek_time = request->track_num - CURRENT_TRACK;
                int new_seek_time = (*iter)->track_num - CURRENT_TRACK;
                if (q_option) {
                    printf("%d:%d ", (*iter)->IO_num, abs(new_seek_time));
                }
                if (abs(new_seek_time) > abs(current_seek_time) && !request->is_finished) {
                    request = *iter;
                    current_seek_time = new_seek_time;
                }
            }
        }
    }
    if (q_option) {
        printf(") --> %d\n", request->IO_num);
    }
    IO_ready_queue.remove(request);
    return request;
}

IO_Request *FLOOK::get_next_request(list<IO_Request *> &IO_ready_queue) {

}