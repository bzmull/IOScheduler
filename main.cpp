#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "IO_Request.h"
#include "Scheduler.h"


using namespace std;


char linebuf[4096];
int line_num = 0;
int CURRENT_TIME = 0;
int CURRENT_TRACK = 0;


char* get_nextline(FILE *&inputfile);



int main() {

    list <IO_Request *> IO_list;

    char *input_line;
    string file_path = "./lab4_testdata/TestData/input9";
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

    //PSEUDO CODE
    IO_Request *active_request = nullptr;
    while ( !IO_list.empty() && !IO_ready_queue.empty() ) {

        if (CURRENT_TIME == IO_list.front()->arrive_time) {         //#1 in istructions
            IO_Request *request = IO_list.front();
            push into IO_ready_queue based on algorithm.
        }
        if (active_request != nullptr) {
            if (active_request->track_num == CURRENT_TRACK) {       //#2 in istructions
                save_request_for_summary();
                active_request = nullptr;
            }
            else {                                                  //#3 in istructions
                move_disk_head();
            }
        }
        if (active_request == nullptr && !IO_ready_queue.empty()) { //#4 in istructions
            active_request = get_next_request();        //this function is in the scheduler class
        }

        CURRENT_TIME++;
    }


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
