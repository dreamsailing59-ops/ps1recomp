#include "recompiler.h"
#include <stdio.h>

void recompile_instruction(FILE* out, uint32_t pc, uint32_t instr, uint32_t next_instr) {
    uint8_t opcode = instr >> 26;
    uint8_t rs = (instr >> 21) & 0x1F;
    uint8_t rt = (instr >> 16) & 0x1F;
    uint8_t rd = (instr >> 11) & 0x1F;
    uint16_t imm = instr & 0xFFFF;
    uint8_t funct = instr & 0x3F;

    fprintf(out, "    /* 0x%08X */ ", pc);

    // If the instruction is a NOP (all zeros), just comment it
    if (instr == 0) {
        fprintf(out, "    /* 0x%08X */ // NOP\n", pc);
        return;
    }

    switch (opcode) {
        case 0x00: // SPECIAL
            switch (funct) {
                case 0x00: fprintf(out, "    ctx->r[%d] = ctx->r[%d] << %d;\n", rd, rt, (instr >> 6) & 0x1F); break;
                case 0x02: fprintf(out, "    ctx->r[%d] = ctx->r[%d] >> %d;\n", rd, rt, (instr >> 6) & 0x1F); break;
                case 0x08: fprintf(out, "    ctx->pc = ctx->r[%d]; return;\n", rs); break; // JR
                case 0x21: fprintf(out, "    ctx->r[%d] = ctx->r[%d] + ctx->r[%d];\n", rd, rs, rt); break;
                case 0x24: fprintf(out, "    ctx->r[%d] = ctx->r[%d] & ctx->r[%d];\n", rd, rs, rt); break;
                case 0x25: fprintf(out, "    ctx->r[%d] = ctx->r[%d] | ctx->r[%d];\n", rd, rs, rt); break;
                case 0x2B: fprintf(out, "    ctx->r[%d] = ((int32_t)ctx->r[%d] < (int32_t)ctx->r[%d]);\n", rd, rs, rt); break;
                default:   fprintf(out, "    // Unknown SPECIAL 0x%02x\n", funct); break;
            }
            break;

        case 0x03: // JAL (Jump and Link)
            // Fix: Hardcode the return address instead of using 'pc' variable
            fprintf(out, "    {\n        ctx->r[31] = 0x%08X;\n", pc + 8);
            recompile_instruction(out, pc + 4, next_instr, 0); 
            fprintf(out, "        ctx->pc = 0x%08X; return;\n    }\n", (pc & 0xF0000000) | ((instr & 0x3FFFFFF) << 2));
            break;

        case 0x05: // BNE
            fprintf(out, "    {\n        ");
            // Delay slot executes first
            recompile_instruction(out, pc + 4, next_instr, 0); 
            uint32_t target = pc + 4 + ((int16_t)imm << 2);
            fprintf(out, "        if (ctx->r[%d] != ctx->r[%d]) { ctx->pc = 0x%08X; return; }\n    }\n", rs, rt, target);
            break;

        case 0x08: // ADDI
        case 0x09: // ADDIU
            fprintf(out, "    ctx->r[%d] = ctx->r[%d] + %d;\n", rt, rs, (int16_t)imm); 
            break;

        case 0x0F: // LUI
            fprintf(out, "    ctx->r[%d] = 0x%08X;\n", rt, imm << 16); 
            break;

        case 0x23: // LW (Load Word)
            // Fix: Pass 'ctx' as the first argument
            fprintf(out, "    ctx->r[%d] = mem_read32(ctx, ctx->r[%d] + %d);\n", rt, rs, (int16_t)imm); 
            break;

        case 0x2B: // SW (Store Word)
            // Fix: Pass 'ctx' as the first argument
            fprintf(out, "    mem_write32(ctx, ctx->r[%d] + %d, ctx->r[%d]);\n", rs, (int16_t)imm, rt); 
            break;

        default:
            fprintf(out, "    // Unknown Opcode 0x%02x\n", opcode);
            break;
    }
}