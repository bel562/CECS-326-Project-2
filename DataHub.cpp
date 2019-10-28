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
		int ProbeNo = 4;
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
			
		
		cout << "Receiving Probe" << endl;

		msgrcv(qid, (struct msgbuf *)&msg, size, -300, 0);

		
		msg.mtype = 311;
		ProbeBCounter++;

		if (msg.ProbeNo == 1){
			cout << msg.ProbeNo << " PID: " << msg.PID << "- Message: " << msg.greeting << endl;
			//Send confirm rcv message to ProbeA
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);

			if (msg.termination) {
				//change ProbeA_Active flag to false
				A_RunFlag = false;
				cout << "ProbeA PID:" << msg.PID << "- now exits" << endl;
			}
		}
		else if(msg.ProbeNo == 2){
			cout << msg.ProbeNo << " PID: " << msg.PID << "- Message: " << msg.greeting << endl;
			if (ProbeBCounter >= 10000) {
				//change ProbeB_Active flag to false
				B_RunFlag = false;
				force_patch(msg.PID);
				cout << "ProbeB PID:" << msg.PID << "- now exits" << endl;
			}
		}
		else if(msg.ProbeNo == 3){
			cout << msg.ProbeNo << " PID: " << msg.PID << "- Message: " << msg.greeting << endl;
			
		}
		else if (msg.ProbeNo == 0 && msg.greeting == 100) {
				//change ProbeC_Active flag to false
				C_RunFlag = false;
				cout << "ProbeC PID:" << msg.PID << "- now exits" << endl;
		}
		
		
		
		

		
	}
    
	
	
    // now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);
	cout << "DataHub now exits" << endl;
	exit(0);
    
}
