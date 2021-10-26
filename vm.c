#include "src/debug.h"
#include "src/instructions.h"
#include "src/mappings.h"
#include<stdlib.h>
#include<stdio.h>
#include<inttypes.h>

// Constants that can be editied:
static const char DUMP_FILE_NAME[] = "dump.bin";

// Don't change these global and constant variables. If you reduce the memory size programs may go out of bounds as there is no mechanism for restricting memory access.
uint16_t *mem;
// 16bit system gives 65536 (2^16) addressable memory locations.
// mem[i] refers to a word whereas malloc takes bytes
static const int MEM_SIZE_WORDS = 65536;
static const int MEM_SIZE_BYTES = MEM_SIZE_WORDS * sizeof(uint16_t); // 65536 * 2
uint16_t *registers;

uint16_t correct_endianness_of__uint16_t__if_needed(uint16_t value) {
    if (__BYTE_ORDER == __ORDER_LITTLE_ENDIAN__) {
        value = __builtin_bswap16(value);
        return value;
    } else if (__BYTE_ORDER == __ORDER_BIG_ENDIAN__) {
        // All okay
        return value;
    } else {
        fprintf(stderr, "You either have a compiler that doesn't support __BYTE_ORDER__, or you are running on a system that doesn't use either big or little endian.");
        exit(1);
    }
}

// Loads a binary file into memory starting at the memory location represented by the first two bytes of the file
// TODO Add out of bounds check
void load_binary_into_memory(char file[]) {
    FILE *init_file;
    uint16_t addr;
    init_file = fopen(file, "rb");
    if (init_file == NULL) {
        fprintf(stderr, "Error: The file %s could not be opened\n", file);
        exit(1);
    }

    fread(&addr, 2, 1, init_file);
    addr = correct_endianness_of__uint16_t__if_needed(addr);

    debug_print("Reading binary %s to memory starting at position %d\n", file, addr);

    fread(mem+addr, MEM_SIZE_BYTES, 1, init_file);

    if (ferror(init_file)) {
        fprintf(stderr, "\n Error reading from file\n\n");
        exit(1);
    }

    fclose(init_file);
}

void dump_memory_to_binary() {
    FILE *mem_dump_file;
    mem_dump_file = fopen(DUMP_FILE_NAME, "wb");
    if (mem_dump_file == NULL) {
        fprintf(stderr, "Error: The file %s could not be opened\n", DUMP_FILE_NAME);
        exit(1);
    }

    debug_print("Dumping memory to file %s\n", DUMP_FILE_NAME);

    fwrite(mem, MEM_SIZE_BYTES, 1, mem_dump_file);
    fclose(mem_dump_file);
}

int main(int argc, char *argv[]) {
    // Allocate memory for the registers. The registers are stored in an array where there position corresponds to their number 
    registers = malloc(sizeof(uint16_t)*32);
    if (registers == NULL) {
        fprintf(stderr, "Error: Unable to allocate %ld bytes of memory", 32*sizeof(uint16_t));
        exit(1);
    }
    // Generate a human readable mapping from the register names (which are going to be used in the assambally compiler) to their positions in the array
    struct RegistersAddressMapStruct rams = createRegistersAddressMap(registers);

    // Allocate memory for the RAM
    mem = malloc(MEM_SIZE_BYTES);
    if (mem == NULL) {
        fprintf(stderr, "Error: Unable to allocate %d bytes of memory", MEM_SIZE_BYTES);
        exit(1);
    }

    // For every command line argument load the specified file into memory
    for (int i=1; i<argc; i++) {
        load_binary_into_memory(argv[i]);
    }

    // If needed change the endianness of the memory
    for (unsigned int i=0; i<MEM_SIZE_WORDS; i++) {
        mem[i] = correct_endianness_of__uint16_t__if_needed(mem[i]);
    }

    // ################################################################################

    debug_print("Start execution\n");
    // Set program counter to zero and start the execution
    *rams.ctr = 0;
    uint16_t running = 1;
    while (running) {
        // Extract the current operation and extract the opcode and arguments from it by applying bitmasks
        uint16_t operation = mem[*rams.ctr];
        uint16_t opcode = (operation & 0b1111110000000000) >> 10; // First 6 bits
        uint16_t arg1 = (operation & 0b0000001111100000) >> 5;
        uint16_t arg2 = operation & 0b0000000000011111;

        debug_print("Running instruction at memory location %d\n", *rams.ctr);
        debug_print("Operation: %d\n", operation);
        debug_print("Opcode: %d\n", opcode);

        // Run the correct instruction based on the opcode specified
        // TODO Change switch/case into function pointer array with some way to get the function's name
        switch (opcode) {
            case 0:
                // End the loop
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

        // If the first bit in the operation is 1 then set it to zero and load it into the res register
        if ((operation & 0b1000000000000000) != 0) {
            uint16_t imported = operation & ~(operation & 0b1000000000000000);
            debug_print("Importing value %d to register res\n", imported);
            *rams.res = imported;
        }

        debug_print("Register values after operation:\n");
        debug_print("ctr: %d\n", *rams.ctr); // May be different than before operation.
        debug_print("adr: %d\n", *rams.adr);
        debug_print("res: %d\n", *rams.res);   
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

        // If in debug mode, wait for the user to press enter before moving to the next instruction
        #if DEBUG != 0 
        printf("\n\nHit enter to continue> ");
        getchar();
        printf("\n");
        #endif

        // Increment the program counter
        // TODO Make it so if jump is called this is one less. Maybe change jump to jump - 1 in that function
        *rams.ctr = *rams.ctr + 1;
    }
    debug_print("System Halted\n");

    // ##############################################

    // Put the endianness back to how it was before dumping the memory
    for (unsigned int i=0; i<MEM_SIZE_WORDS; i++) {
        mem[i] = correct_endianness_of__uint16_t__if_needed(mem[i]);
    }
    dump_memory_to_binary();
}