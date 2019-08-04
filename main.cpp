#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "IO_Request.h"


using namespace std;


char linebuf[4096];
int line_num = 0;
int CURRENT_TIME = 1;


char* get_nextline(FILE *&inputfile);



int main() {

    list <IO_Request *> IO_list;

    char *input_line;
    string file_path = "./lab4_testdata/TestData/input0";
    FILE *inputfile = fopen(file_path.c_str(), "r");
    while (input_line = get_nextline(inputfile)) {
        IO_Request *new_request = new IO_Request(0,0);
        sscanf(input_line, "%d %d", &new_request->arrive_time, &new_request->track_num);
        IO_list.push_back(new_request);
    }

    printf("num requests = <%d>\n", IO_list.size());

    int i = 1;
    list<IO_Request*>::iterator iter;
    for (iter = IO_list.begin(); iter!= IO_list.end(); ++iter) {
        printf("IO %d: %d--%d\n", i++, (*iter)->arrive_time, (*iter)->track_num);
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
