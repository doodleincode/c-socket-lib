c-socket-lib
============

A socket library written in C that models after the Python socket API. This library is only supported on Linux.

This class models the Python socket API. If you can create a socket in Python, you can just as easily create one using this class!

How to use this library
-------------

Instantiate the socket object:

    Socket *s = init_socket(AF_INET, SOCK_STREAM);

Where <code>AF_INET</code> is the address family, and <code>SOCK_STREAM</code> is the socket type.

Then either <code>connect</code> to act as a client, or <code>bind</code>, <code>listen</code>, and <code>accept</code> to act as a server.

Use <code>send</code> and <code>recv</code> to send and receive data to/from the client/server.

When you're done, destroy the object:

    socket_close(s);

Sample echo server
-------------

These examples are also in the tests/ directory.

The server:

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "socket.h"
    
    int main()
    {
        char buff[100];
        int bytes;
    
        Socket *s = init_socket(AF_INET, SOCK_STREAM);
        s->bind(s, "127.0.0.1", 12345);
        s->listen(s, 5);
        
        ClientAddr client_addr;
        Socket *c = s->accept(s, &client_addr);
        
        printf("Client connected at %s:%d\n", client_addr.ip_addr, client_addr.port);
        
        while (1) {
            c->recv(c, buff, sizeof(buff));
            printf("Got: %s\n", buff);
            c->send(c, buff);
            printf("Sending: %s\n", buff);
        }
        
        socket_close(c);
        socket_close(s);
    }

The client:

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "socket.h"
    
    int main()
    {
        int bytes;
        char buff[100];
        Socket *s = init_socket(AF_INET, SOCK_STREAM);
        s->connect(s, "127.0.0.1", 12345);
        
        while (1) {
            memset(buff, 0, sizeof(buff));
            
            printf(">> ");
            
            if (fgets(buff, sizeof(buff), stdin) == NULL) {
                perror("fgets error");
                break;
            }
            
            s->send(s, buff);
            s->recv(s, buff, sizeof(buff));
            
            printf("Received: %s\n", buff);
        }
        
        socket_close(s);
    }
