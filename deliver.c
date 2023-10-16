// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>  
            
#define PORT 8080
#define MAXLINE 1024

//access token:
//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

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
            char *ftp = "ftp";
            struct sockaddr_in             servaddr;
            char fileName[MAXLINE];
            bool fileExists = false;

            //initialize clock
            clock_t start, end;
            double roundTripTime;
            
            // Creating socket file descriptor
            if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                        perror("socket creation failed");
                        exit(EXIT_FAILURE);
            }
            
            memset(&servaddr, 0, sizeof(servaddr));
                        
            // Filling server information
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(atoi(argv[2]));
            // servaddr.sin_addr.s_addr = INADDR_ANY;
            servaddr.sin_addr.s_addr = inet_addr(argv[1]);

            // inet_aton("192.168.1.1", &servaddr.sin_addr.s_addr);
                        
            int n, len;

            printf("ftp ");
            fgets(fileName, sizeof(fileName), stdin);

            size_t length = strlen(fileName);

            if(length > 0 && fileName[length - 1] == '\n'){
                fileName[length - 1] = '\0';
            }

            DIR* dir = opendir("/homes/r/rahma877/ece361/ECE361");
            if(dir == NULL){
                perror("Error opening directory");
                return -1;
            }

            struct dirent* entry;
            while((entry = readdir(dir)) != NULL){
                // printf("%s, %s\n", entry->d_name, fileName);
                if(strcmp(entry->d_name, fileName) == 0){
                    fileExists = true;
                    // printf("found file\n");
                }
            }
            closedir(dir);

            if(!fileExists){
                close(sockfd);
                exit(0);
            }

            //creating packet to send to server
            Packet packet0;
            packet0.total_frag = 1;
            packet0.frag_no = 1;
            packet0.size = sizeof("test data");
            packet0.filename = "test file";
            packet0.filedata = "test data";

            //initializing string to hold packet data;
            char packetString[strlen(itoa(packet0.total_frag)) + strlen(itoa(packet0.frag_no))
              + strlen(itoa(packet0.size)) + strlen(packet0.filename) + strlen(packet0.filedata) + 5];

            strcat(packetString, itoa(packet0.total_frag));
            strcat(packetString, ':');
            strcat(packetString, itoa(packet0.frag_no));
            strcat(packetString, ':');
            strcat(packetString, itoa(packet0.size));
            strcat(packetString, ':');
            strcat(packetString, packet0.filename);
            strcat(packetString, ':');
            strcat(packetString, packet0.filedata);
            strcat(packetString, "\n");

            //start clock
            start = clock();

            sendto(sockfd, (const char *)ftp, strlen(ftp),
                        MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                                    sizeof(servaddr));
                                    
            n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                                                MSG_WAITALL, (struct sockaddr *) &servaddr,
                                                &len);
            //end clock
            end = clock();

            buffer[n] = '\0';

            printf("round trip time is: %f seconds\n", ((double)(end - start))/CLOCKS_PER_SEC);

            if(buffer[0] == 'y' && buffer[1] == 'e' && buffer[2] == 's' && buffer[3] == '\0'){
                printf("\nA file transfer can start\n");
            }
            else{
                close(sockfd);
                exit(0);
            }
            
            close(sockfd);
            return 0;
}