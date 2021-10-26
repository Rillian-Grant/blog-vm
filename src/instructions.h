#ifndef FILE_INSTRUCTIONS_INCLUDED
#define FILE_INSTRUCTIONS_INCLUDED

#include "debug.h"
#include "mappings.h"

#include<inttypes.h>
#include<stdio.h>

// This macro prints the debug messages (if debugging is enabled) for each instruction. If the instruction takes arguments then these are printed.
#define debug_messages(num_of_arguments) \
debug_print("Instruction called\n"); \
if (num_of_arguments >= 1) debug_print("Argument 1 is register %d with contents %d\n", arg1, registers->reg_array[arg1]); \
if (num_of_arguments == 2) debug_print("Argument 2 is register %d with contents %d\n", arg2, registers->reg_array[arg2]);

// ################################################################################

// 000001 (1) Loads the word from memory at position adr to res
void lod(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    debug_print("Memory location %d reads: %d\n", *registers->adr, mem[*registers->adr]);
    *registers->res = mem[*registers->adr];
}

// 000010 (2) Saves the word in res to memory at position adr
void sav(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    debug_print("Saving value %d to memory at %d\n", *registers->res, *registers->adr);
    mem[*registers->adr] = *registers->res;
}

// 000011 (3) Move the word in the register represented by the first argument to the one represented by the second
void mov(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(2);
    registers->reg_array[arg2] = registers->reg_array[arg1];
}

// 001000 (8) Compares the first argument with the second. See the if statement for results
void cmp(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(2);
    arg1 = registers->reg_array[arg1];
    arg2 = registers->reg_array[arg2];

    if (arg1==arg2) {
        *registers->res = 0;
    } else if (arg1<arg2) {
        *registers->res = 1;
    } else if (arg1>arg2) {
        *registers->res = 2;
    }
}

// 001001 (9) Reduces adr by one and copys it to ctr
void jmp(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    *registers->ctr = *registers->adr - 1;
}

// 001010 (10) jmp if res == 0
void jeq(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    if (*registers->res == 0) {
        *registers->ctr = *registers->adr - 1;
    }
}

// 001011 (11) jmp if res != 0
void jne(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    if (*registers->res != 0) {
        *registers->ctr = *registers->adr - 1;
    }
}

// 001100 (12) jmp if res == 1
void jlt(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    if (*registers->res == 1) {
        *registers->ctr = *registers->adr - 1;
    }
}

// 001101 (13) jmp if res == 1 or 0
void jle(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    if (*registers->res == 1 || *registers->res == 0) {
        *registers->ctr = *registers->adr - 1;
    }
}

// 001110 (14) jmp if res == 2
void jgt(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    if (*registers->res == 2) {
        *registers->ctr = *registers->adr - 1;
    }
}

// 001111 (15) jmp if res == 2 or 0
void jge(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(0);
    if (*registers->res == 2 || *registers->res == 0) {
        *registers->ctr = *registers->adr - 1;
    }
}

// 010000 (16) add arguments, result in res
void add(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(2);
    *registers->res = registers->reg_array[arg1] + registers->reg_array[arg2];
}

// 010001 (17) Negate first argument, result in res
void neg(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(1);
    *registers->res = ~(registers->reg_array[arg1])+1;
}

// 010010 (18) Shift bits left in first argument, result in res
// TODO use << and >> to shift
void sfl(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(1);
    *registers->res = (registers->reg_array[arg1])*2;
}

// 010011 (19) Shift bits right in first argument, result in res
void sfr(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(1);
    *registers->res = (registers->reg_array[arg1])/2;
}

// 010100 (20) Bitwise and, result in res
void and(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(2);
    *registers->res = registers->reg_array[arg1] & registers->reg_array[arg2];
}

// 010101 (21) Bitwise or, result in res
void orr(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(2);
    *registers->res = registers->reg_array[arg1] | registers->reg_array[arg2];
}

// 010110 (22) Bitwise xor, result in res
void xor(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(2);
    *registers->res = registers->reg_array[arg1] ^ registers->reg_array[arg2];
}

// 010111 (23) Bitwise not on first argument, result in res
void not(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(1);
    *registers->res = ~registers->reg_array[arg1];
}

// 011000 (24) Swap first and last bytes of word from first argument, result in res
void swp(uint16_t arg1, uint16_t arg2, struct RegistersAddressMapStruct *registers, uint16_t *mem) {
    debug_messages(1);
    uint16_t second_half = (registers->reg_array[arg1] & 0x00FF) << 8;
    uint16_t first_half = registers->reg_array[arg1] >> 8;
    *registers->res = first_half | second_half;
}

#endif