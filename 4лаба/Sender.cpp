#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <sys/shm.h>
#include <errno.h>
#include "Struct.cpp"
#include <fcntl.h>

#include <sys/sem.h>

using namespace std;
//#include "Receiver.cpp"
fileData* FileData;
const char* const ReadSemaphoreName = "my_mailbox_sem";

int main(int argc, char ** argv){

    printf("SENDER beging\n");
    
    



    int shmid;   // IPC дескриптор для области разделяемой памяти
    char pathname[] = "/home/uliana/programming/Папка/os/4лаба/soop";
    key_t key; // IPC ключ
    // Генерируем IPC ключ из имени файла и
    // номера экземпляра области разделяемой памяти 0
    if ((key = ftok(pathname, 1)) < 0)
    {
        perror("lab11.1 - Не могу сгенерировать ключ...");
        exit(-1);
    }

    //Пытаемся эксклюзивно создать разделяемую память для сгенерированного
    // ключа, т.е. если для этого ключа она уже существует, системный вызов
    // вернет отрицательное значение.
    // Размер памяти определяем как размер массива из трех целых переменных,
    // права доступа 0666 – чтение и запись разрешены для всех
    if ((shmid = shmget(key,sizeof(fileData), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        // В случае ошибки пытаемся определить: возникла ли она из-за того,
        // что сегмент разделяемой памяти уже существует или по другой причине
        if (errno != EEXIST)
        {
            // Если по другой причине – прекращаем работу
            perror("lab11.1 - Не могу открыть разделяемую память");
            exit(-1);
        }
        else
        {
            // Если из-за того, что разделяемая память уже существует,
            // то пытаемся получить ее IPC дескриптор и, в случае удачи,
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
    /* Выполним операцию D(semid1,1) для нашего массива семафоров.
    * Для этого сначала заполним нашу структуру.
    * Флаг, как обычно, полагаем равным 0.
    * Наш массив семафоров состоит:
    * из одного семафора с номером 0. Код операции -1.*/
    // mybuf.sem_op = 1;
    // mybuf.sem_flg = 0;
    // mybuf.sem_num = 0;
    // puts("lab10.1 - Начинаю выполнять системный вызов: semop(...)...");
    // if(semop(semid, &mybuf, 1) < 0){
    // perror("lab10.1 - Не могу выполнить semop(...):");
    // exit(-1);
    // }
    // puts("lab10.1 - Выполнил операцию D(semid1,1)...");
    // puts("lab10.1 - завершила работу...");


    printf("SENDER stopping\n");
    raise(SIGSTOP);
    printf("SENDER pridoljil\n");

        int val = semctl (semid, 0, GETVAL);
        printf("семафор1: может войти: %d\n",val);
     mybuf.sem_op = 1;
        mybuf.sem_flg = SEM_UNDO;
        mybuf.sem_num = 0;
        if(semop(semid, &mybuf, 1) < 0){
        perror("lab10.1 - Не могу выполнить semop(...):");
        exit(-1);
        }
         val = semctl (semid, 0, GETVAL);
        printf("семафор11: может войти: %d\n",val);

    // while (1){
    //      printf("//////////S st \n");

    //     int  val = semctl (semid, 0, GETVAL);
    //      printf("семафор2: может войти: %d\n",val);

    //     mybuf.sem_op = -1;
    //     mybuf.sem_flg = SEM_UNDO;
    //     mybuf.sem_num = 0;
    //     if(semop(semid, &mybuf, 1) < 0){
    //     perror("lab10.1 - Не могу выполнить semop(...):");
    //     exit(-1);
    //     }
    //     printf("1 - send a message\n2 - exit\n");
    //     int answer;
    //     scanf("%d",&answer);
    //     if (answer == 1) {
    //         if(FileData->currAmmountOflinesInFile != FileData->ammountOfLinesInFile){
    //             FileData->currAmmountOflinesInFile++;
    //             printf("message:\n");
    //             char str[20];
    //             scanf("%20s", str);
    //             printf(" write message:\n");
    //             ofstream fout(argv[1], ios::out | ios::app);
    //             fout.write(str,20);
    //             fout.close();
    //         } else{
    //             printf("there is no space for a message. Wait...\n");
    //         }
    //         val = semctl (semid, 0, GETVAL);
    //         printf("семафор: может войти22: %d\n",val);
    //         mybuf.sem_op = 1;
    //         mybuf.sem_flg = SEM_UNDO;
    //         mybuf.sem_num = 0;
    //         if(semop(semid, &mybuf, 1) < 0){
    //         perror("lab10.1 - Не могу выполнить semop(...):");
    //         exit(-1);
    //     }
    //     }
    //     else{
    //         val = semctl (semid, 0, GETVAL);
    //         printf("семафор: может войти22: %d\n",val);

    //         mybuf.sem_op = 1;
    //         mybuf.sem_flg = SEM_UNDO;
    //         mybuf.sem_num = 0;
    //         if(semop(semid, &mybuf, 1) < 0){
    //         perror("lab10.1 - Не могу выполнить semop(...):");
    //         exit(-1);
    //         }
    //         return 0;
    //     }

    //     printf("//////////S end \n");

    // }
    


}