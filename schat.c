//wcg9ev
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

struct pollfd fds[2];

int main(int argc, char *argv[]) {
    if (argc == 1) {

        // start by getting a random port from the ephemeral port range
        srandom(getpid()); // random seed based on this process's OS-assigned ID
        int port = 0xc000 | (random() & 0x3fff); // random element of 49152–65535

        // create an address structure: IPv4 protocol, any IP address, on given port
        // note: htonl and htons are endian converters, essential for Internet communication
        struct sockaddr_in ipOfServer;
        memset(&ipOfServer, 0, sizeof(struct sockaddr_in));
        ipOfServer.sin_family = AF_INET;
        ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
        ipOfServer.sin_port = htons(port);

        // we'll have one socket that waits for other sockets to connect to it
        // those other sockets will be the ones we used to communicate
        int listener = socket(AF_INET, SOCK_STREAM, 0);

        // and we need to tell the OS that this socket will use the address created for it
        bind(listener, (struct sockaddr*)&ipOfServer, sizeof(ipOfServer));

        // wait for connections; if too many at once, suggest the OS queue up 20
        listen(listener, 20);

        system("host $HOSTNAME"); // display all this computer's IP addresses
        printf("The server is now listening on port %d\n", port); // and listening port


        printf("Waiting for a connection\n");
        // get a connection socket (this call will wait for one to connect)
        int connection = accept(listener, (struct sockaddr*)NULL, NULL);
        puts("connection with client established");
        close(listener);

        fds[1].fd = connection;
        fds[0].events = POLLIN;

        for(;;){
if(fds[1].revents & POLLIN){
                char* msgIn = (char*)calloc(1024, sizeof(char));
                read(connection, msgIn, 1024);
                printf("%s", msgIn);
                free(msgIn);
        }
if(fds[0].revents & POLLIN){
                char *msgOut = (char *)calloc(1024, sizeof(char));
                read(0, msgOut, 1024);
                write(connection, msgOut, strlen(msgOut));
                free(msgOut);
        }
        }
        close(connection); // and disconnect

        // unreachable code, but still have polite code as good practice
        close(listener);
        return 0;
    }
    else {
        int port = atoi(argv[2]);
        char* ip_address_of_server = argv[1];

        struct sockaddr_in ipOfServer;
        ipOfServer.sin_family = AF_INET;
        ipOfServer.sin_port = htons(port);
        ipOfServer.sin_addr.s_addr= inet_addr(ip_address_of_server);

        int connection = socket(AF_INET, SOCK_STREAM, 0);
        connect(connection, &ipOfServer, sizeof(ipOfServer));
        puts("connection to server established");

        fds[1].fd = connection;
        fds[0].events = POLLIN;

        for (;;) {
if(fds[0].revents & POLLIN){
            char* msgIn = (char*)calloc(1024, sizeof(char));
            read(connection, msgIn, 1024);
            printf("%s", msgIn);
            free(msgIn);}
if(fds[0].revents & POLLIN){
            char* msgOut = (char*)calloc(1024, sizeof(char));
            read(0, msgOut, 1024);
            write(connection, msgOut, 1024);
            free(msgOut);}
        }

        close(connection);
    }
}
