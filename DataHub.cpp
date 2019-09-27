#include <string>
#include <list>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(){
    key_t key;
    key = ftok("progfile", 65);
    
}