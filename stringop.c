#include "stringop.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int readSatLine(FILE* fd, char* buff)
{
    char sym;
    int i = 0;
    while(1) {
        sym = fgetc(fd);
        if (sym == EOF)
            break;
        if (sym == '\n')
            break;
        if (sym == ';') {
            while ((sym = fgetc(fd)) != '\n')
                continue;
            break;
        }
        buff[i++] = sym;
    }
    buff[i] = '\0';
    return i;
}

static int getNum(char* str, int* num)
{
    int i = 0;
    char numBuff[8];
    while (str[i] != ' ' && str[i] != '\0') {
        if (!isdigit(str[i])) {
            return -1;
        }
        numBuff[i] = str[i];
        i++;
    }
    if (i == 0)
        return -1;
    numBuff[i] = '\0';
    *num = atoi(numBuff);
    return i;
}

static int getName(char* str, char* name)
{
    int i = 0;
    while (str[i] != ' ') {
        if (isdigit(str[i]) || str[i] == '\n')
            return -1;
        name[i] = str[i];
        i++;
    }
    if (i == 0)
        return -1;
    name[i] = '\0';
    return i;
}

#define cmp(s1, s2) (!strcmp(s1, s2))
#define ccmp(s) cmp(command, s)

static int choiseCommand(char* command)
{
    if (ccmp("READ")) {
        return 10;
    } else if (ccmp("WRITE")) {
        return 11;
    } else if (ccmp("LOAD")) {
        return 20;
    } else if (ccmp("STORE")) {
        return 21;
    } else if (ccmp("ADD")) {
        return 30;
    } else if (ccmp("SUB")) {
        return 31;
    } else if (ccmp("DIVIDE")) {
        return 32;
    } else if (ccmp("MUL")) {
        return 33;
    } else if (ccmp("JUMP")) {
        return 40;
    } else if (ccmp("JNEG")) {
        return 41;
    } else if (ccmp("JZ")) {
        return 42;
    } else if (ccmp("HALT")) {
        return 43;
    } else if (ccmp("MOVR")) {
        return 72;
    } else if (ccmp("=")) {
        return 0;
    }
    return -1;
}

Command* satParse(char* str)
{
    int memNum, command, operand;
    int retVal = getNum(str, &memNum);
    if (retVal == -1) {
        fprintf(stderr, "Неверный номер памяти\n");
        return NULL;
    }
    while (str[retVal] == ' ') {
        retVal++;
        continue;
    }

    char commName[40];
    int retVal2 = getName(str + retVal, commName);
    if (retVal == -1) {
        fprintf(stderr, "Неверная инструкция\n");
        return NULL;
    }
    retVal += retVal2;
    command = choiseCommand(commName);
    while (str[retVal] == ' ') {
        retVal++;
        continue;
    }

    if (command != 43) {
        retVal = getNum(str + retVal, &operand);
        if (retVal == -1) {
            fprintf(stderr, "Неверный операнд\n");
            return NULL;
        }
    } else {
        operand = 0;
    }

    return newCommand(memNum, command, operand);
}

Command* newCommand(int memNum, int command, int operand)
{
    Command* c = malloc(sizeof(*c));
    if (c == NULL)
        return c;
    c->memNum = memNum;
    c->command = command;
    c->operand = operand;

    return c;
}

void deleteCommand(Command* c)
{
    free(c);
}
