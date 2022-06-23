 #include <pthread.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
     
     


pthread_mutex_t queueMutex;
pthread_cond_t queueCond;
Queue queue;

void Initialize() {
    //Initialize the mutex and the condition variable
    pthread_mutex_init(&queueMutex, NULL);
    pthread_cond_init(&queueCond, NULL);
}

void Producer() {
    //First we get some new data
    Data *newData = MakeNewData();

    //Lock the queue mutex to make sure that adding data to the queue happens correctly
    pthread_mutex_lock(&queueMutex);

    //Push new data to the queue
    queue.push(newData);

    //Signal the condition variable that new data is available in the queue
    pthread_cond_signal(&queueCond);

    //Done, unlock the mutex
    pthread_mutex_unlock(&queueMutex);
}


void Consumer() {

    //Run the consumer loop
    while(1) {

        //Start by locking the queue mutex
        pthread_mutex_lock(&queueMutex);

        //As long as the queue is empty,
        while(queue.empty()) {
            // - wait for the condition variable to be signalled
            //Note: This call unlocks the mutex when called and
            //relocks it before returning!
            pthread_cond_wait(&queueCond, &queueMutex);
        }

        //As we returned from the call, there must be new data in the queue - get it,
        Data *newData = queue.front();
        // - and remove it from the queue
        queue.pop();

        //Now unlock the mutex
        pthread_mutex_unlock(&queueMutex);
        
        // - and process the new data
        ProcessData(newData);
    }
}
