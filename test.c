// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

typedef struct packet{
      unsigned int
      total_frag;unsigned
      int frag_no; unsigned
      int size;
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

int main(){
    FILE* file;
    char buffer[100000];
    struct packet number[1000];
    int Num_packets;
    file = fopen("Text.txt", "r");
    if(file == NULL){
        printf("error");
        return 0;
    }
    else{
        size_t i = 0;
        int c;

        while ((c = fgetc(file)) != EOF && i < sizeof(buffer) - 1) {
            buffer[i++] = (char)c;
        }
        //fgets(buffer, sizeof(buffer), file);    
        int temp_length = strlen(buffer);
        Num_packets = 1;
        while(temp_length > 1000){
            temp_length = temp_length - 1000;
            Num_packets +=1;
        }
        int length = strlen(buffer);
        for(int i = 0; i< Num_packets; i++){
            number[i].total_frag = Num_packets;
            number[i].frag_no = i+1;
            if(i == Num_packets-1){
                number[i].size = length%1000;
            }
            else{
                number[i].size = 1000;
            }
            number[i].filename = "Text.txt";
            if(i == Num_packets-1 && (length <= 1001)){
                strcpy(number[i].filedata , extractSubstring(buffer, 0, length-1 ));
            }
            else if(i == Num_packets-1){
                strcpy(number[i].filedata , extractSubstring(buffer, i*1000, length-1 ));
            }
            else{
                strcpy(number[i].filedata , extractSubstring(buffer, i*1000, (i+1)*1000-1 ));
            }
        }        
        fclose(file);
    } 
    printf("%d", Num_packets);
    for(int j = 0; j < Num_packets; j++){
        printf("\n\n\nNum_paket: %d\nPacket_num: %d\nPacket_size: %d\nFilename: %s\nfile_data: %s\n",number[j].total_frag, number[j].frag_no, number[j].size, number[j].filename, number[j].filedata );
    }
   

};