#include <string>
#include <list>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(){
    int qid = msgget(ftok(".",'u'),0);
    struct buf {
        long mtype;
        char greeting[50];

    };
    buf msg;
    int font_size = sizeof(msg) - sizeof(long);
    msg.mtype = 111;
    strncpy(msg.greeting,"Fake message");

}