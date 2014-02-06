//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*
Python API:

host = ''
port = 50000
backlog = 5
size = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host,port))
s.listen(backlog)
while 1:
    client, address = s.accept()
    data = client.recv(size)
    if data:
        client.send(data)
    client.close() 

*/



// class macros header
#define NEW(_CLASS_) ((_CLASS_*)malloc(sizeof(_CLASS_)))
#define FUNC(_TYPE_, _NAME_, _PARAM_) _TYPE_ (*_NAME_) _PARAM_
#define CLASS( _NAME_, _BODY_ ) typedef struct _NAME_ { _BODY_ } _NAME_

void release(void *ptr)
{
    free(ptr);
}

// class header

typedef struct {
    const char *ip_addr;
    int port;
} ClientAddr;

CLASS(Socket, 
    FUNC(void, connect, (struct Socket *, const char *, int));
    FUNC(void, bind, (struct Socket *, const char *, int));
    FUNC(void, listen, (struct Socket *, int));
    FUNC(struct Socket *, accept, (struct Socket *, ClientAddr *));
    FUNC(int, send, (struct Socket *, const char *));
    FUNC(int, recv, (struct Socket *, char *, int));
    
    // Private properties and methods
    int __sockfd__;
    int __family__;
    int __type__;
    int __proto__;
    
    //FUNC(int, __create__, (struct Socket *, struct addrinfo *));
);

Socket *init_socket(int, int);

void socket_close(Socket *);

// class implementation

/*static int _create(struct Socket *s, struct addrinfo *)
{
    // TODO: implementation
}*/

static void _connect(Socket *s, const char *ip_addr, int port)
{
    // TODO: implementation
}

static void _bind(Socket *s, const char *ip_addr, int port)
{
    // TODO: implementation
}

static void _listen(Socket *s, int max_clients)
{
    // TODO: implementation
}

static Socket *_accept(Socket *s, ClientAddr *c)
{
    // TODO: implementation
    
    if (c) {
        // Return client struct with client ip and port
    }
    
    return init_socket(0, 0);
}

static int _send(Socket *s, const char *str)
{
    // TODO: implementation
    
    return 0;
}

static int _recv(Socket *s, char *buff, int max_len)
{
    // TODO: implementation
    
    return 0;
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
    //s->__create__ = _create;
    
    // Give initial values to our struct member
    s->__sockfd__ = -1;
    s->__family__ = 0;
    s->__type__ = 0;
    s->__proto__ = 0;
    
    // TODO: implementation
    
    return s;
}

void socket_close(Socket *s)
{
    // TODO: implementation
    
    release(s);
}

// Program
int main()
{
    char buff[1024];
    Socket *s = init_socket(AF_INET, SOCK_STREAM);
    
    s->bind(s, "127.0.0.1", 12345);
    s->listen(s, 10);
    
    while (1) {
        Socket *c = s->accept(s, NULL);
        c->recv(c, buff, sizeof(buff));
        printf("%s\n", buff);
        socket_close(c);
    }
    
    socket_close(s);
    
    return 0;
}


