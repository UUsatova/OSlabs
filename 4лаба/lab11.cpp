/* Эта программа lab10.1 получает доступ к одному системному семафору
и ждет, пока его значение не станет больше или равным 1,
а затем уменьшает его на 1*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    char str[20];
    scanf("%20s", str);
    printf(str);
}
