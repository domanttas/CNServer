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

    char const_buffer[8];

    const_buffer[0] = 'S';
    const_buffer[1] = 'o';
    const_buffer[2] = 'c';
    const_buffer[3] = 'k';
    const_buffer[4] = 'e';
    const_buffer[5] = 't';
    const_buffer[7] = ' ';

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
        printf(stderr, "Server: %s\n", gai_strerror(rv));
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
        printf(stderr, "Server: failed to bind\n");
        exit(1);
    }

    freeaddrinfo(ai);

    /*
     * Listening
     */

    if (listen(listener, 5) < 0) {
        printf("Error while listening\n");
        exit(1);
    }

    //Adding listener to master list
    FD_SET(listener, &master);

    //Biggest descriptor
    fdmax = listener;

    for(;;) {
        //Copying
        read_fds = master;

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            printf("Error in select");
            exit(1);
        }

        //Looping through connections
        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                //There's connection
                if (i == listener) {
                    //Handling connection
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1) {
                        printf("Error in accepting connection");
                    } else {
                        //Adding to master list
                        FD_SET(newfd, &master);

                        if (newfd > fdmax) {
                            //Keeping maximum descriptor
                            fdmax = newfd;
                        }
                        printf("Server: new connection from %s on " "socket %d\n",
                                inet_ntop(remoteaddr.ss_family,
                                        get_in_addr((struct sockaddr*)&remoteaddr),
                                                remoteIP, INET6_ADDRSTRLEN),
                                                newfd);

                    }
                } else {
                    //Handling data
                    if ((nbytes = recv(i, buffer, sizeof buffer, 0)) <= 0) {
                        //Some error
                        if (nbytes == 0) {
                            //Connection was closed
                            printf("Server: socket %d lost connection\n", i);
                        } else {
                            printf("Error while receiving");
                        }
                        close(i);
                        //Removing it
                        FD_CLR(i, &master);
                    } else {
                        //Data received
                        for (j = 0; j <= fdmax; j++) {
                            //Sending data to every client
                            if (FD_ISSET(j, &master)) {
                                //Same client
                                if (j == i) {
                                    const_buffer[6] = i + '0';
                                    if (send(j, const_buffer, sizeof const_buffer, 0) == -1) {
                                        printf("Error while sending data out");
                                    }

                                    int answer = calculate(buffer);
                                    memset(buffer, 0, sizeof buffer);
                                    sprintf(buffer, "%d", answer);

                                    if (send(j, buffer, nbytes, 0) == -1) {
                                        printf("Error while sending data out");
                                    }

                                    memset(buffer, 0, sizeof buffer);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}