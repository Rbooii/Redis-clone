#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <string_view>
#include <cassert>
#include <cerrno>

const size_t k_max_msg = 4096;

static int32_t QueryClient(int fd, const std::string &body){
    uint32_t len = (uint32_t)body.size();
    if (len > k_max_msg) return -1;

    //write buff prep
    char wbuff[4+len];
    memcpy(wbuff, &len, 4);
    memcpy(&wbuff[4], body.c_str(), len);
    
}

int main(void){

    return 0;
}