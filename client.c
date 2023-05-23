#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "SERVER_IP_ADDRESS"
#define PORT 12345

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char message[2000];

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    printf("Initialized.\n");

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to remote server
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server\n");

    // Receive and send messages
    while (1) {
        printf("Client: ");
        fgets(message, sizeof(message), stdin);

        // Send message to server
        if (send(client_socket, message, strlen(message), 0) < 0) {
            printf("Send failed\n");
            return 1;
        }

        // Receive response from server
        memset(message, 0, sizeof(message));
        if (recv(client_socket, message, sizeof(message), 0) < 0) {
            printf("Receive failed\n");
            break;
        }

        printf("Server: %s", message);
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
