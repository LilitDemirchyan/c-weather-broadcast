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


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <time.h>

// #define PORT 8080
// #define BROADCAST_IP "192.168.35.255"
// #define MESSAGE_INTERVAL 5  

// void generate_weather_info(char *buffer, size_t buffer_size) {
//     const char *weather_conditions[] = {"Sunny", "Rainy", "Cloudy", "Windy", "Stormy"};
//     int temperature = rand() % 41 - 10;  
//     snprintf(buffer, buffer_size, "Weather: %s, Temp: %d°C", 
//              weather_conditions[rand() % 5], temperature);
// }

// int main() {
//     int sockfd;
//     struct sockaddr_in broadcast_addr;
//     int broadcast_enable = 1;
//     char weather_info[100];

//     srand(time(NULL));

//     if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
//         perror("Error setting broadcast option");
//         close(sockfd);
//         exit(EXIT_FAILURE);
//     }

//     memset(&broadcast_addr, 0, sizeof(broadcast_addr));
//     broadcast_addr.sin_family = AF_INET;
//     broadcast_addr.sin_port = htons(PORT);
//     broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

//     while (1) {
//         generate_weather_info(weather_info, sizeof(weather_info));

//         if (sendto(sockfd, weather_info, strlen(weather_info), 0, 
//                    (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
//             perror("Error broadcasting weather info");
//             close(sockfd);
//             exit(EXIT_FAILURE);
//         }

//         printf("Broadcasted: %s\n", weather_info);

//         sleep(MESSAGE_INTERVAL);
//     }

//     close(sockfd);
//     return 0;
// }
