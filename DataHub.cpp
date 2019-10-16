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
	msg.mtype = 311;
	bool A_RunFlag, B_RunFlag, C_RunFlag = true; 
	
	//We need to handle all probe messages
	while (A_RunFlag || B_RunFlag || C_RunFlag) {
		//***********ProbeA*************************
		msgrcv(qid, (struct msgbuf *)&msg, size, 211, IPC_NOWAIT);
		cout << "ProbeA Debug: " << msg.greeting << endl;
		//condition check if probe A sent a message
		if ((int) msg.greeting != -1) {		
			cout << "ProbeA" << msg.greeting << endl;
			//Send confirm rcv message to ProbeA
			strncpy(msg.greeting,  "Received", 50);
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			if (((string)msg.greeting).compare("ProbeA terminated") == 0) {
				//change ProbeA_Active flag to false
				A_RunFlag = false;
			}
		}
		//***********Probe B********************************
		// msgrcv(qid, (struct msgbuf *)&msg, size, 211, IPC_NOWAIT);
		// cout << "ProbeB Debug: " << msg.greeting << endl;
		// //condition check if probe A sent a message
		// if ((int) msg.greeting != -1) {		
		// 	cout << "ProbeB" << msg.greeting << endl;
	}
    
	
    // now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
    
}
