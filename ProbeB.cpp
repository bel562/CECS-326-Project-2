

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
    int beta = 500341;
	int randomNumber = 0;
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		int greeting; // mesg content
		int PID;
		bool termination; //messege for termination
	};

	buf msg;
	msg.mtype = 167;
	msg.PID = getpid();
	msg.termination = false;
	int size = sizeof(msg)-sizeof(long);
	
   	while(true) {
        randomNumber = rand();
        if (randomNumber % beta == 0){
            msg.greeting = randomNumber;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
    }
	
}

