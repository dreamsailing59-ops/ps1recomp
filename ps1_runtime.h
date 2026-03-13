#include <stdint.h>

typedef struct {
    uint32_t r[32];
    uint32_t pc;
    uint8_t ram[2048 * 1024]; // 2MB RAM
} CPU_Context;

// Fake memory handlers for now
static inline uint32_t mem_read32(uint32_t addr) { return 0; }
static inline void mem_write32(uint32_t addr, uint32_t val) {}
static inline void mem_write16(uint32_t addr, uint16_t val) {}
static inline void mem_write8(uint32_t addr, uint8_t val) {}