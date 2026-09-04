#include "CoreDebug.hpp" 
#include <stdio.h>     
#include <stdlib.h>   
#include <string>
#include <vector>
#include <iostream>

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

void printASCII(){
    std::vector<std::string> ASCII_ART = {
        " ______     ______     _____     __     __  __ ",
        "/\\  == \\   /\\  ___\\   /\\  __-.  /\\ \\   /\\_\\_\\_\\  ",
        "\\ \\  __<   \\ \\  __\\   \\ \\ \\/\\ \\ \\ \\ \\  \\/_/\\_\\/_",
        "\\ \\_\\ \\_\\  \\ \\_____\\  \\ \\____-  \\ \\_\\   /\\_\\/\\_\\",
        "\\/_/ /_/   \\/_____/   \\/____/   \\/_/   \\/_/\\/_/ "
    };

    for (const auto& line : ASCII_ART) {
        std::cout << line << "\n";
    }
}