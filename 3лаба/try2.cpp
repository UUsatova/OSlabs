    #include <pthread.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>

#include <vector>
#include<algorithm>

using namespace std;
     
     
     int main(int argc, char const *argv[])
     {
          vector<int> vec = {1, 2, 3, 6, 7, 2, 4, 8, 2};
            // .. put in some values ..
            int int_to_remove = 8;
            for (int i = 0; i < vec.size() ; i++){
            printf("%d  ",vec.at(i));
            
            }
        printf("\n");
            vec.erase(std::remove(vec.begin(), vec.end(), int_to_remove), vec.end());
            for (int i = 0; i < vec.size() ; i++){
            printf("%d  ",vec.at(i));
            
        }
         return 0;
     }
     