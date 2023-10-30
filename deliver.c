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
#include <math.h>  
            

#define PORT 8080
#define MAXLINE 1024


typedef struct packet{
      unsigned int  total_frag;
      unsigned int frag_no;
      unsigned int size;
      char* filename;
      char filedata[1000];
}Packet;
            

char *extractSubstring(const char *inputString, int start, int end) {
    if (inputString == NULL || start < 0 || end < start || end >= strlen(inputString)) {
        return NULL; // Invalid input
    }

    // Calculate the length of the substring
    int length = end - start + 1;

    // Allocate memory for the substring
    char *substring = (char *)malloc(length + 1); // +1 for null-terminator

    if (substring == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy characters from the original string to the substring
    for (int i = 0; i < length; i++) {
        substring[i] = inputString[start + i];
    }

    // Null-terminate the substring
    substring[length] = '\0';

    return substring;
}

char* packetToString(Packet packet0){
    int strSize[] = { 0, 0, 0 };
  double temp = (double) packet0.total_frag;
  while (temp >= 1)
    {
      temp /= 10;
      strSize[0]++;
    }

  temp = (double) packet0.frag_no;
  while (temp >= 1)
    {
      temp /= 10;
      strSize[1]++;
    }

  temp = (double) packet0.size;
  while (temp >= 1)
    {
      temp /= 10;
      strSize[2]++;
    }

//   char packetString[strSize[0] + strSize[1] + strSize[2] +
// 		    strlen (packet0.filename) + strlen (packet0.filedata) +
// 		    5];

  char *packetString = (char *) malloc ((strSize[0] + strSize[1] + strSize[2] 
  + strlen (packet0.filename) + strlen (packet0.filedata) + 4) * sizeof (char));

  char buffer0[strSize[0]];
  char buffer1[strSize[1]];
  char buffer2[strSize[2]];

  //itoa
  snprintf (buffer0, strSize[0] + 1, "%d", packet0.total_frag);
  snprintf (buffer1, strSize[1] + 1, "%d", packet0.frag_no);
  snprintf (buffer2, strSize[2] + 1, "%d", packet0.size);

  strcat (packetString, buffer0);
  strcat (packetString, ":");
  strcat (packetString, buffer1);
  strcat (packetString, ":");
  strcat (packetString, buffer2);
  strcat (packetString, ":");
  strcat (packetString, packet0.filename);
  strcat (packetString, ":");
  strcat (packetString, packet0.filedata);
  strcat (packetString, "\0");

  return packetString;
}



// Driver code
int main(int argc, char* argv[]) {

    FILE* file;
    char tempbuff[100000];
    struct packet number[1000];
    int Num_packets;
    
    

    








    int sockfd;
    char buffer[MAXLINE];
    char ACK_or_NACK[10];
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

    DIR* dir = opendir("/homes/c/cheu1444/ECE 361/ECE361");
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
    else{
        file = fopen(fileName, "r");
        if(file == NULL){
            printf("error");
            return 0;
        }
        else{
            size_t i = 0;
            int c;

            while ((c = fgetc(file)) != EOF && i < sizeof(tempbuff) - 1) {
                tempbuff[i++] = (char)c;
            }
            //fgets(buffer, sizeof(buffer), file);    
            int temp_length = strlen(tempbuff);
            Num_packets = 1;
            while(temp_length > 1000){
                temp_length = temp_length - 1000;
                Num_packets +=1;
            }
            int length = strlen(tempbuff);
            for(int i = 0; i< Num_packets; i++){
                number[i].total_frag = Num_packets;
                number[i].frag_no = i+1;
                if(i == Num_packets-1){
                    number[i].size = length%1000;
                }
                else{
                    number[i].size = 1000;
                }
                number[i].filename = fileName;
                if(i == Num_packets-1 && (length <= 1001)){
                    strcpy(number[i].filedata , extractSubstring(tempbuff, 0, length-1 ));
                }
                else if(i == Num_packets-1){
                    strcpy(number[i].filedata , extractSubstring(tempbuff, i*1000, length-1 ));
                }
                else{
                    strcpy(number[i].filedata , extractSubstring(tempbuff, i*1000, (i+1)*1000-1 ));
                }
            }        
            fclose(file);
        } 
        printf("%d", Num_packets);
        for(int j = 0; j < Num_packets; j++){
            printf("\n\n\nNum_paket: %d\nPacket_num: %d\nPacket_size: %d\nFilename: %s\nfile_data: %s\n",number[j].total_frag, number[j].frag_no, number[j].size, number[j].filename, number[j].filedata );
        }
    }

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







    ///////////////////////////////////////

    
/*
    for (int i = 0; i < num packets; i++
        while (buffer[n] != "ACK"){
            sleep(rand());
            sendto(sockfd, (const char *)packetString[i], strlen(packetString[i]),
                MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        }

*/
    
    

    buffer[n] = '\0';

    printf("round trip time is: %f seconds\n", ((double)(end - start))/CLOCKS_PER_SEC);

    if(buffer[0] == 'y' && buffer[1] == 'e' && buffer[2] == 's' && buffer[3] == '\0'){
        printf("\nA file transfer can start\n");
        bool packet_sucessfully_sent = false;
        //First Send First Packjet To Server
        
        
        for(int packet_number = 0; packet_number < Num_packets; packet_number++){
            packet_sucessfully_sent = false;
            while(packet_sucessfully_sent){
                sendto(sockfd, (const char *)packetToString(number[packet_number-1]), strlen(packetToString(number[packet_number-1])),
                    MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                                sizeof(servaddr));
                n = recvfrom(sockfd, (char *)ACK_or_NACK, 10,
                                            MSG_WAITALL, (struct sockaddr *) &servaddr,
                                            &len);   

                if(ACK_or_NACK[0] == 'A' && ACK_or_NACK[1] == 'C' && ACK_or_NACK[2] == 'K'){
                    packet_sucessfully_sent = true;
                    packet_number = packet_number +1;
                } 
            }
        }
        
                       
                           
                            




    }
    else{
        close(sockfd);
        exit(0);
    }
    
    close(sockfd);
    return 0;
}