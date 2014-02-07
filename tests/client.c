/**
 * Example echo server
 *
 * This is the client script
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/socket.h"

int main()
{
    char buff[100];
    
    // Instantiate a socket object
    Socket *s = init_socket(AF_INET, SOCK_STREAM);
    
    // Connect to the server
    s->connect(s, "127.0.0.1", 12345);
    
    while (1) {
        memset(buff, 0, sizeof(buff));
        
        printf(">> ");
        
        // Get user input
        if (fgets(buff, sizeof(buff), stdin) == NULL) {
            perror("fgets error");
            break;
        }
        
        // Send the user to the server
        s->send(s, buff);
        
        // Get response from the server
        s->recv(s, buff, sizeof(buff));
        
        printf("Received: %s\n", buff);
    }
    
    // Destroy the socket object
    socket_close(s);
}
