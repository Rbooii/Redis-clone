#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 3333

void reportErrorMessage(char *str, u_int16_t end){
    printf("Error -> %s\n", str);
    if(end == 1){
        printf("Aborted!\n");
        abort();
    }
}

static void do_something(int connfd){
    char read_buff[64] = {};
    ssize_t n = read(connfd, read_buff, sizeof(read_buff)-1);
    if(n<0){
        reportErrorMessage("read() err!",0);
        return;
    }
    printf("Client Message : %s\n", read_buff);


    //write to client
    char w_buff[] = "hi this is server!";
    write(connfd, w_buff, strlen(w_buff));
}

int main(void){
    printf("Server code running at port:%d...\n", PORT);

    /*
    AF_INET -> IPv4 compatibility
    SOCK_STREAM -> TCP
    0 -> protocol [default ipv4 and tcp]
    */
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) reportErrorMessage("Failed to create FD", 1);
    printf("Socket Created!\n");
    
    /* enable reuse address socket option */
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    //bind
    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(0);
    int rv = bind(fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    if(rv) reportErrorMessage("Failed to Bind Port", 1);

    //listen
    rv = listen(fd, SOMAXCONN);
    if(rv) reportErrorMessage("Failed to Listen", 1);

    //accept client connection loop
    while(1){
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if(connfd < 0) continue;
        do_something(connfd);
        close(connfd);  
    }

    close(fd);
    return 0;
}