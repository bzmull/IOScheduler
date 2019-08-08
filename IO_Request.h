//
// Created by ben on 8/4/19.
//

#ifndef IOSCHEDULER_IO_REQUEST_H
#define IOSCHEDULER_IO_REQUEST_H


class IO_Request {
public:
    int IO_num;
    int arrive_time;
    int track_num;
    int start_time;
    int end_time;
    double turnaround_time;
    double wait_time;
    bool is_finished;

////summary stats
//    int total_time;
//    int tot_movement;
//    int avg_turnaround;
//    int avg_waittime;
//    int max_waittime;

    IO_Request(int arrive_time, int track_num);
};


#endif //IOSCHEDULER_IO_REQUEST_H
