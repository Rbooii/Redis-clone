#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "CoreDebug.h"
#include "CoreIO.h"

int main(int argc, char *argv[]){
    // args -> [1] *server port to connect;
    if(argc < 2){
        reportErrorMessage("NO PORT SPECIFIED!", 0);
        return 1;
    }

    int server_port = atoi(argv[1]);
    printf("Connect to Server port -> %d \n", server_port);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) reportErrorMessage("Failed to create FD", 1);
    printf("Socket Created!\n");

    //connect to server
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int rv = connect(fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if(rv) reportErrorMessage("Failed to Bind Port", 1);

    //send some msg
    char msg[] = "Hello this is from client!";
    if(argv[2]){
        strcpy(msg, argv[2]);
    }
    write(fd, msg, strlen(msg));

    //read server message
    char read_buff[64] = {};
    ssize_t n = read(fd, read_buff, sizeof(read_buff)-1);
    if(n<0){
        reportErrorMessage("read() err!",0);
        return 1;
    }
    printf("Server Message : %s\n", read_buff);
    return 0;
}