#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;

ofstream pfile;
ofstream cfile;
int MAX_VALUE=1000000;

typedef struct { 
    volatile int pIndex; //volatile avoid performance degradation
    volatile int cIndex; 
    int qSize;
    int * circularQueue;
 } ThreadArgs;
 
void * producerRoutine(void * args){
    ThreadArgs * tArgs = (ThreadArgs *) args;
    int pValue=0;
  
        while(true){
            int condValue = (tArgs->pIndex +1) & (tArgs->qSize-1);
            if (condValue==tArgs->cIndex){ //if true queue is full
                usleep(1);            
            }else{

                tArgs->circularQueue[tArgs->pIndex]=pValue;
                pfile << tArgs->circularQueue[tArgs->pIndex] << endl;
                pValue++;
                tArgs->pIndex=condValue;
                if (pValue==MAX_VALUE)
                   pthread_exit(NULL);
            }
        }
}

void * consumerRoutine(void * args){
    ThreadArgs * tArgs = (ThreadArgs *) args;

    while(true){
        
        if (tArgs->cIndex==tArgs->pIndex){ //if true queue is empty
            usleep(1);            
        }else{
            int value=tArgs->circularQueue[tArgs->cIndex];
            cfile << value << endl;
            tArgs->cIndex = (tArgs->cIndex+1) & (tArgs->qSize-1);
        }
    }
}

int main(){

    cout<<"Running. Please wait.."<<endl;

    pthread_t producer;
    pthread_t consumer;
    
    pfile.open("producer.txt");
    cfile.open("consumer.txt");

    ThreadArgs * sharedArgs = new ThreadArgs();
    sharedArgs->pIndex=0;
    sharedArgs->cIndex=0;
    sharedArgs->qSize=32; // must be a power of 2
    sharedArgs->circularQueue=new int[sharedArgs->qSize];

    pthread_create(&producer, NULL, &producerRoutine, sharedArgs);
    pthread_create(&consumer, NULL, &consumerRoutine, sharedArgs);

    pthread_join(producer,NULL);
    cout<<"Producer terminated, wait for 3 sec"<<endl;
    sleep(3);
    cout<<"Terminated."<<endl;
}