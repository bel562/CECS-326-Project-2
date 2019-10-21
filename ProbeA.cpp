

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
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

    msg.mtype = 211;
    while (randomNumber >= 100){
        randomNumber = rand();
        if (randomNumber % alpha == 0){
            msg.greeting = randomNumber;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << ": now sending " << msg.greeting << endl;

			//Receive Message From Datahub
			msgrcv(qid, (struct msgbuf *)&msg, size, 311, 0);
		
        }
    }
	//Send ProbeA Finished to Datahub
	//strncpy(msg.greeting, "ProbeA terminated", 50);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	

	cout << getpid() << ": now exits" << endl;

	exit(0);
}

