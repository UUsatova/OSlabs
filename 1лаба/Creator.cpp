
#include <iostream>
#include "Employee.h"
using namespace std;
int main(int argc, char *argv[]){
        char* fileName = argv[2];
        int amountOfLinesInBFile = atoi( argv[3]);

        FILE *f; 
        f=fopen(fileName, "wb");

        for (int i=0; i<amountOfLinesInBFile; ++i){
                Employee emp;
                cout << "Write num: ";
                cin >> emp.num;
                cout << "Write name: ";
                cin >> emp.name;
                cout << "Write hours: ";
                cin >> emp.hours;
                fwrite(&emp, sizeof(emp), 1, f);
        }
        fclose(f);
        system("pause");
        return 0;
}
