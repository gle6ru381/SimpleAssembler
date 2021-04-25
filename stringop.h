#ifndef STRINGOP_H
#define STRINGOP_H
#include <stdio.h>

typedef struct _Command {
    int memNum;
    int command;
    int operand;
} Command;

Command* newCommand(int memNum, int command, int operand);
void deleteCommand(Command* comm);

int readSatLine(FILE* fd, char* buff);
Command* satParse(char* str);

#endif // STRINGOP_H
