#include <iostream>
#include <fstream> 
#include "Employee.h"
using namespace std;
int main(int argc, char const *argv[]){
    const char* reportFileName = argv[3];
    ofstream fout(reportFileName);
    if (!fout.is_open()) {
     cout << "Файл не может быть открыт или создан\n"; 
     return 1; 
    }

    const char* originalBinaryFileName = argv[2];
    int payPerHour = atoi( argv[4]);

    //const char* originalBinaryFileName = "bin2.dat";
    //int payPerHour = 8;
    FILE *f = fopen(originalBinaryFileName, "rb");
    fout << "Отчет по файлу \"" << originalBinaryFileName << "\"" << endl;

     while ( !feof(f) ){
         Employee employee;

         fread(&employee, sizeof(employee), 1, f);
         if (feof(f)) break;
         fout << employee.num << " ";
         fout << employee.name  << " "; 
         fout << employee.hours << " ";
         fout << payPerHour *  employee.hours << " " << endl;   
    }
    fclose(f);
    
    return 0;
}
