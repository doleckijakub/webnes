#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "inttypes.h"

typedef enum {
	IM_IMP, // implied
	IM_IMM, // immediate
	IM_ZP0, // zero page
	IM_ZPX, // zero page with x offset
	IM_ZPY, // zero page with y offset
	IM_REL, // relative
	IM_ABS, // absolute
	IM_ABX, // absolute with x offset
	IM_ABY, // absolute with y offset
	IM_IND, // indirect
	IM_IZX, // indirect x
	IM_IZY, // indirect y
} InstructionMode;

const char *instruction_mode_to_string(InstructionMode mode);

typedef struct {
	const char *name;
	uint8_t (*operand)(void);
	InstructionMode mode;
	uint8_t cycles;
} Instruction;

#endif