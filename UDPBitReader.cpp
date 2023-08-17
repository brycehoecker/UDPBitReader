#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void printBits(unsigned char byte) {
    for (int i = 7; i >= 0; --i) {
        std::cout << ((byte >> i) & 1);
    }
    std::cout << " ";
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        char buffer[BUFFER_SIZE] = {0};
        socklen_t len = sizeof(clientAddr);

        // Receive data
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &len);
        if (n < 0) {
            std::cerr << "Error in recvfrom" << std::endl;
            break;
        }

        std::cout << "Received packet from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
        
        for (int i = 0; i < n; ++i) {
            printBits(buffer[i]);
        }
        std::cout << std::endl;
    }
    
    for (int i = 0; i < n; ++i) {
    printBits(buffer[i]);
    }

    close(sockfd);
    return 0;
}
