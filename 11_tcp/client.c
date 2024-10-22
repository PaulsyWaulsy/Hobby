#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd) {
    char buff[MAX];
    int n;

    // Open a FILE* stream for both reading and writing
    FILE* to = fdopen(sockfd, "w");
    FILE* from = fdopen(sockfd, "r");

    if (!to || !from) {
        perror("fdopen");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        bzero(buff, sizeof(buff));

        // Get input from the user
        printf("Enter the string: ");
        fgets(buff, sizeof(buff), stdin);

        // Write to the socket using the FILE* stream
        fprintf(to, "%s", buff);
        fflush(to); // Make sure data is sent immediately

        bzero(buff, sizeof(buff));

        // Read from the socket using the FILE* stream
        if (fgets(buff, sizeof(buff), from) != NULL) {
            printf("From Server: %s", buff);
        }

        // Check if the client should exit
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }

    // Close the FILE* streams
    fclose(to);
    fclose(from);
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}

