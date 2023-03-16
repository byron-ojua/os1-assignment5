#ifndef HEADER_H
#define HEADER_H

#include "dbgmsg.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>

#define STOP_COMMAND "STOP\n"
#define LINE_SEPARATOR "\n"
#define MAX_LINE_LENGTH 1000
#define LINE_COUNT 50
#define OUTPUT_LINE_LENGTH 80

struct dataBuffer{
    char data[MAX_LINE_LENGTH];
    int size;
};

void put_buff_1(struct dataBuffer *buff);
void put_buff_2(struct dataBuffer *buff);
void put_buff_3(struct dataBuffer *buff);
void get_buff_1(struct dataBuffer *buff);
void get_buff_2(struct dataBuffer *buff);
void get_buff_3(struct dataBuffer *buff);
void get_user_input(struct dataBuffer *buff);
void *input_thread(void *arg);
void *separator_thread(void *arg);
void *plus_thread(void *arg);
void *output_thread(void *arg);

#endif
