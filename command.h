#ifndef COMMAND_H
#define COMMAND_H

#define MEM_SIZE 100

typedef unsigned char uint8;
typedef unsigned short uint16;

int commandEncode(int command, int operand, uint16* value);
uint16 makeValue(uint16 val);
uint16 remValue(uint16 val);

#endif // COMMAND_H
