#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <string_view>
#include <cassert>
#include <cerrno>

const uint16_t SERVERPORT = 3000;
const size_t k_max_msg = 4096;

static int32_t read_full(int fd, char *buf, size_t n){
    while(n > 0){
        ssize_t rv = read(fd, buf, n);
        if(rv <= 0) return -1; //err
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buf, size_t n){
    while(n>0){
        ssize_t rv = write(fd, buf, n);
        if(rv <= 0) return -1; //err
        assert((size_t)rv <= n);
        n-=(size_t)rv;
        buf+= rv;
    }
    return 0;
}

std::string concat(std::string str1, std::string str2){
    return str1+str2;
}

static void reportErrorMsg(std::string x){
    using namespace std;
    cout << "Crash, Error from -> " << x << "| errno -> " << static_cast<int>(errno) << endl;
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
    std::string wbuf = concat("emak lu ", rbuf);
    write(connfd, wbuf.c_str(), wbuf.length());
}

static int32_t work_req(int connfd){
    char rbuf[4+k_max_msg] = {};
    errno = 0;
    //baca 4 byte header;
    int32_t err = read_full(connfd, rbuf, 4); //4byte header = n;
    if(err)
    {
        std::cout << (errno == 0 ? "EOF" : "read() fail") << std::endl;
        return err;
    }
    
    uint32_t len = 0;
    memcpy(&len, rbuf, 4);
    if(len > k_max_msg){
        reportErrorMsg("messageTooLong");
        return -1;
    }

    //req-body
    err = read_full(connfd, &rbuf[4], len);
    if(err){
        std::cout << "read() fail" << std::endl;
        return err;
    }

    //imitate do-something func
    std::cerr << std::string(&rbuf[4], len) << std::endl;
    //reply w protocol
    std::string replyMessage = "helloo this is from server!";
    char writebuffer[4+replyMessage.size()];
    len = (uint32_t)replyMessage.length();
    memcpy(writebuffer, &len, 4);
    memcpy(&writebuffer[4], replyMessage.c_str(), len);
    return write_all(connfd, writebuffer, 4+len);
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
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv) reportErrorMsg("bind");

    //listen
    rv = listen(fd, SOMAXCONN);
    if(rv) reportErrorMsg("listen()");

    //accept-client-conn
    while (true) {
        sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if(connfd < 0) continue;

        //do_something_lol(connfd);
        work_req(connfd);
        close(connfd);
    }

    close(fd);
    return 0;
}