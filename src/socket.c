/**
 * Socket pseudo-class
 *
 * @author 	Daniel Hong
 * 
 * This program is licensed under the GNU GENERAL PUBLIC LICENSE Version 2.
 * A LICENSE file should have accompanied this program.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <errno.h>
#include "../include/socket.h"

Socket *init_socket(int family, int type)
{
    Socket *s;
    
    if ((s = malloc(sizeof(Socket))) == NULL) {
        return NULL;
    }
    
    // Map the the function pointers
    s->connect = _connect_;
    s->bind = _bind_;
    s->listen = _listen_;
    s->accept = _accept_;
    s->send = _send_;
    s->recv = _recv_;
    s->_create = _create_;
    s->_inet_ntop = _inet_ntop_;
    s->_exit_error = _exit_error_;
    
    // Give initial values to our members
    s->_sockfd = -1;
    s->_family = family;
    s->_type = type;
    s->_proto = 0;   // Unused right now, for future
    
    // Zero out our addrinfo hints structure
    memset(&(s->_addrinfo_hints), 0, sizeof(s->_addrinfo_hints));
    
    // Set initial values for our hints
    s->_addrinfo_hints.ai_family = family;
    s->_addrinfo_hints.ai_socktype = type;
    
    return s;
}

void socket_close(Socket *s)
{
    // Make sure it hadn't been freed already
    if (s) {
        // Close the socket
        close(s->_sockfd);
        
        // Release the structure from memory
        release(s);
    }
}

static void _connect_(Socket *this, const char *ip_addr, int port)
{
    // Setup and create the socket
    this->_create(this, SOCKET_CLIENT, ip_addr, port);
    
    if (connect(this->_sockfd, this->_addrinfo->ai_addr, 
                    this->_addrinfo->ai_addrlen) == -1) {
    	this->_exit_error(this, "Error connecting to the remote server.");
    }
    
    // We don't need our addrinfo structure anymore
    freeaddrinfo(this->_addrinfo);
}

static void _bind_(Socket *this, const char *ip_addr, int port)
{
    // Setup and create the socket
    this->_create(this, SOCKET_SERVER, ip_addr, port);
    
    if (bind(this->_sockfd, this->_addrinfo->ai_addr, 
                this->_addrinfo->ai_addrlen) == -1) {
        this->_exit_error(this, "Error binding the socket.");
    }
    
    // We don't need our addrinfo structure anymore
    freeaddrinfo(this->_addrinfo);
}

static void _listen_(Socket *this, int max_queue)
{
    if (listen(this->_sockfd, max_queue) == -1) {
    	this->_exit_error(this, "Error listening for connections.");
    }
}

static Socket *_accept_(Socket *this, ClientAddr *c)
{
    struct sockaddr_in client_addr;
    unsigned addr_size = sizeof(client_addr);
    int client_fd;
    
    if ((client_fd = accept(this->_sockfd, (struct sockaddr *)&client_addr, &addr_size)) == -1) {
		this->_exit_error(this, "Error accepting a remote connection.");
	}
    
    if (c) {
        // Return client struct with client ip and port
        c->ip_addr = this->_inet_ntop(this, (struct sockaddr *)&client_addr);
        c->port = client_addr.sin_port;
    }
    
    // Return a socket object initialized with our client socket
    Socket *sock = init_socket(this->_family, this->_type);
    sock->_sockfd = client_fd;
    return sock;
}

static int _send_(Socket *this, const char *str)
{
    return send(this->_sockfd, str, strlen(str), 0);
}

static int _recv_(Socket *this, char *buff, int max_len)
{
    int bytes_recv;
    char tmp[max_len];
    
    // Clear buffer
    memset(tmp, 0, max_len);
    memset(buff, 0, max_len);
    
    bytes_recv = recv(this->_sockfd, tmp, max_len, 0);
    snprintf(buff, max_len, "%s", tmp);
    
    return bytes_recv;
}

// Create a socket
// This will handle both client and server socket creation
static void _create_(Socket *this, int type, const char *ip_addr, int port)
{
    char sport[6];
    
    // We need to convert the port number to a string
    if (snprintf(sport, 6, "%d", port) < 0) {
        this->_exit_error(this, "Error converting port number to string.");
    }
    
    // If the type of socket we are creating is for binding/listening, we need
    // to make a few changes
    if (type == SOCKET_SERVER) {
        this->_addrinfo_hints.ai_flags = AI_PASSIVE;
    }
    
    // Get our addrinfo structure to use to create the socket
    // We use getaddrinfo() because it correctly populates our structure 
    // regardless of IPv4 or IPv6
    if (getaddrinfo(ip_addr, sport, &(this->_addrinfo_hints), &(this->_addrinfo)) != 0) {
    	this->_exit_error(this, "Error getting address info.");
    }
    
    // Finally we can create the socket
    if ((this->_sockfd = socket(this->_addrinfo->ai_family, 
                                this->_addrinfo->ai_socktype, 
                                this->_addrinfo->ai_protocol)) == -1) {
    	this->_exit_error(this, "Error creating a socket.");
    }
}

static void _exit_error_(Socket *this, const char *msg)
{
    socket_close(this);
    
    // Print with error code message
    if (errno != 0) {
        printf ("(%s) %s\n", strerror(errno), msg);
        exit(1);
    }
    
    // Just print our message
    printf("%s\n", msg);
    exit(1);
}

// Get human friendly IP address
// This function uses inet_ntop() as it supports IPv4 & IPv6
static char *_inet_ntop_(Socket *this, struct sockaddr *sa)
{
    static char str[INET6_ADDRSTRLEN];
    
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

