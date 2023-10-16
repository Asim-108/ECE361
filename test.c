// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct packet{
    unsigned int total_frag;
    unsigned int frag_no;
    int size;
    char* filename;
    char filedata[1000];
};

int main(){
    FILE* file;
    char buffer[100000];
    file = fopen("test.txt", "r");
    if(file == NULL){
        printf("error");
        return 0;
    }
    else{
        fgets(buffer, sizeof(buffer), file);
        printf((buffer));
        int length = strlen(buffer);
        printf("\n%d", length);
        

        int Num_packets = 0;
        while(length > 1000){
            length = length - 1000;
            Num_packets 
        }

        fclose(file);
    }
    

};