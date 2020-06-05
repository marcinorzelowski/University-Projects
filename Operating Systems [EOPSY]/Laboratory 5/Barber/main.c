#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdbool.h>
#define N1 1
#define N2 1
#define N3 1
#define M 10
#define MAX 10
#define KEY 2131

int barberN1;
int chairsID;
int chairs = M;

struct sembuf buf;

void upSem(int semid, int semnum){

    buf.sem_num = semnum;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1){
        perror("Semaphore up");
        exit(1);
    }
}
void downSem(int semid, int semnum){
    printf("SEMNUM: %d, PID: %d \n", semnum, getpid());
    buf.sem_num = semnum;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1){
        perror("Semaphore down.");
        exit(1);
    }
}
void barber(int id) {
    printf("Barber [%d]: Hightime to get some money!. \n", id);
    int chair;
    while(1){
        chair = getFirstChairOccupied();
        if(chair != -1){
            printf("Barber [%d]: I am cutting the client from chair %d \n", id, chair);
            sleep(rand()%3);

            printf("Barber [%id]: For sure client from chair no.%d have best haircut now! \n",id, chair);
            downSem(chairsID, chair);
        }
    }
}

void client(int id){
    printf("Client [%d]: Hello! \n", id);
    fflush(stdout);
    int chair = getFirstChairFree();
    if(chair != -1){
        printf("STATUS %d \n",semctl(chairsID, chair, GETVAL));
        upSem(chairsID, chair); //occupy chair
        printf("Client [%d]: I will sit on waiting place no.%d \n", id, chair);
        printf("STATUS AFTER: %d \n",semctl(chairsID, chair, GETVAL));
        fflush(stdout);
        while(semctl(chairsID, chair, GETVAL) == 2){
            //dummy loop
        }
        printf("Client [%d]: Goodbye!", id);
    }else {
        printf("Client [%d]: Ouuchhh... its to crowdy there... \n", id);
    }
}

int getFirstChairOccupied(){
    for(int i = 0; i<M; i++){
        if(semctl(chairsID, i, GETVAL) == 2){
            printf("CHAIR...%d", i);
            return i;
        }

    }
    return -1;
}

int getFirstChairFree(){
    for(int i = 0; i<M; i++){
        if(semctl(chairsID, i, GETVAL) == 1){
            return i;
        }
    }
    return -1;
}




int  main(){
    int pid;
    chairsID = semget(25423,M,IPC_CREAT|0600);
    if(barberN1 == -1 || chairsID == -1){
        perror("Semaphore error.");
        exit(1);
    }
    for(int i = 0; i<M; i++){
        if(semctl(chairsID, i, SETVAL, 1) == -1){
            perror("Semaphore initial value error");
            exit(1);
        }
    }


    for(int i = 0; i < N1; i++){
        pid = fork();
        if(pid == 0)
        {
            barber(getpid());
        }
    }

    for(int i=0; i<20; i++){
        pid = fork();
        if(pid == 0){
            client(getpid());
            break;
        }
        sleep(3);
    }





    return 0;



}
