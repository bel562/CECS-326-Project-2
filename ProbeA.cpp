/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program A will use a message queue created by Program B. 
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by anyone.

Both child processes use message type mtype = 113 and 114.

*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

int main() {
    long alpha = 9000000;

	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

    msg.mtype = 211;
    while (msg.mtype >= 100){
        msg.mtype = rand();
        if (alpha % msg.mtype != 0){
            strncpy(msg.greeting, "Probe A Hello", 15);
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
    }
	

	cout << getpid() << ": now exits" << endl;

	exit(0);
}

