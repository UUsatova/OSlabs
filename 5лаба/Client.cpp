#include "Client.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
struct data_to_pass_st my_data;
int server_fifo_fd, client_fifo_fd;
char client_fifo[256];
void createRequest(char line[]){
    sprintf(my_data.some_data , line);
    printf("%d sent %s, ", my_data.client_pid, my_data.some_data);
    write(server_fifo_fd, &my_data, sizeof(my_data));
}

char* waitAndGetAnswer(){
     client_fifo_fd = open(client_fifo, O_RDONLY);
     if (client_fifo_fd != -1) {
        if (read(client_fifo_fd, &my_data, sizeof(my_data)) > 0) {
            close(client_fifo_fd);
            return my_data.some_data;

        }
        close(client_fifo_fd);

    }
}

void read(char line[]){
     sprintf(line + strlen(line) , " %d",1);
     createRequest(line);
     printf("recieved: %s\n",waitAndGetAnswer());
}



int main() {
    
    int times_to_send;

    server_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
    if (server_fifo_fd == -1) {
        fprintf (stderr, "Sorry, no server\n");
        exit(EXIT_FAILURE);

    }
    my_data.client_pid = getpid();
    sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);

    if (mkfifo(client_fifo, 0777) == -1) {
        fprintf(stderr, "Sorry, can't make %s\n", client_fifo);
        exit(EXIT_FAILURE);

    }

    while(1) {
        char line[20];
        printf("Для чтения введите 1\n");
        printf("Для модификации введите 2\n");
        printf("Для остановки введите 0\n");
        scanf("%s",line);
        if(atoi(line) == 0 ) {
            sprintf(line + strlen(line) , " %d",0);
            createRequest(line);
            break;
        }
        else if(atoi(line) == 1) {
             printf("введите id юзера чтобы получить о нем информацию \n");
             scanf("%20s",line);
             read(line);

             try{
             while (atoi(line) != 3) {
             printf("введите 3 \n");
                 scanf("%s",line);
                 }
             }catch(...){}
             
             
        }
        else if(atoi(line) == 2){
            printf("введите id юзера чтобы получить о нем информацию c последующей модификацией \n");
             scanf("%20s",line);
             char helpLine [20];
             memcpy(helpLine, line, 20);
             read(helpLine);

            printf("введите модификации 1\n");
            char madInf[20];
            scanf("%20s",madInf);
            sprintf(line + strlen(line) , " %d %s",2,madInf);
            createRequest(line);
            printf(waitAndGetAnswer());

            try{
            while (atoi(line) != 3) {
             printf("введите 3 \n");
                 scanf("%s",line);
                 }
             }catch(...){}

        }

    }

 close(server_fifo_fd);
 unlink(client_fifo);
 exit(EXIT_SUCCESS);

}