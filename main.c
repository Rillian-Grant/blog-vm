#include<stdlib.h>
#include<stdio.h>

unsigned short int *mem;
// 16bit system gives 65536 (2^16) addressable memory locations.
// mem[i] refers to a word whereas malloc takes bytes
static const int MEM_SIZE_BYTES = 65536 * sizeof(unsigned short int);
static const int MEM_SIZE_WORDS = 65536;

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

    // Main code

    for (unsigned int i=0; i<MEM_SIZE_WORDS; i++) {
        mem[i] = __builtin_bswap16(mem[i]);
    }
    dump_memory_to_binary();
}