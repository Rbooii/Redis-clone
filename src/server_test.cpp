#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <string_view>

uint16_t SERVERPORT = 3000;

static void reportErrorMsg(std::string x){
    using namespace std;
    cout << "Crash, Error from -> " << x << endl;
    abort();
}

static void do_something_lol(int connfd){
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if(n < 0){
        std::cout << "read error\n";
        return;
    }else if (n == 0){
        std::cout << "client disconnected!\n";
        return;
    }
    std::cout << "Client says : " << rbuf << std::endl;
    char wbuf[] = "world";
    write(connfd, wbuf, std::string_view(wbuf).length());
}

int main(void){
    //socket creation
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    //if socket failed 
    if(fd < 0) reportErrorMsg("fd()");
    std::cout << "Socket Created (fd):" << fd << std::endl;

    //mystery stuff lmao
    int v = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));

    //bind
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVERPORT);
    addr.sin_addr.s_addr = htonl(0);
    int rv = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));
    if(rv) reportErrorMsg("bind");

    rv = listen(fd, SOMAXCONN);
    if(rv) reportErrorMsg("listen()");

    //accept
    while (true) {
        sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if(connfd < 0) continue;

        do_something_lol(connfd);
        close(connfd);
    }

    close(fd);
    return 0;
}