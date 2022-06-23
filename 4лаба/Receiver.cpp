#include <fstream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <unistd.h>


#include <fcntl.h>
#include <iostream>


#include <sys/shm.h>
#include <errno.h>
#include "Struct.cpp"



#include <sys/sem.h>

using namespace std;


/* Тип сообщения для прекращения работы программы lab11.6 */
#define LAST_MESSAGE 255
const char* const ReadSemaphoreName = "my_mailbox_sem";
const int MailboxBytes = 500;


fileData* FileData;
int ammountOfSendlers;


int main(int argc, char ** argv)
{

    char* fileName = "/home/uliana/programming/Папка/os/4лаба/file.txt";
    ifstream fin(fileName);



    int shmid;   // IPC дескриптор для области разделяемой памяти
    char pathname[] = "/home/uliana/programming/Папка/os/4лаба/soop";
    key_t key; // IPC ключ
    // Генерируем IPC ключ из имени файла и номера экземпляра области разделяемой памяти 0
    if ((key = ftok(pathname, 1)) < 0)
    {
        perror("lab11.1 - Не могу сгенерировать ключ...");
        exit(-1);
    }

    // Пытаемся создать разделяемую память для сгенерированного  ключа, т.е. если для этого ключа она уже существует, системный вызов
    // вернет отрицательное значение. Размер памяти определяем как размер массива из трех целых переменных, права доступа 0666 – чтение и запись разрешены для всех
    if ((shmid = shmget(key,sizeof(fileData), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        // В случае ошибки пытаемся определить: возникла ли она из-за того, что сегмент разделяемой памяти уже существует или по другой причине
        if (errno != EEXIST)
        {
            // Если по другой причине – прекращаем работу
            perror("lab11.1 - Не могу открыть разделяемую память");
            exit(-1);
        }
        else
        {
            // Если из-за того, что разделяемая память уже существует, то пытаемся получить ее IPC дескриптор и, в случае удачи,
            // сбрасываем флаг необходимости инициализации элементов массива
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0)
            {
                perror("lab11.1 - Не могу найти разделяемую память: ");
                exit(-1);
            }
        }
        
    }

    // Пытаемся отобразить разделяемую память в адресное пространство текущего
    // процесса. Обратите внимание на то, что для правильного сравнения мы явно
    // преобразовываем значение -1 к указателю на целое.
    if ((FileData = (fileData *)shmat(shmid, NULL, 0)) == (fileData *)(-1))
    {
        perror("lab11.1 - Не могу подсоединить разделяемую память: ");
        exit(-1);
    }

    //FileData->sem = sem_open(ReadSemaphoreName, O_CREAT, 0777, 1);
    FileData->currAmmountOflinesInFile = 0;
    FileData->ammountOfLinesInFile = 5;
  


    /* IPC дескриптор для массива IPC семафоров */
    int semid;
    /* Структура для задания операции над семафором */
    struct sembuf mybuf;
    /* Пытаемся получить доступ по ключу к массиву семафоров,
    * если он существует, или создать его из одного семафора,
    * если его еще не существует, с правами доступа
    * read & write для всех пользователей */
    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
    perror("lab10.1 - Не могу получит semid:");
    exit(-1);
    }
    system("ipcs -s");
        

    printf("Resiver sozdau sender\n");
    pid_t childCreator = fork();
    if (childCreator < 0){
        printf("fork():ERROR childCreator\n");

    }else if (childCreator == 0){
        char * myargv[] =  {
                            (char*)"./Receiver",
                            (char*)"./Sender",
                            fileName,
                            NULL
                        }; 
        execvp(myargv[1], &myargv[1]);
        perror("execvp");
        return EXIT_FAILURE; 
    }

    pid_t cpid;
    int status;
     while((cpid = waitpid(-1, &status, WUNTRACED)) > 0) {
        if (WIFSTOPPED(status)) {
            // printf("Resiver buju sender\n");
            kill(cpid, SIGCONT);
            break;
        }
     }

        int val = semctl (semid, 0, GETVAL);
        printf("семафор0: может войти: %d\n",val);
        mybuf.sem_op = -1;
        mybuf.sem_flg = SEM_UNDO;
        mybuf.sem_num = 0;
        if(semop(semid, &mybuf, 1) < 0){
        perror("lab10.1 - Не могу выполнить semop(...):");
        exit(-1);
        }
        val = semctl (semid, 0, GETVAL);
        printf("семафор00: может войти: %d\n",val);



 /* Выполним операцию D(semid1,1) для нашего массива семафоров.
    * Для этого сначала заполним нашу структуру.
    * Флаг, как обычно, полагаем равным 0.
    * Наш массив семафоров состоит:
    * из одного семафора с номером 0. Код операции -1.*/
//     int val = semctl (semid, 0, GETVAL);
//     printf("%d",val);
//     mybuf.sem_op = -1;
//     mybuf.sem_flg = SEM_UNDO;
//     mybuf.sem_num = 0;
//     puts("lab10.1 - Начинаю выполнять системный вызов: semop(...)...");
//     if(semop(semid, &mybuf, 1) < 0){
//     perror("lab10.1 - Не могу выполнить semop(...):");
//     exit(-1);
//     }
//     puts("lab10.1 - Выполнил операцию D(semid1,1)...");
//     puts("lab10.1 - завершила работу...");
// val = semctl (semid, 0, GETVAL);
//     printf("%d",val);








    // while (1)
    // {
    //      printf("////////////R st \n");
    //     mybuf.sem_op = -1;
    //     mybuf.sem_flg = SEM_UNDO;
    //     mybuf.sem_num = 0;
    //     if(semop(semid, &mybuf, 1) < 0){
    //     perror("lab10.1 - Не могу выполнить semop(...):");
    //     exit(-1);
    //     }

    //     int val = semctl (semid, 0, GETVAL);
    //     printf("семафор1: может войти: %d\n",val);
    //     printf("1 - read a message\n2 - exit\n");

    //     int answer;
    //     scanf("%d",&answer);
    //     if (answer == 1) {
    //         if(FileData->currAmmountOflinesInFile > 0){
    //             string line;
    //             getline(fin,line);
    //             cout << line;
    //         }else{
    //             printf("There are no messages. Wait...\n");
    //             fin.clear();
    //         }
    //         mybuf.sem_op = 1;
    //         mybuf.sem_flg = SEM_UNDO;
    //         mybuf.sem_num = 0;
    //         if(semop(semid, &mybuf, 1) < 0){
    //         perror("lab10.1 - Не могу выполнить semop(...):");
    //         exit(-1);
    //         }

    //         val = semctl (semid, 0, GETVAL);
    //         printf("семафор11: может войти: %d\n",val); 

    //     }else {
    //         fin.close();
    //         return 0;
    //     }

    //      printf("////////////R end \n");
         
    // }
    
    
    
   
    return 0;
}