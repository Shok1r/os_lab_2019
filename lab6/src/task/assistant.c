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

    int port = 20002;
    int number_of_assistent = 1;

    struct sockaddr_in main_server;
    main_server.sin_family = AF_INET;
    main_server.sin_port = htons(20001);                  
    main_server.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 


    while (number_of_assistent < 12)
    {
    
        int sck = socket(AF_INET, SOCK_STREAM, 0);
        if (sck < 0)
        {
            fprintf(stderr, "Can not create server socket!");
            return 1;
        }

        int opt_val = 1;
        setsockopt(sck, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
        
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons((uint16_t)port);
        server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        int err = bind(sck, (struct sockaddr *)&server, sizeof(server));
        if (err < 0)
        {
            fprintf(stderr, "Can not bind to socket!");
            return 1;
        }

        
        if (connect(sck, (struct sockaddr *)&main_server, sizeof(main_server)) < 0)
        {
            fprintf(stderr, "Assistant %d connection failed\n", number_of_assistent);
        }
        else 
            printf("Соединение отправлено %d\n", number_of_assistent);
        
        port += 1;
        number_of_assistent += 1;

    }
    
    return 0;
}