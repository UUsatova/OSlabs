#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

void *thread_function(void *arg);
void showMass();

struct ReadyMarkerCounter {
    int start;
    int currDestroy;
    int curSign;
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

int main(int argc, char const *argv[]){
    pthread_mutex_init(&fp.mutex,NULL);
    pthread_cond_init(&fp.condition_variableForMain,NULL);
    fp.start = 0;
    fp.amountOfReadyMarkers = 0;

    cin >> massSize;
    mass = new int[massSize];
    for (int i = 0; i < massSize; i++){
        mass[i] = 0;
    }

    printf( "введите количество потоков marker\n" );
    cin >> ammountOfMarkerStrim;
    int s = ammountOfMarkerStrim;
    vector<int> ind;
    fp.condition_variable = new pthread_cond_t [ammountOfMarkerStrim ];
    for (int i = 0; i < ammountOfMarkerStrim ; i++){
        pthread_cond_init(&fp.condition_variable[i],NULL);
        printf("%d  ",fp.condition_variable[i].__data);
        ind.push_back(i+1);

    }
    printf("\n");
    pthread_t *a_thread = new pthread_t[ammountOfMarkerStrim ];
    for (int i = 0; i < ammountOfMarkerStrim; i++){
        pthread_create(&a_thread[i], NULL, thread_function, (void*)&ind.at(i));
           
    }

    while(ammountOfMarkerStrim > 0) {
        pthread_mutex_lock(&fp.mutex);
        fp.start = 1;
        pthread_cond_broadcast(&fp.condition_StartAll);
       
        while (fp.amountOfReadyMarkers != ammountOfMarkerStrim) {
            pthread_cond_wait(&fp.condition_variableForMain,&fp.mutex); 
        }

        showMass();
        printf( "введите номер унинчтожаемого потока\n" );
        scanf("%d",&fp.currDestroy);
        fp.curSign = fp.currDestroy; 
        pthread_cond_signal(&fp.condition_variable[fp.currDestroy - 1]); 
        while (fp.start != 3){
        pthread_cond_wait(&fp.condition_variableForMain, &fp.mutex);

        }
        
        showMass();
        pthread_cond_signal(&fp.condition_variable[fp.currDestroy - 1]);
        fp.start = 2;
        ammountOfMarkerStrim--;
        ind.erase(std::remove(ind.begin(), ind.end(), fp.currDestroy), ind.end());
        for (int i = 0; i < ind.size() ; i++){
            printf("%d  ",ind.at(i));
            
        }
        printf("\n");
        for (int i = 0 ; i < (int)ind.size()  ; i++){
            fp.curSign = ind.at(i);pthread_cond_signal(&fp.condition_variable[fp.curSign - 1]); 
            pthread_cond_wait(&fp.condition_variableForMain, &fp.mutex); 
                   
        }
        fp.curSign = -1;
         fp.start = 1;
         pthread_mutex_unlock(&fp.mutex);
         pthread_cond_broadcast(&fp.condition_StartAll);
    }
    
    return 0;
}


void *thread_function(void *arg) {
    int num = *(int*)arg;
    int number = 0;
    int amountOfMarked = 0;
    
    while (true)
    {    pthread_mutex_lock(&fp.mutex);
         while (fp.start != 1){
            pthread_cond_wait(&fp.condition_StartAll,&fp.mutex);
        }

        pthread_mutex_unlock(&fp.mutex);
        number = rand() % massSize;
        if(mass[number] == 0){
            sleep(0.005);
            mass[number] = num;
            amountOfMarked++;
            sleep(0.005);
        }
        else{
            pthread_mutex_lock(&fp.mutex);
            printf("Мой порядковый номер: %d \n",num);
            printf("Количество помеченных элементов: %d\n",amountOfMarked);
            printf("Номер элемента который невозможно пометить: %d\n",number );
            fp.amountOfReadyMarkers++; 
    
            pthread_cond_signal(&fp.condition_variableForMain);
            while (fp.curSign != num ){ 
                pthread_cond_wait(&fp.condition_variable[num - 1],&fp.mutex); 
            }
            if(fp.currDestroy == num){ 
                for (int i = 0; i < massSize; i++){
                   if(mass[i] == num ) mass[i] = 0;
                }
                fp.amountOfReadyMarkers = 0; 
                fp.start = 3;
                pthread_cond_signal(&fp.condition_variableForMain);
                pthread_mutex_unlock(&fp.mutex);
                pthread_exit(NULL);

            }else {
                pthread_mutex_unlock(&fp.mutex);
                pthread_cond_signal(&fp.condition_variableForMain);
            }
               
        }

    }
    
 }

void showMass(){
    for (int i = 0; i < massSize; i++){
      cout << mass[i] << " "; 
       
    }
    cout << endl;
      
}
