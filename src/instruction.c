#include "instruction.h"

const char *instruction_mode_to_string(InstructionMode mode) {
    switch (mode) {
        case IM_IMP: return "IM_IMP";
        case IM_IMM: return "IM_IMM";
        case IM_ZP0: return "IM_ZP0";
        case IM_ZPX: return "IM_ZPX";
        case IM_ZPY: return "IM_ZPY";
        case IM_REL: return "IM_REL";
        case IM_ABS: return "IM_ABS";
        case IM_ABX: return "IM_ABX";
        case IM_ABY: return "IM_ABY";
        case IM_IND: return "IM_IND";
        case IM_IZX: return "IM_IZX";
        case IM_IZY: return "IM_IZY";
    }

    return "nul";
}