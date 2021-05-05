/*****************************************************************************
 * server-c.c                                                                 
 * Name:
 * NetId:
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(char *server_port) {
    char buffer[RECV_BUFFER_SIZE];
    int numbytes;
    int sockfd, connfd;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // not specified
    hints.ai_socktype = SOCK_STREAM; // TCP stream
    hints.ai_flags = AI_PASSIVE; // use my IP

    if (getaddrinfo(NULL, server_port, &hints, &servinfo) != 0) {
        fprintf(stderr, "server: error in getaddrinfo\n");
        return 1;
    }

    // servinfo is linked list
    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind");
        return 1;
    }

    freeaddrinfo(servinfo);

    if (listen(sockfd, RECV_BUFFER_SIZE) == -1) {
        fprintf(stderr, "server: failed to listen");
        return 1;
    }

    while (1) {
        struct sockaddr addr;
        socklen_t addrlen;

        connfd = accept(sockfd, &addr, &addrlen);
        if (connfd == -1) {
            continue;
        }

        numbytes = recv(connfd, buffer, RECV_BUFFER_SIZE, 0);
        while (numbytes > 0) {
            fwrite(buffer, numbytes, 1, stdout);
            fflush(stdout);
            numbytes = recv(connfd, buffer, RECV_BUFFER_SIZE, 0);
        }
    }

    return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) {
    char *server_port;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./server-c [server port]\n");
        exit(EXIT_FAILURE);
    }

    server_port = argv[1];
    return server(server_port);
}
