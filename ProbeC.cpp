#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;

int main(){
    int rho = 257;
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

   	while(true) {
        randomNumber = rand();
        if (randomNumber % rho == 0){
            msg.greeting = randomNumber;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
    }
	


}
