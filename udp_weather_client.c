#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_BUF_SIZE 1024

int main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket opening failed");
        close(sockfd);
        exit(1);
    }
    int reuse = 1;
    char buffer[MAX_BUF_SIZE];
    struct sockaddr_in listenaddr;
    socklen_t addrlen = sizeof(listenaddr);
    int opt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if(opt < 0){
        perror("setsockopt failed");
        exit(1);
    }
    memset(&listenaddr, 0, sizeof(listenaddr));
    listenaddr.sin_family = AF_INET;
    listenaddr.sin_addr.s_addr = INADDR_ANY;
    listenaddr.sin_port = htons(PORT);
    int bindd = bind(sockfd, (struct sockaddr*)&listenaddr, sizeof(listenaddr));
    if(bindd < 0){
        perror("bind failed");
        exit(1);
    }
    printf("Client listening weather for port %d...\n", PORT);
    int recv = recvfrom(sockfd, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*)&listenaddr, &addrlen);
    if(recv < 0){
        perror("recvfrom failed");
        close(sockfd);
        exit(1);
    }
    buffer[recv] = '\0';
    printf("Recieved %s", buffer);
    close(sockfd);
    return 0;
}