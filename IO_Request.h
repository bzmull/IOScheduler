//
// Created by ben on 8/4/19.
//

#ifndef IOSCHEDULER_IO_REQUEST_H
#define IOSCHEDULER_IO_REQUEST_H


class IO_Request {
public:
    int arrive_time;
    int track_num;

    IO_Request(int arrive_time, int track_num);
};


#endif //IOSCHEDULER_IO_REQUEST_H
