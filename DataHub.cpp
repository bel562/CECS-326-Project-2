#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "force_patch.h"
using namespace std;

int main(){
   // create my msgQ with key value from ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype; // required
		char greeting[50]; // mesg content
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	boolean check = true; 
	//We need to handle all probe messages
	while (check) {
		msgrcv(qid, (struct msgbuf *)&msg, size, 211, 0);
		cout << "ProbeA" << msg.greeting << endl;
		if (msg.greeting.compare("ProbeA terminated") == 0) {
			
		}
	}
     // reading
	cout << getpid() << ": (PID) Message Received" << endl;
	cout << "reply: " << msg.greeting << endl;
	
    // now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
    
}
