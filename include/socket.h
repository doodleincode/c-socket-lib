/**
 * Socket pseudo-class
 *
 * This class models the Python socket API. If you can create a socket in Python,
 * you can just as easily create one using this class!
 *
 * @author 	Daniel Hong
 * 
 * This program is licensed under the GNU GENERAL PUBLIC LICENSE Version 2.
 * A LICENSE file should have accompanied this program.
 */

#ifndef _DH_SOCKET_H_
#define _DH_SOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../../c-functions/include/class.h"

#define SOCKET_SERVER 1
#define SOCKET_CLIENT 2

/**
 * Structure to hold info on a connected client's IP and port
 */
typedef struct {
    const char *ip_addr;
    int port;
} ClientAddr;

/**
 * Defining the socket class
 */
CLASS(Socket,
    /**
     * Connect to a server
     *
     * @param   Socket *        IN      Pointer to the socket object created by init_socket(int, int)
     * @param   const char *    IN      IP address of the server to connect to
     * @param   int             IN      Port of the remote server
     */
    FUNC(void, connect, (struct Socket *, const char *, int));
    
    /**
     * Bind a socket to the given IP address and port
     *
     * @param   Socket *        IN      Pointer to the socket object created by init_socket(int, int)
     * @param   const char *    IN      IP address of the server to connect to
     * @param   int             IN      Port of the remote server
     */
    FUNC(void, bind, (struct Socket *, const char *, int));
    
    /**
     * Listen for incoming connections
     *
     * @param   Socket *        IN      Pointer to the socket object created by init_socket(int, int)
     * @param   int             IN      Max number of queued connection allowed
     */
    FUNC(void, listen, (struct Socket *, int));
    
    /**
     * Accept a connection. The socket must be bound to an address and listening for connections
     *
     * @param   Socket *        IN          Pointer to the socket object created by init_socket(int, int)
     * @param   ClientAddr *    OUT/OPT     Pointer to a ClientAddr structure.
     *                                      If used, this will populate the structure with
     *                                      the connected client's IP and port
     * @return  Socket *                    Returns a pointer to a Socket object where it
     *                                      is used to send and receive data on the connection
     */
    FUNC(struct Socket *, accept, (struct Socket *, ClientAddr *));
    
    /**
     * Send data to the connected remote socket
     *
     * @param   Socket *        IN      Pointer to the socket object created by init_socket(int, int)
     * @param   const char *    IN      The data to send
     * @return  int                     If successful, returns the number of bytes sent.
     *                                  Returns -1 on error.
     *                                  Applications are responsible for checking that all data has been
     *                                  sent. If only some of the data was transmitted, the application
     *                                  needs to attempt delivery of the remaining data.
     */
    FUNC(int, send, (struct Socket *, const char *));
    
    /**
     * Receive data from the connected remote socket
     *
     * @param   Socket *    IN      Pointer to the socket object created by init_socket(int, int)
     * @param   char *      OUT     Pointer to a character buffer. This buffer will be 
     *                              populated with the received data.
     * @return  int                 If successful, returns the number of bytes sent.
     *                              Returns -1 on error, zero (0) if the peer performed a shutdown 
     */
    FUNC(int, recv, (struct Socket *, char *, int));
    
    // Below are private members
    // These members should generally not be used by the end-user
    // and as such will not be documented
    
    int _sockfd;
    int _family;
    int _type;
    int _proto;
    struct addrinfo _addrinfo_hints;
    struct addrinfo *_addrinfo;
    
    FUNC(void, _create, (struct Socket *, int, const char *, int));
    FUNC(void, _exit_error, (struct Socket *, const char *));
    FUNC(char *, _inet_ntop, (struct Socket *, struct sockaddr *));
);

/**
 * Instantiate a socket object and return a pointer to said object
 * Think of this as the "new" keyword in OOP languages like C++ and JAVA where
 * the object is created in the heap. Since it's in the heap, you must call the
 * socket_close(obj) function on every object instantiation.
 *
 * @param   int         Address family. Can be AF_INET, AF_INET6, or AF_UNIX
 * @param   int         The socket type. Can be SOCK_STREAM or SOCK_DGRAM
 * @return  Socket *    A pointer to a Socket object
 *
 * Example:
 *
 *    Socket *sock = init_socket(AF_INET, SOCK_STREAM);
 *
 *    // Do stuff with sock
 *    // ...
 *
 *    // Close the socket
 *    socket_close(sock);
 */
Socket *init_socket(int, int);

/**
 * Closes an open socket. This function also releases the object from memory.
 *
 * @param   Socket *    Pointer to a socket to close
 */
void socket_close(Socket *);

// -----------------------------------------------------------------------------
//  Function pointers. These are private members and cannot be called directly
// -----------------------------------------------------------------------------

static void _connect_(Socket *, const char *, int);
static void _bind_(Socket *, const char *, int);
static void _listen_(Socket *, int);
static Socket *_accept_(Socket *, ClientAddr *);
static int _send_(Socket *, const char *);
static int _recv_(Socket *, char *, int);
static void _create_(Socket *, int, const char *, int);
static void _exit_error_(Socket *, const char *);
static char *_inet_ntop_(Socket *, struct sockaddr *);

#endif /* _DH_SOCKET_H_ */

