#include "command.h"

union CommandForm {
    uint16 buff;
struct Command {
    uint8 operand : 7;
    uint8 operationCode : 7;
    uint8 commandB : 1;
} command;
};

static int validateCommand(int command)
{
    if (command < 10 || command > 73)
        return 0;
    switch(command) {
    case 12 ... 19:
    case 22 ... 29:
    case 34 ... 39:
    case 44 ... 50:
        return 0;
    default:
        return 1;
    }
}

static int validateOperand(int operand)
{
    uint8 mask = 1 << 7;
    if (operand >= mask || operand < 0)
        return 0;
    return 1;
}

#include <stdio.h>

int commandEncode(int command, int operand, uint16* value)
{
    if (!validateCommand(command))
        return -1;
    if (!validateOperand(operand))
        return -2;

    union CommandForm form;
    form.buff = 0;
    form.command.commandB = 0;
    form.command.operationCode = command;
    form.command.operand = operand;

    *value = (int)form.buff;
    return 0;
}

uint16 makeValue(uint16 val)
{
    union CommandForm form;
    form.buff = (uint16)val;
    form.command.commandB = 1;
    return form.buff;
}

uint16 remValue(uint16 val)
{
    union CommandForm form;
    form.buff = (uint16)val;
    form.command.commandB = 0;
    return form.buff;
}
