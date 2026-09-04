#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <poll.h>

//Core-Header -> written
#include "CoreDebug.hpp"
#include "CoreIO.hpp"

#define PORT 3333

/*
    disini pakai protokol stream data seperti berikut :
    [panjang msg-1 (4byte)][msg-1][panjang msg-n (4byte)][msg-n][...]
    4byte -> little endian
*/

int main(void){
    printf("Server code running at port:%d...\n", PORT);

    /*
    AF_INET -> IPv4 compatibility
    SOCK_STREAM -> TCP
    0 -> protocol [default ipv4 and tcp]
    */
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) reportErrorMessage("Failed to create FD", 1);
    printASCII();
    
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

    std::vector<Conn *> fd2conn;
    fd_set_nb(fd); // -> setting fd to non blocking mode

    //event loop
    std::vector<struct pollfd> poll_args;
    while(1){
        poll_args.clear();
        struct pollfd pfd = {fd, POLLIN, 0};
        poll_args.push_back(pfd);

        for(Conn *conn : fd2conn){
            if(!conn) continue;
            struct pollfd pfd = {};
            pfd.fd  = conn->fd;
            pfd.events = (conn->state == STATE_REQ) ? POLLIN : POLLOUT;
            pfd.events = pfd.events | POLLERR;
            poll_args.push_back(pfd);
        }

        //poll untuk fd(s) yang aktif
        int rv = poll(poll_args.data(), (nfds_t)poll_args.size(), 1000);
        if(rv < 0){
            reportErrorMessage("Poll Error at Event Loop", 1);
        }

        //process active connection
        for(size_t i = 1; i < poll_args.size(); ++i){
            if(poll_args[i].revents){
                Conn *conn = fd2conn[poll_args[i].fd];
                connection_io(conn);
                if(conn->state == STATE_END){
                    fd2conn[conn->fd] = NULL;
                    (void)close(conn->fd);
                    delete(conn);
                }
            }
        }
        if(poll_args[0].revents)(void)accept_new_conn(fd2conn, fd);
    }

    //close(fd);
    return 0;
}