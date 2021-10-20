#include<stdlib.h>
#include<stdio.h>
#include<inttypes.h>

unsigned short int *mem;
// 16bit system gives 65536 (2^16) addressable memory locations.
// mem[i] refers to a word whereas malloc takes bytes
static const int MEM_SIZE_BYTES = 65536 * sizeof(unsigned short int);
static const int MEM_SIZE_WORDS = 65536;

void load_binary_into_memory(char file[]) {
    printf("File: %s\n", file);
    FILE *init_file;
    init_file = fopen(file, "rb");
    fread(mem, MEM_SIZE_BYTES, 1, init_file);
    fclose(init_file);
}

int main(int argc, char *argv[]) {
    mem = malloc(MEM_SIZE_BYTES);
    if (mem == NULL) {
        printf("Error: Unable to allocate %d bytes of memory", MEM_SIZE_BYTES);
        return 1;
    }

    for (int i=1; i<argc; i++) {
        load_binary_into_memory(argv[i]);
    }

    for (unsigned int i=0; i<MEM_SIZE_WORDS; i++) {
        mem[i] = __builtin_bswap16(mem[i]);
    }

    for (int i=0; i<MEM_SIZE_WORDS; i++) {
        printf("%d: %d\n", i, mem[i]);
    }

    for (unsigned int i=0; i<MEM_SIZE_WORDS; i++) {
        mem[i] = __builtin_bswap16(mem[i]);
    }
    FILE *mem_dump_file;
    mem_dump_file = fopen("dump.bin", "wb");
    fwrite(mem, MEM_SIZE_BYTES, 1, mem_dump_file);
    fclose(mem_dump_file);
}