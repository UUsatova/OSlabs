#include "Client.h"
#include <fstream>
#include <ctype.h>
#include <iostream>
#include <string>
using namespace std;
ofstream fout; 
ifstream fin; 
int server_fifo_fd, client_fifo_fd;
char client_fifo[256];
struct data_to_pass_st my_data;

void createAnswer(string info){
    strcpy(my_data.some_data, info.c_str());
    sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);
    client_fifo_fd = open(client_fifo, O_WRONLY);
    if (client_fifo_fd != -1) {
        write(client_fifo_fd, &my_data, sizeof(my_data));
        close(client_fifo_fd);

    }
}

string getDataFromFileById(int id){
    fin.open("DataBase.txt");
    while (!fin.eof())
    {
        string line;
        fin >> line;
        int currID = stoi(line);
        fin >> line;
        if(currID == id) {
            fin.close();
            return line;
        }
    }
    fin.close();
    return "неверный id";
     
}

void showFile(string fileName){
    fin.open("DataBase.txt");
    string line;
    while (!fin.eof()){
        getline(fin,line);
        cout << line << endl;
                    
    }
    fin.close();

}

void modifFile(int id,char extraData[]){
    fout.open("DataBase.txt",ios ::app);
    fout << "\n" << id << " " << extraData << "\n";
    fout.close();


}

int main() {
    showFile("DataBase.txt");

    
    int read_res;
    char *tmp_char_ptr;

    mkfifo(SERVER_FIFO_NAME, 0777);

     int ammountOfClients = 1;
    for (int i = 0 ; i < ammountOfClients; i++)
    {
        pid_t childCreator = fork();
        if (childCreator < 0){
        cout << "fork():ERROR childCreator\n";

        }else if (childCreator == 0){
            char * myargv[] =  {
                                (char*)"./Server",
                                (char*)"./Client",
                                NULL
                            }; 
            execvp(myargv[1], &myargv[1]);
            perror("execvp");
            return EXIT_FAILURE; 
        }
    }

    server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);

    if (server_fifo_fd == -1) {
        fprintf(stderr, "Server fifo failure\n");
        exit(EXIT_FAILURE);

    }

    

   
    while (1) {
        read_res = read(server_fifo_fd, &my_data, sizeof(my_data)); // возвращает колличество причитанныц байт

        if (read_res > 0) {
                int pearsonID,operationID;
                char extraData [20];
                sscanf(my_data.some_data, "%d %d %s", &pearsonID, &operationID,&extraData);
                if(operationID == 0 ){
                    ammountOfClients--;
                    if(ammountOfClients == 0) break;
                }
                if(operationID == 1){
                    string info = getDataFromFileById(pearsonID);
                    createAnswer(info);
                    
                }
                if(operationID == 2){
                    modifFile(pearsonID,extraData);
                    createAnswer("file was modifed\n");
                }

        }

    } 

 close(server_fifo_fd);

 unlink(SERVER_FIFO_NAME);

 exit(EXIT_SUCCESS);

}