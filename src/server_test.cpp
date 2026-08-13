#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

int main(void){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(fd < 0){
        std::cerr << "Socket Creation Failed :(\n";
        return 1;
    }
    std::cout << "created Socket : " << fd << " !\n";
    close(fd);
    return 0;
}