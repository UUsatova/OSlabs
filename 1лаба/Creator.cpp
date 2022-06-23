
#include <iostream>
#include "Employee.h"
using namespace std;
int main(int argc, char *argv[])
{

char* fileName = argv[2];
int amountOfLinesInBFile = atoi( argv[3]);


FILE *f; //описываем файловую переменную
//создаем двоичный файл в режиме записи
f=fopen(fileName, "wb");

//цикл для ввода  структур
for (int i=0; i<amountOfLinesInBFile; ++i)
{
//ввод очередной структуры
Employee emp;
        cout << "Write num: ";
        cin >> emp.num;
        cout << "Write name: ";
        cin >> emp.name;
        cout << "Write hours: ";
        cin >> emp.hours;

//запись очередной структуры в двоичный файл
        fwrite(&emp, sizeof(emp), 1, f);
}
//закрываем файл
fclose(f);
system("pause");
return 0;
}