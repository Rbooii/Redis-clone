#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>

//Core-Header -> written
#include "CoreDebug.hpp"
#include "CoreIO.hpp"

#define PORT 3333
#define MAX_MESSAGE_LEN 4096

enum {
    STATE_REQ = 0,
    STATE_RES = 1,
    STATE_END = 2,
};

typedef struct Conn {
    int fd;
    uint32_t state;
    size_t rbuf_size;
    uint8_t rbuf[4+MAX_MESSAGE_LEN];
    size_t wbuf_size;
    size_t wbuf_sent;
    uint8_t wbuf[4+MAX_MESSAGE_LEN];
} Conn ;


/*
    function yang merubah fd dari blocking -> nonblocking mode
*/
static void fd_set_nb(int fd){
    errno = 0;
    int flags = fcntl(fd, F_GETFL, 0);
    if(errno){
        reportErrorMessage("fcntl error", 1);
        return;
    }
    flags |= O_NONBLOCK; //bitwise or
    errno = 0;
    (void)fcntl(fd, F_SETFL, flags);
    if(errno) reportErrorMessage("fcntl error", 1);
}

/*
    disini pakai protokol stream data seperti berikut :
    [panjang msg-1 (4byte)][msg-1][panjang msg-n (4byte)][msg-n][...]
    4byte -> little endian
*/

static int32_t one_request(int connfd){
    char read_buff[4+MAX_MESSAGE_LEN+1] = {}; //+1 for \0 "end of string/char*[]"  
    errno = 0;

    //read 4 byte header defining -> message len
    int32_t err = read_full(connfd, read_buff, 4);
    if(err){
        if(errno == 0){
            reportMessageNonError("EOF/DISCONNECT");
        }else{
            reportErrorMessage("read stream error", 0);
        }
        return err;
    }

    //copy message len and validate to the max message
    uint32_t message_len = 0;
    memcpy(&message_len, read_buff, 4);
    if(message_len > MAX_MESSAGE_LEN){
        reportErrorMessage("Message stream too long",0);
        return -1;
    }

    //actually read the body message stream by knowing len
    err = read_full(connfd, &read_buff[4], message_len);
    if(err){
        reportErrorMessage("Error while reading Message Stream", 0);
        return err;
    }

    //do something with incoming data from client
    read_buff[4+message_len] = '\0'; //end string
    printf("Message From Client : %s\n", &read_buff[4]);

    //reply client / confirm
    const char reply[] = "Hi client! this is server confirming!";
    char write_buffer[4+sizeof(reply)];
    message_len = (uint32_t)strlen(reply);
    memcpy(write_buffer, &message_len, 4);
    memcpy(&write_buffer[4], reply, message_len);
    return write_full(connfd, write_buffer, message_len + 4);
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
        
        //do_something(connfd);
        while(1){
            int32_t err = one_request(connfd);
            if(err) break;
        }

        close(connfd);  
    }

    close(fd);
    return 0;
}