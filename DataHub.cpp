#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "force_patch.h"
#include "kill_patch.h"
using namespace std;

//This Program only will communicate with running Probes A,B, and C.
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
	B_RunFlag = true;
	C_RunFlag = true;

	


	while (A_RunFlag || B_RunFlag || C_RunFlag) {

		//***********ProbeA*************************
		//Run message receive and send only if ProbeA is Still running
		if (A_RunFlag) {
						cout << "Receiving Probe A" << endl;

			msgrcv(qid, (struct msgbuf *)&msg, size, 211, 0);

			cout << "ProbeA PID: " << msg.PID << "- Message: " << msg.greeting << endl;
			msg.mtype = 311;
			ProbeBCounter++;	

			//Send confirm rcv message to ProbeA
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			if (msg.termination) {
				//change ProbeA_Active flag to false
				A_RunFlag = false;
				cout << "ProbeA PID:" << msg.PID << "- now exits" << endl;
			}
		}


		//***********Probe B************************
		//Run message receive and send only if ProbeB is Still running
		if (B_RunFlag) {
			cout << "Receiving Probe B" << endl;
			msgrcv(qid, (struct msgbuf *)&msg, size, 167, 0);

			cout << "ProbeB PID: " << msg.PID << "- Message: " << msg.greeting << endl;
		
			ProbeBCounter++;

			//ProbeB termination
			if (ProbeBCounter >= 10000) {
				//change ProbeB_Active flag to false
				B_RunFlag = false;
				force_patch(msg.PID);
				cout << "ProbeB PID:" << msg.PID << "- now exits" << endl;
			}
			
				
		}

		// //***********Probe C************************
		if (C_RunFlag) {
			cout << "Receiving Probe C" << endl;
			msgrcv(qid, (struct msgbuf *)&msg, size, 123, 0);

			cout << "ProbeC PID: " << msg.PID << "- Message: " << msg.greeting << endl;
		
			ProbeBCounter++;

			if (msg.greeting == 100) {
			//change ProbeA_Active flag to false
			C_RunFlag = false;
			cout << "ProbeC PID:" << msg.PID << "- now exits" << endl;
			}
		}
		
			

		
		

		
	}
    
	
	
    // now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);
	cout << "DataHub now exits" << endl;
	exit(0);
    
}
