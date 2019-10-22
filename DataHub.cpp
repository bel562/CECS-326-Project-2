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
		int greeting; // mesg content
		int PID;
		char termMessage[50]; //messege for termination
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	

	bool A_RunFlag, B_RunFlag, C_RunFlag; 
	pid_t probeA_PID, probeB_PID, probeC_PID;
	int ProbeBCounter = 0;
	
	//We need to handle all probe messages
	//MType:
	//ProbeA-211
	//ProbeB-167
	//ProbeC-123
	A_RunFlag = true;
	B_RunFlag = true;
	C_RunFlag = false;

	
	//--------Syncing-------
	//Wait for B
	msgrcv(qid, (struct msgbuf *)&msg, size, 167, 0);
	probeB_PID =  msg.PID;

	//Wait for A	
	msgrcv(qid, (struct msgbuf *)&msg, size, 211, 0);	
	probeA_PID =  msg.PID;

	//Give A and B 'the Go'
	msg.mtype = 312;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	msg.mtype = 311;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);


	while (A_RunFlag || B_RunFlag || C_RunFlag) {

		//***********ProbeA*************************
		msgrcv(qid, (struct msgbuf *)&msg, size, 211, IPC_NOWAIT);
		cout << "ProbeA Debug: " << msg.greeting << endl;

		//condition check if probe A sent a message
		if ( msg.greeting != -1 && msg.PID == probeA_PID) {		
			
			cout << "ProbeA PID: " << probeA_PID << "- Message: " << msg.greeting << endl;

			ProbeBCounter++;			
			//Send confirm rcv message to ProbeA
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			if (((string)msg.termMessage).compare("ProbeA terminated") == 0) {
				//change ProbeA_Active flag to false
				A_RunFlag = false;
			}
		}


		//***********Probe B************************
		msgrcv(qid, (struct msgbuf *)&msg, size, 167, IPC_NOWAIT);
		cout << "ProbeB Debug: " << msg.greeting << endl;
		probeB_PID =  getpid();

		//condition check if probe B sent a message
		if ( msg.greeting !=  -1  && msg.PID == probeB_PID) {	
			cout << "ProbeB PID: " << probeB_PID << "- Message: " << msg.greeting << endl;
			ProbeBCounter++;			
		}

		
		// //***********Probe C************************
		// msgrcv(qid, (struct msgbuf *)&msg, size, 123, IPC_NOWAIT);
		// cout << "ProbeC Debug: " << msg.greeting << endl;

		// //condition check if probe B sent a message	
		// if ( msg.greeting != -1) {
		// probeC_PID =  msg.PID;		
		// 	cout << "ProbeC-" << probeC_PID + "-" << msg.greeting << endl;
		// 	ProbeBCounter++;			
		// }

		//Probe B to be Terminated
		if (ProbeBCounter >= 10000) {
				cout << "ProbeB PID: " << probeB_PID << "- now exits" << endl;
				force_patch(probeB_PID);
				//change ProbeB_Active flag to false
				B_RunFlag = false;
			}
	}
    
	
    // now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
    
}
