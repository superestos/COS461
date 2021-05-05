/*****************************************************************************
 * client-c.c                                                                 
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

#define SEND_BUFFER_SIZE 2048


/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char *server_ip, char *server_port) {
    char buffer[SEND_BUFFER_SIZE];
    int sockfd, numbytes;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // get address info of server, give hints to server and get possible option in servinfo
    if (getaddrinfo(server_ip, server_port, &hints, &servinfo) != 0) {
        fprintf(stderr, "client: error in getaddrinfo\n");
        return 1;
    }

    // servinfo is linked list
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 1;
    }

    freeaddrinfo(servinfo);
    
    numbytes = fread(buffer, 1, SEND_BUFFER_SIZE, stdin);
    while (numbytes > 0) {
        if (send(sockfd, buffer, numbytes, 0) == -1) {
            fprintf(stderr, "client: failed to send\n");
            close(sockfd);
            return 1;
        }
        numbytes = fread(buffer, 1, SEND_BUFFER_SIZE, stdin);
    }

    return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv) {
    char *server_ip;
    char *server_port;

    if (argc != 3) {
        fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    server_port = argv[2];
    return client(server_ip, server_port);
}
