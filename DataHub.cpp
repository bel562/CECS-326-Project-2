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
   // create my msgQ with key value ipfrom ftok()
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	// declare my message buffer
	struct buf {
		long mtype; // required
		int greeting; // mesg content
		int PID;
		bool termination; //messege for termination
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	msg.mtype = 311;
	msg.termination = false;

	bool A_RunFlag, B_RunFlag, C_RunFlag; 
	pid_t probeA_PID, probeB_PID, probeC_PID;
	int ProbeBCounter = 0;
	
	//We need to handle all probe messages
	//MType:
	//ProbeA-211
	//ProbeB-167
	//ProbeC-123
	//DataHub-311
	A_RunFlag = true;
	B_RunFlag = false;
	C_RunFlag = false;

	


	while (!msg.termination) {

		//***********ProbeA*************************
		if (A_RunFlag) {
			msgrcv(qid, (struct msgbuf *)&msg, size, 211, 0);

			cout << "ProbeA PID: " << msg.PID << "- Message: " << msg.greeting << endl;
			msg.mtype = 311;
			ProbeBCounter++;	

			//Send confirm rcv message to ProbeA
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			if (msg.termination) {
				//change ProbeA_Active flag to false
				A_RunFlag = false;
			}
		}


		// //***********Probe B************************
		// msgrcv(qid, (struct msgbuf *)&msg, size, 167, IPC_NOWAIT);
		// cout << "ProbeB Debug: " << msg.greeting << endl;
		// probeB_PID =  getpid();

		// //condition check if probe B sent a message
		// if ( msg.greeting !=  -1  && msg.PID == probeB_PID) {	
		// 	cout << "ProbeB PID: " << probeB_PID << "- Message: " << msg.greeting << endl;
		// 	ProbeBCounter++;			
		// }

		
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
		// if (ProbeBCounter >= 10000) {
		// 		cout << "ProbeB PID: " << probeB_PID << "- now exits" << endl;
		// 		force_patch(probeB_PID);
		// 		//change ProbeB_Active flag to false
		// 		B_RunFlag = false;
		// 	}
	}
    
	
    // now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);
	cout << "DataHub now exits" << endl;
	exit(0);
    
}
