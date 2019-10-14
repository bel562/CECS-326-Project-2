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
   int rho = 257;
	int randomNumber = 0;
	// create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), 0);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

	int mtype = 123;
   	while(true) {
        randomNumber = rand();
        if (randomNumber % rho == 0){
            strncpy(msg.greeting, randomNumber, 50);
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
    }
	

	cout << getpid() << ": now exits" << endl;



	exit(0);

}
