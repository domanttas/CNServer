//
// Created by domantas on 19.2.7.
//

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void *get_in_addr(struct sockaddr *);
int calculate(char []);

#endif //SERVER_UTILS_H
