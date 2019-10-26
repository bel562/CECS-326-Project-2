

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
		int PID;
		bool termination; //messege for termination
		
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);	
	msg.termination = false;
	
	
    while (randomNumber >= 100){
        randomNumber = rand();
        if (randomNumber % alpha == 0){
			msg.mtype = 211;
            msg.greeting = randomNumber;
			msg.PID = getpid();

            msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			//Receive Message From Datahub
			msgrcv(qid, (struct msgbuf *)&msg, size, 311, 0);
			cout << "help" << endl;
        }
    }

	//Send ProbeA Finished to Datahub
	msg.mtype = 211;
	msg.PID = getpid();
	msg.termination = true; 

	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	//Receive Message From Datahub
	msgrcv(qid, (struct msgbuf *)&msg, size, 311, 0);
	

	cout << "ProbeA PID:" << getpid() << "- now exits" << endl;

	exit(0);
}

