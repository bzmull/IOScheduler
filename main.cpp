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
//int CURRENT_TRACK = 0;

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
void print_function(list <IO_Request *> finished_list);
bool lists_empty(Scheduler *scheduler, bool do_FLOOK);



int main(int argc, char **argv) {

    bool v_option = false;
    bool q_option = false;
    bool f_option = false;

    bool do_FLOOK = false;
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
                            do_FLOOK = true;
                            break;
                        }

                    }
                    break;
                }

                case 'v': {
                    v_option = true;
                    break;
                }
                case 'q': {
                    q_option = true;
                    break;
                }
                case 'f': {
                    f_option = true;
                    break;
                }
            }
        }

        file_path = argv[argc-1];
    }
    else {
        scheduler = new FLOOK();
        file_path = "./inputs/input0";
        v_option = true;
        q_option = false;
        f_option = false;
    }

    int IO_num = 0;
    list <IO_Request *> IO_list;
    char *input_line;
    FILE *inputfile = fopen(file_path.c_str(), "r");
    while (input_line = get_nextline(inputfile)) {
        IO_Request *new_request = new IO_Request(0,0);
        sscanf(input_line, "%d %d", &new_request->arrive_time, &new_request->track_num);
        new_request->IO_num = IO_num++;
        IO_list.push_back(new_request);
    }


/*
   SIMULATION
*/

    if (v_option) {
        printf("TRACE\n");
    }
    scheduler->q_option = q_option;

    double num_requests = IO_list.size();
    list <IO_Request *> finished_list = IO_list;

    while ( !IO_list.empty() || !lists_empty(scheduler, do_FLOOK) || scheduler->active_request != nullptr) {

//#1 --> add to ready queue
        if (!IO_list.empty()) {
            if (CURRENT_TIME == IO_list.front()->arrive_time) {
                IO_Request *request = IO_list.front();
                IO_list.pop_front();
                scheduler->IO_ready_queue.push_back(request);
                if(v_option) {
                    printf("%d:%6d add %d\n", CURRENT_TIME, request->IO_num, request->track_num);
                }
            }
        }

        if (scheduler->active_request == nullptr && !lists_empty(scheduler, do_FLOOK)) {
            scheduler->active_request = scheduler->get_next_request(scheduler->IO_ready_queue);
            scheduler->active_request->start_time = CURRENT_TIME;
//            if (q_option && do_FLOOK) {
//                list <IO_Request *>::iterator iter1;
//                list <IO_Request *>::iterator iter2;
//                printf("ready_queue --> ");
//                for (iter1=scheduler->IO_ready_queue.begin(); iter1!=scheduler->IO_ready_queue.end(); ++iter1) {
//                    printf("%d:%d ", (*iter1)->IO_num, (*iter1)->track_num);
//                }
//                printf("\nadd_queue --> ");
//                for (iter2=scheduler->IO_add_queue.begin(); iter2!=scheduler->IO_add_queue.end(); ++iter2) {
//                    printf("%d:%d ", (*iter2)->IO_num, (*iter2)->track_num);
//                }
//                printf("\n");
//            }
            if (f_option) {
                printf("%d get Q=%d\n", scheduler->active_request->IO_num, scheduler->which_queue);
            }
            if (v_option) {
                printf("%d:%6d issue %d %d\n", CURRENT_TIME, scheduler->active_request->IO_num,
                       scheduler->active_request->track_num, scheduler->CURRENT_TRACK);
            }
        }
//        total_time = CURRENT_TIME;
//        CURRENT_TIME++;

        if (scheduler->active_request != nullptr) {

//#2 --> finished request
            if (scheduler->active_request->track_num == scheduler->CURRENT_TRACK) {
                scheduler->active_request->end_time = CURRENT_TIME;
                save_request_for_summary(scheduler->active_request, IO_list);
                if (v_option) {
                    printf("%d:%6d finish %.0f\n", CURRENT_TIME, scheduler->active_request->IO_num,
                           scheduler->active_request->turnaround_time);
//                    printf("CURRENT TRACK = <%d>\n READY QUEUE = <", scheduler->CURRENT_TRACK);
//                    list<IO_Request *>::iterator iter;
//                    for (iter=finished_list.begin(); iter!=finished_list.end(); ++iter) {
//                        printf("%d, ", (*iter)->IO_num);
//                    }
//                    printf(">\n");
                }
                scheduler->active_request->is_finished = true;
                scheduler->active_request = nullptr;
                continue;
            }

//#3 --> moving head to necessary track
            else {
                move_disk_head(scheduler->CURRENT_TRACK, scheduler->active_request->track_num);
            }
        }

//#4 --> make new active request
//        if (scheduler->active_request == nullptr && !scheduler->IO_ready_queue.empty()) {
//            scheduler->active_request = scheduler->get_next_request(scheduler->IO_ready_queue);
//            scheduler->active_request->start_time = CURRENT_TIME;
//            if (v_option) {
//                printf("%d:%6d issue %d %d\n", CURRENT_TIME, scheduler->active_request->IO_num,
//                        scheduler->active_request->track_num, scheduler->CURRENT_TRACK);
//            }
//        }
//        total_time = CURRENT_TIME;
        CURRENT_TIME++;
        total_time = CURRENT_TIME;
    }

    compute_summary_stats(num_requests);
    print_function(finished_list);


    return 0;
}



bool lists_empty(Scheduler *scheduler, bool do_FLOOK) {
    if (do_FLOOK) {
        return scheduler->IO_ready_queue.empty() && scheduler->IO_add_queue.empty();
    }
    else {
        return scheduler->IO_ready_queue.empty();
    }
}



void print_function(list <IO_Request *> finished_list) {
    int IO_num = 0;
    list <IO_Request *>::iterator iter;

    for (iter=finished_list.begin(); iter!=finished_list.end(); ++iter) {
        printf("%5d: %5d %5d %5d\n", IO_num++, (*iter)->arrive_time,
                (*iter)->start_time, (*iter)->end_time);
    }

    printf("SUM: %d %d %.2lf %.2lf %d\n",
           total_time, tot_movement, avg_turnaround, avg_waittime, max_waittime);
}



char* get_nextline(FILE *&inputfile)
{
    char* rstr;
    while (1) {
        rstr = fgets(linebuf,4096,inputfile);
        if (rstr== nullptr)
            return nullptr;
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