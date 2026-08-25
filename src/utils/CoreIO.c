#include "CoreIO.h"
#include <unistd.h> 
#include <assert.h>
/*
    read size byte -> n;
    read byte stream is transfered by read buffer pointer -> *buf
    loop acts so we read the full byte streams from connfd
    rv -> read as much byte as possible from stream [impossible for rv > n]
*/
int32_t read_full(int fd, char *buf, size_t n){
    while(n>0){
        ssize_t rv = read(fd, buf, n);
        if(rv<=0)return -1;
        assert((size_t)rv<=n);
        n-=(size_t)rv;
        buf+=rv;
    }
    return 0;
}
/*
    basically same with read_full() tapi instead of read we use -> write
*/
int32_t write_full(int fd, char *buf, size_t n){
    while(n>0){
        ssize_t rv = write(fd, buf, n);
        if(rv<=0)return -1;
        assert((size_t)rv<=n);
        n-=(size_t)rv;
        buf+=rv;
    }
    return 0;
}