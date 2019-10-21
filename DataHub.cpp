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
	pid_t probeA_PID, probeB_PID, probeC_PID;
	int ProbeBCounter = 0;
	
	//We need to handle all probe messages
	//MType:
	//ProbeA-211
	//ProbeB-167
	//ProbeC-123
	C_RunFlag = false;
	while (A_RunFlag || B_RunFlag || C_RunFlag) {
		//***********ProbeA*************************
		msgrcv(qid, (struct msgbuf *)&msg, size, 211, IPC_NOWAIT);
		cout << "ProbeA Debug: " << msg.greeting << endl;
		if (msg.mtype == 211){	probeA_PID =  getpid(); }

		//condition check if probe A sent a message
		if ( msg.greeting != (const char*)-1) {		
			cout << "ProbeA-" << probeA_PID + "-" << msg.greeting << endl;
			ProbeBCounter++;			
			//Send confirm rcv message to ProbeA
			strncpy(msg.greeting,  "Received", 50);
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			if (((string)msg.greeting).compare("ProbeA terminated") == 0) {
				//change ProbeA_Active flag to false
				A_RunFlag = false;
			}
		}


		//***********Probe B************************
		msgrcv(qid, (struct msgbuf *)&msg, size, 167, IPC_NOWAIT);
		cout << "ProbeB Debug: " << msg.greeting << endl;
		if (msg.mtype == 167){	probeB_PID =  getpid(); }

		//condition check if probe B sent a message
		if ( msg.greeting != (const char*) -1 ) {		
			cout << "ProbeB-" << probeB_PID + "-" << msg.greeting << endl;
			ProbeBCounter++;			
		}

		
		// //***********Probe C************************
		// msgrcv(qid, (struct msgbuf *)&msg, size, 123, IPC_NOWAIT);
		// cout << "ProbeC Debug: " << msg.greeting << endl;
		// if (msg.mtype == 123){	probeC_PID =  getpid(); }	

		// //condition check if probe B sent a message	
		// if ((int) msg.greeting != -1) {		
		// 	cout << "ProbeC-" << probeC_PID + "-" << msg.greeting << endl;
		// 	ProbeBCounter++;			
		// }

		//Probe B to be Terminated
		if (ProbeBCounter >= 10000) {
				cout << probeB_PID << ": now exits" << endl;
				force_patch(probeB_PID);
				//change ProbeB_Active flag to false
				B_RunFlag = false;
			}
	}
    
	
    // now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
    
}
