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
	
    int rho = 6353;
	int randomNumber = 0;
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		int greeting; // mesg content
		int PID;
		bool termination; //messege for termination
		int ProbeNo;
	};
	

	buf msg;
	msg.PID = getpid();
	int size = sizeof(msg)-sizeof(long);
	msg.termination = false;
	msg.mtype = 123;
	msg.ProbeNo = 3;


	buf msg2;
	msg2.PID = getpid();
	msg2.ProbeNo = 0;
	msg2.greeting = 100;
	int size2 = sizeof(msg2)-sizeof(long);
	msg2.termination = true;
	msg2.mtype = 123;
	
   	while(true) {
        randomNumber = rand();
        if (randomNumber % rho == 0){
            msg.greeting = randomNumber;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
		
		kill_patch(qid,(struct msgbuf *)&msg2,  size2, msg2.mtype);
		
    }
	


}

