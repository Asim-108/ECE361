// Server side implementation of UDP client-server model
// #include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
   
#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256

// int socket(int domain, int type, int protocol);

// int bind(int socket, struct sockaddr* address, int addr_len);
// int listen(int socket, int backlog);
// int accept(int socket, struct sockaddr* address, int *addr_len);


int main(){
    struct sockaddr_in sin, cliaddr;
    char buf[MAX_LINE];
    char *message = "Hello Client";
    int buf_len, addr_len;
    int s, new_s, len;

    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

     if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
         perror("simplex-talk: socket");
         exit(1);
     }
     if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
         perror("simplex-talk: bind");
         exit(1);
     }
     listen(s, MAX_PENDING);

    /* wait for connection, then receive and print text */
     while(1) {
         if ((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0) {
             perror("simplex-talk: accept");
             exit(1);
         }
         while (buf_len = recv(new_s, buf, sizeof(buf), 0))
         fputs(buf, stdout);
         close(new_s);
     }

    // s = socket(AF_INET, SOCK_STREAM, 0);
    // bind(s, (struct sockaddr*)&sin, sizeof(sin));

    // len = sizeof(cliaddr);
    // int n = recvfrom(s, buf, sizeof(buf),
    //         0, (struct sockaddr*)&cliaddr,&len); //receive message from server
    // buf[n] = '\0';
    // puts(buf);

    // sendto(s, message, MAX_LINE, 0,
    //       (struct sockaddr*)&cliaddr, sizeof(cliaddr));

}