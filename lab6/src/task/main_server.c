#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char **argv)
{

    int port = 20001;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        fprintf(stderr, "Can not create server socket!");
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons((uint16_t)port);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

    int err = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
    if (err < 0)
    {
        fprintf(stderr, "Can not bind to socket!");
        return 1;
    }
 
    while(true) {
        err = listen(server_fd, 9);
        if (err < 0)
        {
        fprintf(stderr, "Could not listen on socket\n");
        return 1;
        }
    }


    printf("Main server listening at %d\n", port);

    return 0;
}