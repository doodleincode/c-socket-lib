/**
 * Socket pseudo-class
 *
 * @author 	Daniel Hong
 * 
 * GNU GENERAL PUBLIC LICENSE Version 2. A LICENSE file should have
 * accompanied this library.
 */

#define NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include "../include/socket.h"

static void _connect(Socket *this, const char *ip_addr, int port)
{
    // Setup and create the socket
    this->__create__(this, SOCKET_CLIENT, ip_addr, port);
    
    if (connect(this->__sockfd__, this->__addrinfo__->ai_addr, 
                    this->__addrinfo__->ai_addrlen) == -1) {
    	perror("Error connecting to the remote server.");
    	exit(1);
    }
    
    // We don't need our addrinfo structure anymore
    freeaddrinfo(this->__addrinfo__);
}

static void _bind(Socket *this, const char *ip_addr, int port)
{
    // Setup and create the socket
    this->__create__(this, SOCKET_SERVER, ip_addr, port);
    
    if (bind(this->__sockfd__, this->__addrinfo__->ai_addr, 
                this->__addrinfo__->ai_addrlen) == -1) {
    	perror("Error binding the socket.");
    	exit(1);
    }
    
    // We don't need our addrinfo structure anymore
    freeaddrinfo(this->__addrinfo__);
}

static void _listen(Socket *this, int max_queue)
{
    if (listen(this->__sockfd__, max_queue) == -1) {
    	perror("Error listening for connections.");
    	exit(1);
    }
}

static Socket *_accept(Socket *this, ClientAddr *c)
{
    struct sockaddr_in client_addr;
    unsigned addr_size = sizeof(client_addr);
    int client_fd;
    
    if ((client_fd = accept(this->__sockfd__, (struct sockaddr *)&client_addr, &addr_size)) == -1) {
		perror("Error accepting a remote connection.");
		exit(1);
	}
    
    if (c) {
        // Return client struct with client ip and port
        c->ip_addr = this->__inet_ntop__(this, (struct sockaddr *)&client_addr);
        c->port = client_addr.sin_port;
    }
    
    // Return a socket object initialized with our client socket
    Socket *sock = init_socket(this->__family__, this->__type__);
    sock->__sockfd__ = client_fd;
    return sock;
}

static int _send(Socket *this, const char *str)
{
    return send(this->__sockfd__, str, strlen(str), 0);
}

// TODO This function is unsafe!!!
// Need to figure out a better way to do it
static int _recv(Socket *this, char *buff, int max_len)
{
    int bytes_recv;
    char tmp[max_len];
    
    // Clear buffer
    memset(&tmp, 0, max_len);
    memset(buff, 0, max_len);
    
    bytes_recv = recv(this->__sockfd__, tmp, max_len, 0);
    strncpy(buff, tmp, max_len);
    
    return bytes_recv;
}

// This is a "private" method
static void _create(Socket *this, int type, const char *ip_addr, int port)
{
    char sport[6];
    
    // We need to convert the port number to a string
    if (snprintf(sport, 6, "%d", port) < 0) {
        printf("Error converting port number of type integer to string.");
        exit(1);
    }
    
    // If the type of socket we are creating is for binding/listening, we need
    // to make a few changes
    if (type == SOCKET_SERVER) {
        this->__addrinfo_hints__.ai_flags = AI_PASSIVE;
        ip_addr = NULL;
    }
    
    // Get our addrinfo structure to use to create the socket
    // We use getaddrinfo() because it correctly populates our structure 
    // regardless of IPv4 or IPv6
    if (getaddrinfo(ip_addr, sport, &(this->__addrinfo_hints__), &(this->__addrinfo__)) != 0) {
    	perror("Error getting address info.");
    	exit(1);
    }
    
    // Finally we can create the socket
    if ((this->__sockfd__ = socket(this->__addrinfo__->ai_family, 
                                this->__addrinfo__->ai_socktype, 
                                this->__addrinfo__->ai_protocol)) == -1) {
    	perror("Error creating a socket.");
    	exit(1);
    }
}

// This is a "private" method
static char *_inet_ntop(Socket *this, struct sockaddr *sa)
{
    static char str[39];
    
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), str, INET_ADDRSTRLEN);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), str, INET6_ADDRSTRLEN);
            break;

        default:
            strncpy(str, "Unknown", INET_ADDRSTRLEN);
            return NULL;
    }
    
    return str;
}

Socket *init_socket(int family, int type)
{
    Socket *s = NEW(Socket);
    assert(s);  // Make sure it's good. This will be disabled for production
    
    // Map the function pointers to our "class" members
    s->connect = _connect;
    s->bind = _bind;
    s->listen = _listen;
    s->accept = _accept;
    s->send = _send;
    s->recv = _recv;
    s->__create__ = _create;
    s->__inet_ntop__ = _inet_ntop;
    
    // Give initial values to our struct members
    s->__sockfd__ = -1;
    s->__family__ = family;
    s->__type__ = type;
    s->__proto__ = 0;   // Unused right now, for future
    
    // Zero out our addrinfo hints structure
    memset(&(s->__addrinfo_hints__), 0, sizeof(s->__addrinfo_hints__));
    
    // Set initial values for our hints
    s->__addrinfo_hints__.ai_family = family;
    s->__addrinfo_hints__.ai_socktype = type;
    
    return s;
}

void socket_close(Socket *s)
{
    // Make sure it hadn't been freed already
    if (s) {
        // Close the socket
        close(s->__sockfd__);
        
        // Release the structure from memory
        release(s);
    }
}
