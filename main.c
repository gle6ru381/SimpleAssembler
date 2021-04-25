#include <stdio.h>
#include "command.h"
#include "stringop.h"
#include <malloc.h>

int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Неверные аргументы\n");
        return -1;
    }
    FILE* in, *out;
    in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "Ошибка при открытии файла\n");
        return -1;
    }
    out = fopen(argv[2], "wb");
    if (out == NULL) {
        fprintf(stderr, "Ошибка при создании файла\n");
        return -1;
    }

    char* strLine = malloc(sizeof(char) * 1000);
    Command** commArray = calloc(MEM_SIZE, sizeof (Command*));
    int strNum = 1;
    while (!feof(in)) {
        int retVal = readSatLine(in, strLine);
        if (retVal == 0)
            continue;
        Command* comm = satParse(strLine);
        if (comm == NULL) {
            fprintf(stderr, "Ошибка на строке: %d\n", strNum);
            return -1;
        }
        if (comm->memNum >= MEM_SIZE) {
            fprintf(stderr, "Неправильная ячейка памяти на строке: %d\n", strNum);
            return -1;
        }
        commArray[comm->memNum] = comm;
        strNum++;
    }

    uint16* memory = calloc(MEM_SIZE, sizeof (uint16));
    for (int i = 0; i < MEM_SIZE; i++) {
        if (commArray[i] == NULL) {
            memory[i] = 0;
        } else {
            Command* comm = commArray[i];
            if (comm->command == 0) {
                if (comm->operand > (uint16)~(1 << 15)) {
                    fprintf(stderr, "Неправильная переменная по адресу: %d\n", i);
                    return -1;
                }
                memory[i] = makeValue(comm->operand);
            } else {
                int retVal = commandEncode(comm->command, comm->operand, &memory[i]);
                if (retVal != 0) {
                    fprintf(stderr, "Неправильная команда по адресу: %d\n", i);
                    return -1;
                }
            }
        }
    }
    fwrite(memory, sizeof(uint16), MEM_SIZE, out);
    return 0;
}
