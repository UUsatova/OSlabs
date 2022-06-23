#include <semaphore.h>
struct fileData
{
    sem_t* sem;
    int ammountOfLinesInFile;
    int currAmmountOflinesInFile;

};