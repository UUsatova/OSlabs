#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <vector>
using namespace std;

void *thread_function(void *arg);
struct ReadyMarkerCounter {
    int start;
    int currDestroy;
    int amountOfReadyMarkers;
    pthread_mutex_t  mutex;
    pthread_cond_t*   condition_variable;
    pthread_cond_t   condition_variableForMain;
    pthread_cond_t   condition_StartAll;
};

int massSize =  1024;
int ammountOfMarkerStrim = 0;
int* mass;
ReadyMarkerCounter fp;

void incCounterWhenMarkerIsReady( ){
    pthread_mutex_lock(&fp.mutex);
    printf("B0\n");
    fp.amountOfReadyMarkers++;
    printf("fp.amountOfReadyMarkers %d == ammountOfMarkerStrim %d\n ",fp.amountOfReadyMarkers,ammountOfMarkerStrim );
    if(fp.amountOfReadyMarkers == ammountOfMarkerStrim ){
        fp.amountOfReadyMarkers = 0;
        pthread_mutex_unlock(&fp.mutex);
        pthread_cond_signal(&fp.condition_variableForMain);
    }else pthread_mutex_unlock(&fp.mutex);
}

void showMass(){
    for (int i = 0; i < massSize; i++){
      cout << mass[i] << " ";  
    }
    cout << endl;
      
}

int main() {

    pthread_mutex_init(&fp.mutex,NULL);
    pthread_cond_init(&fp.condition_variableForMain,NULL);
    pthread_cond_init(&fp.condition_StartAll,NULL);
    fp.start = 0;

    cin >> massSize;
    mass = new int[massSize];
  
    for (int i = 0; i < massSize; i++){
        mass[i] = 0;
    }
    
    printf( "введите количество потоков marker\n" );
    cin >> ammountOfMarkerStrim;


    fp.condition_variable = new pthread_cond_t [ammountOfMarkerStrim ];
    int res;
    pthread_t *a_thread = new pthread_t[ammountOfMarkerStrim ];
    int* ind = new int[ammountOfMarkerStrim];
    for (int i = 0; i < ammountOfMarkerStrim ; i++){
        pthread_cond_init(&fp.condition_variable[i],NULL);
        ind[i] = i+1;
    }
    
   pthread_mutex_lock(&fp.mutex);
        for (int i = 0; i < ammountOfMarkerStrim; i++){
            res = pthread_create(&a_thread[i], NULL, thread_function, (void*)&ind[i]);
            if (res != 0) {
                perror("Thread creation failed");
                exit(EXIT_FAILURE);
            }

        }
    fp.start = 1;
    pthread_mutex_unlock(&fp.mutex);
   // pthread_mutex_lock(&fp.mutex);
    pthread_cond_broadcast(&fp.condition_StartAll);
    // printf("86\n");
    //pthread_mutex_unlock(&fp.mutex);
    // printf("88\n");
    
        int s = ammountOfMarkerStrim;
        while(ammountOfMarkerStrim > 0) {
     //    printf("92\n");
            pthread_mutex_lock(&fp.mutex);
            while (fp.amountOfReadyMarkers != ammountOfMarkerStrim )
            { 
                pthread_cond_wait(&fp.condition_variableForMain, &fp.mutex);
                 printf("97\n");
            }
            showMass();
            int deliteMarkeTread = 0;
            printf( "введите номер унинчтожаемого потока\n" );
            scanf("%d",&deliteMarkeTread);
            fp.currDestroy = deliteMarkeTread;
            fp.start = 1;
            // printf("B5\n");
            pthread_mutex_unlock(&fp.mutex);
            printf("B6\n");
            pthread_cond_signal(&fp.condition_variable[deliteMarkeTread - 1]);
            printf("B7\n");
            pthread_mutex_lock(&fp.mutex);
            printf("B8\n");
            pthread_cond_wait(&fp.condition_variableForMain, &fp.mutex);
            printf("B9\n");
            ammountOfMarkerStrim--;
            showMass();
            pthread_mutex_unlock(&fp.mutex);

          //  pthread_mutex_lock(&fp.mutex);
            int i = 1;
                while (i <= s ){
                   // printf( "while (%d < %d ) %d\n",i,s );
                   pthread_cond_signal(&fp.condition_variable[i - 1]); 
                   i++;
                }
          //  pthread_mutex_unlock(&fp.mutex); 
         //   printf(" ammountOfMarkerStrim %d\n", ammountOfMarkerStrim );
           // pthread_cond_broadcast(&fp.condition_variable);
    }
    
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) { 
    int amountOfMarked = 0;
    int number = 0;
    vector<int> marked;

    pthread_mutex_lock(&fp.mutex);
    while (fp.start != 1){
    pthread_cond_wait(&fp.condition_StartAll,&fp.mutex);
    }
    pthread_mutex_unlock(&fp.mutex);

    while (true){
        number = rand() % massSize;
       // printf("%d number: %d\n",*(int*)arg , number);
        if(mass[number] == 0){
            sleep(0.005);
            mass[number] = *(int*)arg;
            amountOfMarked++;
            marked.push_back(number);
            sleep(0.005);
        }
        else{

            pthread_mutex_lock(&fp.mutex);
            fp.start = 0;
            printf("B1\n");
            printf("Мой порядковый номер: %d \n",*(int*)arg);
            printf("Количество помеченных элементов: %d\n",amountOfMarked);
            printf("Номер элемента который невозможно пометить: %d\n",number );
            fp.amountOfReadyMarkers++;
            pthread_cond_signal(&fp.condition_variableForMain);
            pthread_cond_wait(&fp.condition_variable[*(int*)arg],&fp.mutex);
            //  printf("fp.amountOfReadyMarkers %d == ammountOfMarkerStrim %d\n ",fp.amountOfReadyMarkers,ammountOfMarkerStrim );
            //  if(fp.amountOfReadyMarkers == ammountOfMarkerStrim ){
            //      fp.amountOfReadyMarkers = 0;
            //      printf("B2\n");
            //      pthread_cond_signal(&fp.condition_variableForMain);
            //      printf("B3\n");
            //      pthread_cond_wait(&fp.condition_variable[*(int*)arg],&fp.mutex);   
            //  } else {
            //      printf("B4\n");
            //      while (fp.start != 1){
            //         pthread_cond_wait(&fp.condition_variable[*(int*)arg],&fp.mutex);
            //         printf("Вышел из ожидания %d" ,*(int*)arg );
            //      }
                 
            //  }


            if(fp.currDestroy == *(int*)arg){
                //printf("Проснулся %d \n",*(int*)arg);
                //printf("massSize %d \n",massSize);
                for (int i = 0; i < massSize; i++){
                   // printf("mass[i] == *(int*)arg  %d \n",mass[i] == *(int*)arg);
                   if(mass[i] == *(int*)arg ) mass[i] = 0;
                }
                //printf("1 pthread_cond_signal(&fp.condition_variableForMain)");
                pthread_mutex_unlock(&fp.mutex);
                pthread_cond_signal(&fp.condition_variableForMain);
                //printf("2 pthread_cond_signal(&fp.condition_variableForMain)");
                
                pthread_exit(NULL);
                //printf(" pthread_exit(NULL);");
                break;
            }
            
            pthread_mutex_unlock(&fp.mutex);

        }

    }
    
	 pthread_exit(NULL);
}
   
   