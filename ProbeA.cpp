

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
    int alpha = 997;
	int randomNumber = 101;
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		int greeting; // mesg content
		char termMessage[50]; //messege for termination
		int PID;
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);
	msg.PID = getpid();
    msg.mtype = 211;
	msg.greeting = randomNumber;
	strncpy(msg.termMessage, "ProbeA Running", 50);	
	
	
    while (randomNumber >= 100){
        randomNumber = rand();
        if (randomNumber % alpha == 0){
            msg.greeting = randomNumber;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			//Receive Message From Datahub
			msgrcv(qid, (struct msgbuf *)&msg, size, 311, 0);
			cout << "help" << endl;
        }
    }
	//Send ProbeA Finished to Datahub
	strncpy(msg.termMessage, "ProbeA terminated", 50);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	

	cout << getpid() << ": now exits" << endl;

	exit(0);
}

