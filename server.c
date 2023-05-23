#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 12345

int main() {
    int server_socket, client_socket, read_size;
    struct sockaddr_in server_addr, client_addr;
    char client_message[2000];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Could not create socket");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen
    listen(server_socket, 3);
    printf("Server is running. Waiting for a connection...\n");

    // Accept incoming connections
    int c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&c);
    if (client_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    printf("Connected to client\n");

    // Receive and send messages
    while ((read_size = recv(client_socket, client_message, 2000, 0)) > 0) {
        printf("Client: %s", client_message);

        printf("Server: ");
        fgets(client_message, 2000, stdin);

        // Send response to client
        write(client_socket, client_message, strlen(client_message));
    }

    if (read_size == 0) {
        printf("Client disconnected\n");
    } else if (read_size == -1) {
        perror("Receive failed");
    }

    close(client_socket);
    close(server_socket);

    return 0;
}
