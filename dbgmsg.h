#ifndef DBGMSG_H
#define DBGMSG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct debugMessenger {
    int numGroups;
    char (*groupNames)[50];
    FILE *output;
    int *groupStatus;
};

// struct debugMessenger *debugMessenger(char names[][50], int numRows);
struct debugMessenger debugMessenger(char names[][50], int numRows);
void prtDbgMsg(char *msg, FILE* out);
void prtDbgMsgGp(char *msg, int group, struct debugMessenger* dbgMsger);
void enblDbgGp(int group, struct debugMessenger* dbgMsger);
void dsblDbgGp(int group, struct debugMessenger* dbgMsger);
void enblDbgOride(struct debugMessenger* dbgMsger);
void dsblDbgOride(struct debugMessenger* dbgMsger);
void dbgSetOutput(struct debugMessenger* dbgMsger, FILE* output);
void freeDbg(struct debugMessenger* dbgMsger);

#endif
