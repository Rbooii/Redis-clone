#include "CoreDebug.hpp" 
#include <stdio.h>     
#include <stdlib.h>   

void reportErrorMessage(const char *str, uint16_t end){
    printf("Error -> %s\n", str);
    if(end == 1){
        printf("Aborted!\n");
        abort();
    }
}

void reportMessageNonError(const char *str){
    printf("Message -> %s\n", str);
}