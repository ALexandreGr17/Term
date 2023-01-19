#include <iostream>
#include "utils.h"

void printArr(char** arr, int size){
    for(int i = 0; i < size; i++){
        std::cout << arr[i];
    }
    std::cout << std::endl;
}
