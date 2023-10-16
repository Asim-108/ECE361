// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024
#define SERVERADDRESS "192.168.1.1"

typedef struct packet{
      unsigned int
      total_frag;unsigned
      int frag_no; unsigned
      int size;
      char* filename;
      char filedata[1000];
}Packet;

// Driver code
int main(int argc, char* argv[]) {
      int sockfd;
      char buffer[MAXLINE];
      char *hello = "Hello from server";
      char *yes = "yes";
      char* no = "no";
      struct sockaddr_in servaddr, cliaddr;
            
      // Creating socket file descriptor
      if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
      }
            
      memset(&servaddr, 0, sizeof(servaddr));
      memset(&cliaddr, 0, sizeof(cliaddr));

      printf("%s\n", argv[1]);
           
      // Filling server information
      servaddr.sin_family = AF_INET; // IPv4
      // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
      servaddr.sin_addr.s_addr = INADDR_ANY;
      servaddr.sin_port = htons(atoi(argv[1]));
            
      // Bind the socket with the server address
      if ( bind(sockfd, (const struct sockaddr *)&servaddr,
                  sizeof(servaddr)) < 0 )
      {
            perror("bind failed");
            exit(EXIT_FAILURE);
      }
            
      int len, n;
      
      len = sizeof(cliaddr); //len is value/result
      
      n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                        MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                        &len);
      buffer[n] = '\0';
      if(buffer[0] == 'f' && buffer[1] == 't' && buffer[2] == 'p' && buffer[3] == '\0'){
          sendto(sockfd, (const char *)yes, strlen(yes),
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                  len);
      }
      else{
          sendto(sockfd, (const char *)no, strlen(no),
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                  len);
      }
      printf("Client : %s\n", buffer);

      close(sockfd);
            
      return 0;
}