#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
using namespace std;

void outFile( char* fileName){
    ifstream fin(fileName); // открыли файл для чтения
    string line;
    while (getline(fin, line))
        {
            cout << line << endl;
        }
 
    
    fin.close(); 

}

int main(int argc, char *argv[]){

    cout << "start\n";


     char* bfileName = "./bin2.dat";
     //cin >> fileName;
     char* amountOfLinesInBFile = "3";
     //cin >> amountOfLinesInBFile;

    pid_t childCreator = fork();
    if (childCreator < 0){
        cout << "fork():ERROR childCreator\n";

    }else if (childCreator == 0){
        char * myargv[] =  {
                            (char*)"./try",
                            (char*)"./Creator",
                            bfileName,
                            (char*)amountOfLinesInBFile,
                            NULL
                        }; 
        execvp(myargv[1], &myargv[1]);
        perror("execvp");
        return EXIT_FAILURE; 
    }
    else{
        int status;
        wait(&status);
        cout << "Creator finished\n";
        
    }
    outFile(bfileName);




     char* reportFileName = "report.txt";
     //cin >> reportFileName;
     char* payPerHour = "3";
     //cin >> payPerHour;

     pid_t childReporter = fork();
    if (childReporter < 0){
        cout << "fork():ERROR childReporter\n";

    }else if (childReporter == 0){
        char * myargv[] =  {
                            (char*)"./try",
                            (char*)"./Reporter",
                            bfileName,
                            reportFileName,
                            (char*)payPerHour,
                            NULL
                        }; 
        execvp(myargv[1], &myargv[1]);
        perror("execvp");
        return EXIT_FAILURE; 
    }
    else{
        int status;
        wait(&status);
        cout << "Reporter finished\n";
        
    }
    outFile(reportFileName);

       

}