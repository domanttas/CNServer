#include <stdio.h>
#include "Utils.h"

#define PORT "9034"

int main() {
    //Master's descriptors list
    fd_set master;

    //Temporary descriptor list
    fd_set read_fds;

    //Max descriptors
    int fdmax;

    //Listening socket's descriptor
    int listener;

    //Accepted socket's descriptor
    int newfd;

    //Client address
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;

    //Data
    char buffer[256];
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    //Setsockopt()
    int yes = 1;

    //Loops
    int i, j;

    //Receiver
    int rv;

    struct addrinfo hints, *ai, *p;

    //Clear sets
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    /*
     * Create socket and bind it
     */
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "Server: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p -> ai_next) {
        listener = socket(p -> ai_family, p -> ai_socktype, p-> ai_protocol);
        if (listener < 0) {
            continue;
        }

        //For address already in use error
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p -> ai_addr, p -> ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    //Failed to bind
    if (p == NULL) {
        fprintf(stderr, "Server: failed to bind\n");
        exit(1);
    }

    freeaddrinfo(ai);

    /*
     * Listening
     */

    if (listen(listener, 5) < 0) {
        perror("Error while listening\n");
        exit(1);
    }

    //Adding listener to master list
    FD_SET(listener, &master);

    //Biggest descriptor
    fdmax = listener;

    for(;;) {

    }

    return 0;
}