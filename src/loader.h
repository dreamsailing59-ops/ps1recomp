#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>

typedef struct {
    uint32_t pc;          // Entry point
    uint32_t gp;          // Global pointer
    uint32_t dest_vaddr;  // RAM destination (usually 0x80010000)
    uint32_t file_size;   // Size of code
    uint8_t *body;        // Raw MIPS instructions
} PSX_Exe;

int load_psx_exe(const char *filename, PSX_Exe *out_exe);

#endif