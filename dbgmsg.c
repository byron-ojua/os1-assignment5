#include "dbgmsg.h"

/**
 * @brief creates a debugMessenger
 * 
 * @param names array of group names
 * @param numRows number of groups
 * @return struct debugMessenger 
 */
struct debugMessenger debugMessenger(char names[][50], int numRows){
    struct debugMessenger dbgMsger;
    char (*gpNames)[50] = malloc((numRows + 1) * sizeof(char[50]));
    int *groupStats = malloc((numRows + 1) * sizeof(int));

    strcpy(gpNames[0], "OVERRIDE");

    for(int i = 0; i < numRows; i++){
        strcpy(gpNames[i + 1], names[i]);
        groupStats[i + 1] = 0;
    }

    dbgMsger.numGroups = numRows;
    dbgMsger.groupStatus = groupStats;
    dbgMsger.groupNames = gpNames;
    dbgMsger.output = stdout;

    return dbgMsger;
}

/**
 * @brief Redirects debug message to new output
 * 
 * @param dbgMsger debugMessenger to change
 * @param out file for new output
 */
void dbgSetOutput(struct debugMessenger* dbgMsger, FILE* out){
    dbgMsger->output = out;
}

/**
 * @brief Prints a message to a provided output
 * 
 * @param msg Message to print
 * @param out outfile to print to
 */
void prtDbgMsg(char *msg, FILE* out){
    fprintf(out, "DEBUG MESSAGE: %s", msg);
    fflush(out);
}

/**
 * @brief Prints a message either if override is on or if individual group is enabled
 * 
 * @param msg Message to print
 * @param group Assigned group number for message
 * @param dbgMsger reference debugMessenger
 */
void prtDbgMsgGp(char *msg, int group, struct debugMessenger* dbgMsger){
    // If group is out of bounds, print error
    if (group < 1 || group > dbgMsger->numGroups){
        fprintf(stderr, "DEBUG MESSAGE: ERROR - Invalid debug group: %d\n", group);
        fflush(stderr);
    } else {
        // If override is on, print
        if(dbgMsger->groupStatus[0]){
            fprintf(dbgMsger->output, "DEBUG MESSAGE (%s - Override): %s", dbgMsger->groupNames[group], msg);
        // If debugGroup is enabled, print
        } else if(dbgMsger->groupStatus[group]){
            fprintf(dbgMsger->output, "DEBUG MESSAGE (%s): %s", dbgMsger->groupNames[group], msg);
        }
    }

    fflush(dbgMsger->output);
}

/**
 * @brief Enables a debug group to allow printing
 * 
 * @param group Group number to enable
 * @param dbgMsger reference debugMessenger
 */
void enblDbgGp(int group, struct debugMessenger* dbgMsger){
    // If group is out of bounds, print error
    if (group > dbgMsger->numGroups - 1){
        fprintf(stderr, "DEBUG MESSAGE: ERROR - Invalid debug group: %d\n", group);
        fflush(stderr);
    } else {
        dbgMsger->groupStatus[group] = 1;
    }
}

/**
 * @brief Disables a debug group from printing
 * 
 * @param group Group number to disable
 * @param dbgMsger reference debugMessenger
 */
void dsblDbgGp(int group, struct debugMessenger* dbgMsger){
    // If group is out of bounds, print error
    if (group > dbgMsger->numGroups - 1){
        fprintf(stderr, "DEBUG MESSAGE: ERROR - Invalid debug group: %d\n", group);
        fflush(stderr);
    } else {
        dbgMsger->groupStatus[group] = 0;
    }
}

/**
 * @brief Enables group override, so all debug messages will print regardless of individual group status
 * 
 * @param dbgMsger reference debugMessenger
 */
void enblDbgOride(struct debugMessenger* dbgMsger){
    dbgMsger->groupStatus[0] = 1;
}

/**
 * @brief Disables group override, so messages will only print if individual group is enabled
 * 
 * @param dbgMsger reference debugMessenger
 */
void dsblDbgOride(struct debugMessenger* dbgMsger){
    dbgMsger->groupStatus[1] = 0;
}

/**
 * @brief Frees memory from debugMessenger
 * 
 * @param dbgMsger reference debugMessenger
 */
void freeDbg(struct debugMessenger* dbgMsger){
    free(dbgMsger->groupNames);
    free(dbgMsger->groupStatus);
}