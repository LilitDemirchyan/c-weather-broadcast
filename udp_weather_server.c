#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 8080
#define BROADCAST_IP "192.168.35.255"

void get_temp(char* buffer, size_t buf_size){
    char* weather_cond[] = {"Windy", "Stormy", "Rainy", "Sunny", "Cloudy"};
    int temp = rand() % 41 + 10;
    snprintf(buffer, buf_size, "Weather is %s, and temperature is %d", weather_cond[rand() % 5], temp);
}

int main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket open failed");
        exit(1);
    }
    srand(time(NULL));
    struct sockaddr_in broadcastaddr;
    char weather_info[100];
    int broadcast_enable = 1;
    memset(&broadcastaddr, 0, sizeof(broadcastaddr));
    int opt = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable));
    if(opt < 0){
        perror("setsockopt failed");
        exit(1);
    }
    broadcastaddr.sin_family = AF_INET;
    broadcastaddr.sin_port = htons(PORT);
    broadcastaddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    while(1){
        get_temp(weather_info, sizeof(weather_info));
        int send = sendto(sockfd, weather_info, strlen(weather_info), 0, (struct sockaddr*)&broadcastaddr, sizeof(broadcastaddr));
        if(send < 0){
            perror("sendto failed");
            exit(1);
        }
        printf("Broadcasted: %s", weather_info);
        sleep(5);
    }
    close(sockfd);
}
