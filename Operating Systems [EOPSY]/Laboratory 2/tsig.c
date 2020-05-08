#include <stdio.h>
#include <signal.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHILD 10
#define WITH_SIGNALS

#ifdef WITH_SIGNALS
char interrupt_flag = 0;

//method called when interruption occurs
void keyboardInterrupt() {
    printf("parent [%i]: The keyboard interrupt received.\n",getpid());
    interrupt_flag = 1;
}


void killedChild() {
    printf("child [%i]: Termination of the process.", getpid());
}

#endif

//Method for creating child process
void createChildProcess(){
    pid_t childPid = 0;
    if(!(childPid=fork())){
        printf("Child [%i]: Child was born.\n",getpid());
        sleep(10);
        printf("Child [%i]: Child finished task.\n",getpid());
        exit(0);
    }else if(childPid == -1){
#ifdef WITH_SIGNALS
        //        if interrupt occurs call kiledChild method which prints info about killed process.
        signal (SIGINT,killedChild);
#endif
        printf("Parent [%i]: Error occurred");
        kill(-1,SIGTERM); // -1 is killing all processes
        exit(1);
    }

}




int main()
{
    int w;
    int processesCounter = 0;

    //    loop creating number of procesess defined by NUM_CHILD
    for(int i = 0; i < NUM_CHILD; i++){
#ifdef WITH_SIGNALS
        for(int j = 0; j < NSIG; j++){ //NSIG - number of signals
            sigignore(j);//The sigignore() function sets the disposition of sig to SIG_IGN.
        }
        signal (SIGCHLD, SIG_DFL); //SIG_DFL specifies the default disposition for the signal
        signal (SIGINT, keyboardInterrupt); //set keyboardInterrupt signal handlerr
#endif

        createChildProcess();
        sleep(1);

#ifdef WITH_SIGNALS
        if (interrupt_flag == 1){
            printf("parent[%i]: Interrupt of the creation process!\n", getppid());
            kill(-2, SIGTERM); //termination request sent to the program
            break;
        }
#endif
    }
    while(1){
        w = wait(NULL);
        if(w == -1) //if process has no child, w will be -1
            break;
        else{
            printf("child [%i]: I finished.\n",w);
            processesCounter++;
        }
    }
    printf("\nSuccess: %d processes were terminated.\n", processesCounter);

#ifdef WITH_SIGNALS
    //    restore all service handlers
    for(int j=0; j<NSIG;j++)
        signal(j,SIG_DFL);
#endif
    return 0;
}
