#include "CoreDebug.h" 
#include <stdio.h>     
#include <stdlib.h>   

void reportErrorMessage(char *str, uint16_t end){
    printf("Error -> %s\n", str);
    if(end == 1){
        printf("Aborted!\n");
        abort();
    }
}