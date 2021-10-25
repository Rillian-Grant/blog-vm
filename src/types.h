#ifndef FILE_TYPES_INCLUDED
#define FILE_TYPES_INCLUDED

#include "instructions.h"

#include<inttypes.h>
#include<stdlib.h>

struct RegistersAddressMapStruct {
    uint16_t *ctr;
    uint16_t *adr;
    uint16_t *res;
    uint16_t *r0;
    uint16_t *r1;
    uint16_t *r2;
    uint16_t *r3;
    uint16_t *r4;
    uint16_t *r5;
    uint16_t *r6;
    uint16_t *r7;
    uint16_t *r8;
    uint16_t *r9;
    
    
    uint16_t *reg_array;
};

struct RegistersAddressMapStruct createRegistersAddressMap(uint16_t registers[]) {
    struct RegistersAddressMapStruct r;

    r.ctr=&registers[31];
    r.adr=&registers[30];
    r.res=&registers[28];
    r.r0=&registers[0];
    r.r1=&registers[1];
    r.r2=&registers[2];
    r.r3=&registers[3];
    r.r4=&registers[4];
    r.r5=&registers[5];
    r.r6=&registers[6];
    r.r7=&registers[7];
    r.r8=&registers[8];
    r.r9=&registers[9];

    r.reg_array=registers;

    return r;
}

/*
static void (* const opcode_table[64]) (uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) = {
    slp,
    lod,
    sav,
    NULL,
    NULL,
    NULL,
    NULL,
    cmp,
    jmp,
    jeq,
    jne,
    jlt,
    jle,
    jgt,
    jge,
    add,
    neg,
    sfl,
    sfr,
    and,
    orr,
    not,
    swp,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};
*/

#endif