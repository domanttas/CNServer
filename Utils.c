//
// Created by domantas on 19.2.7.
//

#include <ctype.h>
#include "Utils.h"

/*
 * Gets socket's address, whether it's IPV4 or IPV6
 */
void *get_in_addr(struct sockaddr *sa) {
    if (sa -> sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa) -> sin_addr);
    }

    return &(((struct sockaddr_in6*)sa) -> sin6_addr);
}


int calculatedAnswer(int firstNum, int secondNum, char operator) {
    int answer;

    if (operator == '+') {
        answer = firstNum + secondNum;
    } else if (operator == '-') {
        answer = firstNum - secondNum;
    } else if (operator == '*') {
        answer = firstNum * secondNum;
    } else if (operator == '/') {
        answer = firstNum / secondNum;
    }

    return answer;
}

int calculate(char buffer[]) {
    int i, j, z = 0;

    char firstNumber[128];
    char secondNumber[128];
    char operator;

    int firstNum;
    int secondNum;

    for (i = 0; i < 256; i++) {
        if (isdigit(buffer[i])) {
            firstNumber[i] = buffer[i];
        } else {
            operator = buffer[i];
            break;
        }
    }

    for (j = i + 1; j < 256; j++) {
        if (isdigit(buffer[j])) {
            secondNumber[z] = buffer[j];
            z++;
        } else {
            break;
        }
    }

    sscanf(firstNumber, "%d", &firstNum);
    sscanf(secondNumber, "%d", &secondNum);

    memset(firstNumber, 0, sizeof firstNumber);
    memset(secondNumber, 0, sizeof secondNumber);

    return calculatedAnswer(firstNum, secondNum, operator);
}

