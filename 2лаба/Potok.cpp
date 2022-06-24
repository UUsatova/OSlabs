#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <iostream>

#include <unistd.h>
using namespace std;

struct Array{
	int size;
	int *array;
	int min;
	int max;
	int average;
};

void showArray(Array array){
	for (int i = 0; i < array.size; i++) {
		cout << array.array[i] << " ";

	}
	cout << endl;
}

void inArray(Array array){
	for (int i = 0; i < array.size; i++) {
		cin >> array.array[i] ;

	}
	cout << endl;
}

void * min_max(void *arg){
	Array* array =  (Array *) arg;
	int min = 2147483647;
	int max = -2147483648;
	for (int i = 0; i < array->size; i++) {
		if(array->array[i] > max ) {
			max = array->array[i];
		}
		sleep(0.007);
		if(array->array[i] < min ){
			min = array->array[i];
		}
		sleep(0.007);
	}
	int * maxmin = new int[2];
	maxmin[0] = max;
	maxmin[1] = min;
	return (void*)maxmin;
}

void * average(void *arg){
	Array array = * (Array *) arg;
	int sum = 0;
	for (int i = 0; i < array.size; i++) {
		sum = sum + array.array[i] ;
		sleep(0.012);
	}
	array.average = sum / array.size;
	return (void*)(array.average);
}

void changeArray(Array array){
	cout << "changeArray" << endl;
	for (int i = 0; i < array.size; i++) {
		if(array.array[i] == array.max) array.array[i] = array.average;
		if(array.array[i] == array.min) array.array[i] = array.average;
	}
}

int main(int argc, char * argv[]){
    


	cout << "array size:" << endl;
    	Array array; 
	cin >> array.size ;
	array.array = new int[array.size];
	cout << "array:" << endl;
    	inArray(array);
	showArray(array);
	
	pthread_t thread1;
	int result = pthread_create(&thread1, NULL, min_max, &array);
	if (result != 0) {
		perror("Создание первого потока!");
		return EXIT_FAILURE;
	}

	pthread_t thread2;
	result = pthread_create(&thread2, NULL, average, &array);
	if (result != 0) {
		perror("Создание второго потока");
		return EXIT_FAILURE;
	}


	int*  maxmin ;
	result = pthread_join(thread1, (void**)&maxmin);
	 if (result != 0) {
		perror("Ждём первый поток");
		return EXIT_FAILURE;
	 }
	  
	 array.max = maxmin[0];
	 array.min = maxmin[1];
	 cout << "max: " << maxmin[0] << endl;
	 cout << "min: " << maxmin[1] << endl;
	int average;
	result = pthread_join(thread2, (void**)&average);
	if (result != 0) {
		perror("Ждём второй поток");
		return EXIT_FAILURE;
	}
	array.average = average;
	cout<< "average: " << average << endl;


   	 changeArray(array);
	showArray(array);

	printf("Done\n");
	return EXIT_SUCCESS;
}

//g++ -Wall -c Potok.cpp -o  Potok.o
//g++ -o Potok Potok.o -lpthread */
