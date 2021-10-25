#include "src/instructions.h"
#include "src/types.h"
#include<stdlib.h>
#include<stdio.h>
#include<inttypes.h>

#ifndef DEBUG
#define DEBUG 0
#endif
#define debug_print(fmt, ...) do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); } while (0)


uint16_t *mem;
// 16bit system gives 65536 (2^16) addressable memory locations.
// mem[i] refers to a word whereas malloc takes bytes
static const int MEM_SIZE_BYTES = 65536 * sizeof(uint16_t);
static const int MEM_SIZE_WORDS = 65536;
//uint16_t registers[32];
uint16_t *registers;

void load_binary_into_memory(char file[]) {
    printf("File: %s\n", file);
    FILE *init_file;
    unsigned int addr;
    init_file = fopen(file, "rb");
    if (init_file == NULL) {
        printf("Error: The file %s could not be opened\n", file);
        exit(1);
    }

    fread(&addr, 2, 1, init_file);
    addr = __builtin_bswap16(addr);

    fread(mem+addr, MEM_SIZE_BYTES, 1, init_file);

    if (ferror(init_file)) {
        printf("\n Error reading from file\n\n");
        exit(1);
    }

    fclose(init_file);
}

void dump_memory_to_binary() {
    FILE *mem_dump_file;
    mem_dump_file = fopen("dump.bin", "wb");
    fwrite(mem, MEM_SIZE_BYTES, 1, mem_dump_file);
    fclose(mem_dump_file);
}

int main(int argc, char *argv[]) {
    registers = malloc(sizeof(uint16_t)*32);
    if (registers == NULL) {
        printf("Error: Unable to allocate %ld bytes of memory", 32*sizeof(uint16_t));
        return 1;
    }
    struct RegistersAddressMapStruct rams = createRegistersAddressMap(registers);

    mem = malloc(MEM_SIZE_BYTES);
    if (mem == NULL) {
        printf("Error: Unable to allocate %d bytes of memory", MEM_SIZE_BYTES);
        return 1;
    }

    for (int i=1; i<argc; i++) {
        load_binary_into_memory(argv[i]);
    }

    // TODO Check wether we need to do this
    for (unsigned int i=0; i<MEM_SIZE_WORDS; i++) {
        mem[i] = __builtin_bswap16(mem[i]);
    }

    // ##############################################
    
    // Tests
    /*
    *rams.adr = 12;
    lod(0, 0, &rams, mem);
    printf("res: %d = 18533\n", *rams.res);

    *rams.res = 22373;
    sav(0, 0, &rams, mem);

    *rams.r0 = 50000;
    cmp(28, 0, &rams, mem);
    printf("Cmp res: %d = 1\n", *rams.res);

    *rams.adr = 5;
    *rams.ctr = 0;
    jmp(0, 0, &rams, mem);
    printf("Jump %d = 5\n", *rams.ctr);
    // /Tests
    */

    // ##############################################

    *rams.ctr = 0;
    uint16_t running = 1;
    while (running) {
        uint16_t operation = mem[*rams.ctr];
        uint16_t opcode = (operation & 0xFC00) >> 10; // First 6 bits
        uint16_t arg1 = (operation & 0x3E0) >> 5;
        uint16_t arg2 = operation & 0x1F;

        debug_print("\n\nRunning instruction at %d\n", *rams.ctr);
        debug_print("Operation: %d\n", operation);
        debug_print("Opcode: %d\n", opcode);
        debug_print("Arg1: %d\n", arg1);
        debug_print("Arg2: %d\n", arg2);

        switch (opcode) {
            case 0:
                running = 0;
                break;
            case 1:
                lod(arg1, arg2, &rams, mem);
                break;
            case 2:
                sav(arg1, arg2, &rams, mem);
                break;
            case 3:
                mov(arg1, arg2, &rams, mem);
                break;
            case 8:
                cmp(arg1, arg2, &rams, mem);
                break;
            case 9:
                jmp(arg1, arg2, &rams, mem);
                break;
            case 10:
                jeq(arg1, arg2, &rams, mem);
                break;
            case 11:
                jne(arg1, arg2, &rams, mem);
                break;
            case 12:
                jlt(arg1, arg2, &rams, mem);
                break;
            case 13:
                jle(arg1, arg2, &rams, mem);
                break;
            case 14:
                jgt(arg1, arg2, &rams, mem);
                break;
            case 15:
                jge(arg1, arg2, &rams, mem);
                break;
            case 16:
                add(arg1, arg2, &rams, mem);
                break;
            case 17:
                neg(arg1, arg2, &rams, mem);
                break;
            case 18:
                sfl(arg1, arg2, &rams, mem);
                break;
            case 19:
                sfr(arg1, arg2, &rams, mem);
                break;
            case 20:
                and(arg1, arg2, &rams, mem);
                break;
            case 21:
                orr(arg1, arg2, &rams, mem);
                break;
            case 22:
                xor(arg1, arg2, &rams, mem);
                break;
            case 23:
                not(arg1, arg2, &rams, mem);
                break;
            case 24:
                swp(arg1, arg2, &rams, mem);
                break;
        }

        if ((operation & 0b1000000000000000) != 0) {
            debug_print("Importing value %d to res\n", ~(operation & 0b1000000000000000));
            uint16_t imported = operation & ~(operation & 0b1000000000000000);
            *rams.res = imported;
        }

        
        debug_print("r00: %d\n", *rams.r0);
        debug_print("r01: %d\n", *rams.r1);
        debug_print("r02: %d\n", *rams.r2);
        debug_print("r03: %d\n", *rams.r3);
        debug_print("r04: %d\n", *rams.r4);
        debug_print("r05: %d\n", *rams.r5);
        debug_print("r06: %d\n", *rams.r6);
        debug_print("r07: %d\n", *rams.r7);
        debug_print("r08: %d\n", *rams.r8);
        debug_print("r09: %d\n", *rams.r9);
        debug_print("ctr: %d\n", *rams.ctr);
        debug_print("adr: %d\n", *rams.adr);
        debug_print("res: %d\n", *rams.res);

        #if DEBUG != 0 
        printf("\n\nHit enter to continue> ");
        getchar();
        printf("\n");
        #endif

        // TODO Make it so if jump is called this is one less. Maybe change jump to jump - 1 in that function
        *rams.ctr = *rams.ctr + 1;
    }

    // ##############################################

    for (unsigned int i=0; i<MEM_SIZE_WORDS; i++) {
        mem[i] = __builtin_bswap16(mem[i]);
    }
    dump_memory_to_binary();
}