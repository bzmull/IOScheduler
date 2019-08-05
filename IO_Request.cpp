//
// Created by ben on 8/4/19.
//

#include "IO_Request.h"

IO_Request::IO_Request(int arrive_time, int track_num) {
    this->arrive_time = arrive_time;
    this->track_num = track_num;
    start_time = 0;
    end_time = 0;
    turnaround_time = 0;
    wait_time = 0;
}