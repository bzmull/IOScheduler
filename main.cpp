#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <getopt.h>

#include "IO_Request.h"
#include "Scheduler.h"


using namespace std;


char linebuf[4096];
int line_num = 0;
int CURRENT_TIME = 0;
int CURRENT_TRACK = 0;

//summary stats
int total_time = 0;
int tot_movement = 0;
double avg_turnaround = 0;
double avg_waittime = 0;
int max_waittime = 0;
double total_wait_time = 0;
double total_turnaround = 0;


char* get_nextline(FILE *&inputfile);
void save_request_for_summary(IO_Request *active_request, list <IO_Request *> IO_list);
void move_disk_head(int &current_track, int goal_track);
void compute_summary_stats(double num_requests);



int main(int argc, char **argv) {

    bool print_request_operations = false;
    bool print_head_movement = false;
    bool print_FLOOK_info = false;
    int arg_count = 1;

    string file_path;
    Scheduler *scheduler;

    int option;
    if (argc > 1) {
        while ((option = getopt (argc, argv, "s:vqf")) != -1) {
            switch (option) {
                case 's': {
                    switch (optarg[0]) {
                        case 'i': {
                            scheduler = new FIFO();
                            break;
                        }
                        case 'j': {
                            scheduler = new SSTF();
                            break;
                        }
                        case 's': {
                            scheduler = new LOOK();
                            break;
                        }
                        case 'c': {
                            scheduler = new CLOOK();
                            break;
                        }
                        case 'f': {
                            scheduler = new FLOOK();
                            break;
                        }

                    }
                    break;
                }

                case 'v': {
                    print_request_operations = true;
                    break;
                }
                case 'q': {
                    print_head_movement = true;
                    break;
                }
                case 'f': {
                    print_FLOOK_info = true;
                    break;
                }
            }
        }
        file_path = argv[arg_count];
    }
    else {
        scheduler = new FIFO();
        file_path = "./lab4_testdata/TestData/input9";
        print_request_operations = false;
        print_head_movement = false;
        print_FLOOK_info = false;
    }

    list <IO_Request *> IO_list;
    char *input_line;
    FILE *inputfile = fopen(file_path.c_str(), "r");
    while (input_line = get_nextline(inputfile)) {
        IO_Request *new_request = new IO_Request(0,0);
        sscanf(input_line, "%d %d", &new_request->arrive_time, &new_request->track_num);
        IO_list.push_back(new_request);
    }

////    print list of IOs to test:
//    printf("num requests = <%d>\n", IO_list.size());
//    int i = 1;
//    list<IO_Request*>::iterator iter;
//    for (iter = IO_list.begin(); iter!= IO_list.end(); ++iter) {
//        printf("IO %d: %d--%d\n", i++, (*iter)->arrive_time, (*iter)->track_num);
//    }

    //SIMULATION
//    Scheduler *scheduler = new FIFO();

    IO_Request *active_request = nullptr;

    double num_requests = IO_list.size();
    int IO_num = 0;

    while ( !IO_list.empty() || !scheduler->IO_ready_queue.empty() || active_request != nullptr) {

//#1 in istructions
        if (!IO_list.empty()) {
            if (CURRENT_TIME == IO_list.front()->arrive_time) {
                IO_Request *request = IO_list.front();
                IO_list.pop_front();
                scheduler->IO_ready_queue.push_back(request);
            }
        }
        if (active_request != nullptr) {
//#2 in istructions
            if (active_request->track_num == CURRENT_TRACK) {
                active_request->end_time = --CURRENT_TIME;
                printf("%5d: %5d %5d %5d\n", IO_num++, active_request->arrive_time,
                        active_request->start_time, active_request->end_time);
                save_request_for_summary(active_request, IO_list);
                active_request = nullptr;
            }
//#3 in istructions
            else {
                move_disk_head(CURRENT_TRACK, active_request->track_num);
            }
        }
//#4 in istructions
        if (active_request == nullptr && !scheduler->IO_ready_queue.empty()) {
            active_request = scheduler->get_next_request(scheduler->IO_ready_queue);
            active_request->start_time = CURRENT_TIME;
        }
        total_time = CURRENT_TIME;
        CURRENT_TIME++;
    }

    compute_summary_stats(num_requests);
    printf("SUM: %d %d %.2lf %.2lf %d\n",
           total_time, tot_movement, avg_turnaround, avg_waittime, max_waittime);


    return 0;
}



char* get_nextline(FILE *&inputfile)
{
    char* rstr;
    while (1) {
        rstr = fgets(linebuf,4096,inputfile);
        if (rstr==NULL)
            return NULL;
        line_num++;
        if (*rstr == '#')
            continue;
        return rstr;
    }
}


void save_request_for_summary(IO_Request *active_request, list <IO_Request *> IO_list) {

    active_request->turnaround_time = active_request->end_time - active_request->arrive_time;
    total_turnaround += active_request->turnaround_time;

    active_request->wait_time = active_request->start_time - active_request->arrive_time;
    if (active_request->wait_time > max_waittime) {
        max_waittime = active_request->wait_time;
    }
    total_wait_time += active_request->wait_time;
}


void compute_summary_stats(double num_requests) {
    avg_waittime = total_wait_time / num_requests;
    avg_turnaround = total_turnaround / num_requests;
}


void move_disk_head(int &current_track, int goal_track) {
    if (current_track < goal_track) {
        current_track++;
    }
    else if (current_track > goal_track) {
        current_track--;
    }
    tot_movement++;
}