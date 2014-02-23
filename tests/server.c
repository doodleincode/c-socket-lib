/**
 * Example echo server
 *
 * This is the server script
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/socket.h"

int main()
{
    char buff[100];
    int bytes;
    
    // Instantiate a socket object
    Socket *s = init_socket(AF_INET, SOCK_STREAM);
    
    // Bind the socket to localhost on port 12345
    // To bind to a specific IP, replace NULL with the IP address,
    // for example: "192.168.1.123"
    s->bind(s, NULL, 12345);
    
    // Listen and allow up to five queued clients
    s->listen(s, 5);
    
    // Define a client address structure to get info about the connected client
    ClientAddr client_addr;
    
    // Accept clients
    Socket *c = s->accept(s, &client_addr);
    
    printf("Client connected at %s:%d\n", client_addr.ip_addr, client_addr.port);
    
    while (1) {
        memset(buff, 0, sizeof(buff));
        c->recv(c, buff, sizeof(buff));
        
        if (strlen(buff) == 0) {
            break;
        }
        
        printf("Got: %s\n", buff);
        c->send(c, buff);
        printf("Sending: %s\n", buff);
    }
    
    // Destroy the client socket object
    socket_close(c);
    
    // Destory the main socket object
    socket_close(s);
}
