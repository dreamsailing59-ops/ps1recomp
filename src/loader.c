#include "loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_psx_exe(const char *filename, PSX_Exe *out_exe) {
    FILE *f = fopen(filename, "rb");
    if (!f) return -1;

    uint8_t header[2048];
    if (fread(header, 1, 2048, f) != 2048) {
        fclose(f);
        return -2;
    }

    if (memcmp(header, "PS-X EXE", 8) != 0) {
        fclose(f);
        return -3;
    }

    // Header values are at specific offsets (Little Endian)
    out_exe->pc         = *(uint32_t*)&header[0x10];
    out_exe->gp         = *(uint32_t*)&header[0x14];
    out_exe->dest_vaddr = *(uint32_t*)&header[0x18];
    out_exe->file_size  = *(uint32_t*)&header[0x1C];

    out_exe->body = malloc(out_exe->file_size);
    fseek(f, 2048, SEEK_SET); 
    fread(out_exe->body, 1, out_exe->file_size, f);

    fclose(f);
    return 0;
}