#include <unistd.h>
#include <errno.h>
#include "utils.h"

ssize_t readn(int fd, void *buffer, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *ptr = buffer;

    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0; // interrupção, tentar de novo
            else
                return -1;
        } else if (nread == 0) {
            break; // EOF
        }

        nleft -= nread;
        ptr += nread;
    }

    return (n - nleft);
}

ssize_t writen(int fd, const void *buffer, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    const char *ptr = buffer;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0; // interrupção, tentar de novo
            else
                return -1;
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return n;
}
