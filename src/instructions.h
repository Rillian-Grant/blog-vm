#ifndef FILE_INSTRUCTIONS_INCLUDED
#define FILE_INSTRUCTIONS_INCLUDED

#include "types.h"

#include<inttypes.h>
#include<stdio.h>

void lod(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    printf("Memory location %d reads: %d\n", *registers->adr, mem[*registers->adr]);
    *registers->res = mem[*registers->adr];
}

void sav(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    printf("Saving value %d to memory at %d\n", *registers->res, *registers->adr);
    mem[*registers->adr] = *registers->res;
}

void mov(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    printf("Move from %d to %d\n", arg2, arg1);
    registers->reg_array[arg2] = registers->reg_array[arg1];
}

void cmp(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    arg1 = registers->reg_array[arg1];
    arg2 = registers->reg_array[arg2];
        printf("CMP: %d and %d\n", arg1, arg2);

    if (arg1==arg2) {
        *registers->res = 0;
    } else if (arg1<arg2) {
        *registers->res = 1;
    } else if (arg1>arg2) {
        *registers->res = 2;
    }
}

void jmp(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->ctr = *registers->adr;
}

void jeq(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    if (*registers->res == 0) {
        *registers->ctr = *registers->adr;
    }
}

void jne(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    if (*registers->res != 0) {
        *registers->ctr = *registers->adr;
    }
}

void jlt(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    if (*registers->res == 1) {
        *registers->ctr = *registers->adr;
    }
}

void jle(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    if (*registers->res == 1 || *registers->res == 0) {
        *registers->ctr = *registers->adr;
    }
}

void jgt(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    if (*registers->res == 2) {
        *registers->ctr = *registers->adr;
    }
}

void jge(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    if (*registers->res == 2 || *registers->res == 0) {
        *registers->ctr = *registers->adr;
    }
}

void add(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = registers->reg_array[arg1] + registers->reg_array[arg2];
}

void neg(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = ~(registers->reg_array[arg1])+1;
}

// TODO use << and >> to shift
void sfl(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = (registers->reg_array[arg1])*2;
}

void sfr(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = (registers->reg_array[arg1])/2;
}

void and(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = registers->reg_array[arg1] & registers->reg_array[arg2];
}

void orr(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = registers->reg_array[arg1] | registers->reg_array[arg2];
}

void xor(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = registers->reg_array[arg1] ^ registers->reg_array[arg2];
}

void not(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    *registers->res = ~registers->reg_array[arg1];
}

void swp(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    uint16_t second_half = (registers->reg_array[arg1] & 0x00FF) << 8;
    uint16_t first_half = registers->reg_array[arg1] >> 8;
    *registers->res = first_half | second_half;
}

#endif