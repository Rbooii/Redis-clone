#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include "CoreDebug.hpp"
#include "CoreIO.hpp"

#define MAX_MESSAGE_LEN 4096
/*
LEGACY REQ-RES CODE
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
*/

static int32_t query(int fd, const char *text){
    uint32_t message_len = (uint32_t)strlen(text);
    if(message_len > MAX_MESSAGE_LEN) return -1;
    char write_buff[4+MAX_MESSAGE_LEN];
    memcpy(write_buff, &message_len, 4); //attach 4 byte header
    memcpy(&write_buff[4], text, message_len); //attach message body
    
    int32_t err = write_full(fd, write_buff, message_len+4);
    if(err){
        reportErrorMessage("Error sending message to server", 0);
        return err;
    }

    char read_buff[4+MAX_MESSAGE_LEN+1] = {};
    errno = 0;
    err = read_full(fd, read_buff, 4);
    if(err){
        if(errno == 0){
            reportErrorMessage("EOF", 0);
        }else{
            reportErrorMessage("read stream error", 0);
        }
        return err;
    }
    //copy message len and validate to the max message
    message_len = 0;
    memcpy(&message_len, read_buff, 4);
    if(message_len > MAX_MESSAGE_LEN){
        reportErrorMessage("Message stream too long",0);
        return -1;
    }

    //actually read the body message stream by knowing len
    err = read_full(fd, &read_buff[4], message_len);
    if(err){
        reportErrorMessage("Error while reading Message Stream", 0);
        return err;
    }

    //do something with incoming data from server
    read_buff[4+message_len] = '\0'; //end string
    printf("Message From Server : %s\n", &read_buff[4]);
    return 0;
}

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
    if(rv) reportErrorMessage("Failed to Connect Port", 1);

    while(1){
        char message[MAX_MESSAGE_LEN];
        printf("Send Data To server : ");
        if (fgets(message, sizeof(message), stdin) == NULL) {
            printf("\nClient Stopped.\n");
            break; 
        }
        message[strcspn(message, "\n")] = 0; 
        if (strlen(message) == 0) continue;
        int32_t err = query(fd, message);
        if(err){
            reportErrorMessage("Query Failed",0);
            break; 
        }
    }
    
    return 0;
}