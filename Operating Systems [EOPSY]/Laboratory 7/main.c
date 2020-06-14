#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include<stdbool.h>

#define N 5
#define LEFT ( i + N - 1 ) % N
#define RIGHT ( i + 1 ) % N
#define THINKING 1
#define HUNGRY 2
#define EATING 3
#define FOOD_LIMIT 3
#define EAT_TIME 3
#define THINK_TIME 3


pthread_mutex_t m; //the internal mutex
pthread_mutex_t s[N]; //array of all the philosopher
pthread_t p[N]; //list of thread ids of philosophers
int state[N]; //current philosopher status (hungry, eating, thinking)

void test(int i)
{
    if( state[i] == HUNGRY
            && state[RIGHT] != EATING
            && state[LEFT] != EATING ) //if a phil is hungry and  both phils to his left and right are not eating, that means that we can eat
    {
        state[i] = EATING;
        pthread_mutex_unlock( &s[i] ); //unlock the phil mutex
    }
}
void grab_forks(int i) {
    pthread_mutex_lock( &m ); //we lock the internal mutex (to restrict the other threads from running
    state[i] = HUNGRY;
    test( i );
    pthread_mutex_unlock( &m );//we unlock the internal mutex after we're done
    pthread_mutex_lock( &s[i] ); //we lock the mutex id of the philosopher

}

void put_away_forks( int i ) {
    pthread_mutex_lock( &m );//we lock the internal mutex (to restrict the other threads from running
    state[i] = THINKING;
    test( LEFT );
    test( RIGHT );
    pthread_mutex_unlock( &m ); //we unlock the internal mutex after we're done


}

void eat(int id){
    grab_forks(id);
    printf("Philosopher [%d] is eating \n", id);
    sleep(EAT_TIME);
    put_away_forks(id);
}

void think(int id){
    printf("Philosopher [%d] is thinking \n", id);
    state[id] = THINKING;
    sleep(THINK_TIME);

}

void *philosopherLifecycle(int philosopherId){
    int foodLeft = FOOD_LIMIT; // each philosopher have to eat same amount of food
    bool hungry = false; //iniially all philosophers starts from thinking
    printf("Philosopher [%d] joined the table \n", philosopherId);
    while(foodLeft) {
        if(hungry){

            eat(philosopherId);
            foodLeft--;
            hungry = !hungry; //change state to fasle

        } else {
            think(philosopherId);
            hungry = !hungry; //change state to true
        }
    }
    printf("Philosopher [%d] left the table. \n", philosopherId);
}








int main() {
    printf("Start of the dinner: \n");
    pthread_mutex_init(&m, NULL); //initializing m, if null given, default mutex atributes are used.
                                  //if success the state of mutex becomes initialised and unlocked


    for(int i = 0; i < N; i++) {
        state[i] = THINKING; //giving initial state for all philosophers to thinking.
        pthread_mutex_init(&s[i], NULL); //initializing mutex for philosphers
//        pthread_mutex_lock(&s[i]);  // locking initlially.

//        creating threads for philosophers, new thread starting routine of 3rd argument, argument to routine are passed after comma.

        if(pthread_create( &p[i], NULL, (void *)philosopherLifecycle, (void*)i ) != 0)
        {
            perror("Creating thread failure");
            exit(1);
        }
    }

    for (int i = 0; i < N; i++)
    {
        if(pthread_join(p[i], NULL)!=0) //wait for finish all of threads.
        {
            perror("Thread join failure");
            exit(1);
        }

    }

    for (int i = 0 ; i < N ; i++)
    {
        pthread_mutex_destroy(&s[i]);//destroy all mutex at the end
    }
    printf("End of the program\n");

    return 0;
}

//side notes:
//  mutex - program object allowing multiple program threads to share the same resources
//          but not simultaneously
//
// 1. NO, mutex can be operated by multiple processes. It would be necessary to put the mutex in
//       shared memory
// 2. Internal mutex is put to unlock because its locked only when philosophers do some action,
//    at begining they are thinking so no action regarding eating is done.
//      Philosophers mutex are locked initially to prevent all of them starting eating the same time.

