#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 32

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, n;
     socklen_t clilen, servlen;

     char buffer[BUFFER_SIZE];
     FILE *fp;
     struct sockaddr_in serv_addr, cli_addr;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd, 5);

     clilen = sizeof(cli_addr);
     servlen = sizeof(serv_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     bzero(buffer, BUFFER_SIZE);
     n = read(newsockfd, buffer, BUFFER_SIZE);
     if (n < 0) error("ERROR reading from socket");

     printf("Received file name: %s\n", buffer);

     fp = fopen(buffer, "rb"); 
     if (fp == NULL) 
         printf("File open failed!\n"); 
     else
         printf("File successfully opened!\n");

    while (1) {
    size_t num_read = fread(buffer, 1, BUFFER_SIZE, fp);
    if (num_read == 0) // end of file.
        break;
         

    n = write(newsockfd, buffer, num_read);
    if (n < 0) // Error
        error("ERROR writing to socket");
    else if (n == 0) // Could handle this too
        break;
}
    printf("File sending complete...\n");

    if (fp != NULL)  
        fclose(fp);
     close(newsockfd);
     close(sockfd);

     return 0; 
}