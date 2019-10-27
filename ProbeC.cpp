#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "kill_patch.h"
using namespace std;


int main(){
    int rho = 251;
	long randomNumber = 0;
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
	msg.PID = getpid();
	int size = sizeof(msg)-sizeof(long);
	msg.termination = false;
	msg.mtype = 123;
	string statement;
   	while(true) {
        randomNumber = rand();
        if (randomNumber % rho == 0){
            msg.greeting = randomNumber;
			cout <<"about to send" << endl;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << "sent" << endl;
        }
		
    }
	


}

