

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
    long beta = 800000;
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

	
   while(true) {
        msg.mtype = rand();
        if (beta % msg.mtype != 0){
            strncpy(msg.greeting, "Probe B Hello", 15);
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
    }
	

	cout << getpid() << ": now exits" << endl;



	exit(0);
}

