#include "r6502.h"

#include "libc.h"

#include "ram.h"

// static variables

static uint8_t a, x, y; // accumulator, x, y registers
static uint8_t sp, st; // stack pointer, status
static uint16_t pc; // program counter
static uint8_t fetched, opcode, cycles; // alu input, instruction, clock cycles required for operation
static uint16_t temp, abs_addr, rel_addr, clock_count; // often used help variable, absolute address, relative address, global clock counter

#include "instruction.h"

// instruction modes

static uint8_t im(InstructionMode mode) { // instruction modes
	switch (mode)
	{
	case IM_IMP:
		fetched = a;
		return 0;
	case IM_IMM:
		abs_addr = pc++;
		return 0;
	case IM_ZP0:
		abs_addr = ram_read_u8(pc++);
		return 0;
	case IM_ZPX:
		abs_addr = ram_read_u8(pc++) + x;
		return 0;
	case IM_ZPY:
		abs_addr = ram_read_u8(pc++) + y;
		return 0;
	case IM_REL:
		rel_addr = ram_read_u8(pc++);
		if (rel_addr & 0x80) rel_addr |= 0xFF00;
		return 0;
	case IM_ABS:
		abs_addr = ram_read_u16(pc);
		pc += 2;
		return 0;
	case IM_ABX:
		temp = ram_read_u16(pc);
		abs_addr = temp + x;
		pc += 2;
		return (abs_addr & 0xFF00) != (temp & 0xFF00);
	case IM_ABY:
		temp = ram_read_u16(pc);
		abs_addr = temp + y;
		pc += 2;
		return (abs_addr & 0xFF00) != (temp & 0xFF00);
		return 0;
	case IM_IND:
		temp = ram_read_u16(pc);
		pc += 2;
		if ((temp & 0x00FF) == 0x00FF) {
			abs_addr = (ram_read_u8(temp & 0xFF) << 8) | ram_read_u8(temp); // page boundary bug
		} else {
			abs_addr = (ram_read_u8(temp + 1) << 8) | ram_read_u8(temp);
		}
		return 0;
	case IM_IZX:
		temp = ram_read_u8(pc++);
		abs_addr = ram_read_u16(temp + x);
		return 0;
	case IM_IZY:
		temp = ram_read_u8(pc++);
		abs_addr = ram_read_u16(temp + y);
		return 0;
	default:
		printfln("unreachable: %s:%d", __FILE__, __LINE__);
		return 0;
	}
}

// processor instructions

#define UNIMPLEMENTED_PROCESSOR_INSTRUCTION() do { \
	printfln("warning: %s:%d: unimplemented processor instruction: %s", __FILE__, __LINE__, __func__); \
	return 0; \
} while (0)

static uint8_t BRK(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t ORA(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t NOP(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t ASL(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t PHP(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BPL(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t CLC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t JSR(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t AND(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BIT(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t ROL(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t PLP(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BMI(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t SEC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t RTI(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t EOR(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t LSR(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t PHA(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t JMP(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BVC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t CLI(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t RTS(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t ADC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t ROR(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t PLA(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BVS(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t SEI(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t STA(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t STY(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t STX(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t DEY(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t TXA(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BCC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t TYA(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t TXS(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t LDY(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t LDA(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t LDX(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t TAY(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t TAX(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BCS(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t CLV(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t TSX(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t CPY(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t CMP(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t DEC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t INY(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t DEX(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BNE(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t CLD(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t CPX(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t SBC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t INC(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t INX(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t BEQ(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }
static uint8_t SED(void) { UNIMPLEMENTED_PROCESSOR_INSTRUCTION(); }

static uint8_t NUL(void) { return 0; } // same as NOP

// instruction map

static Instruction instructions[] = {
	(Instruction) { "BRK", BRK, IM_IMM, 7 },
	(Instruction) { "ORA", ORA, IM_IZX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 3 },
	(Instruction) { "ORA", ORA, IM_ZP0, 3 },
	(Instruction) { "ASL", ASL, IM_ZP0, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "PHP", PHP, IM_IMP, 3 },
	(Instruction) { "ORA", ORA, IM_IMM, 2 },
	(Instruction) { "ASL", ASL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "ORA", ORA, IM_ABS, 4 },
	(Instruction) { "ASL", ASL, IM_ABS, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "BPL", BPL, IM_REL, 2 },
	(Instruction) { "ORA", ORA, IM_IZY, 5 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "ORA", ORA, IM_ZPX, 4 },
	(Instruction) { "ASL", ASL, IM_ZPX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "CLC", CLC, IM_IMP, 2 },
	(Instruction) { "ORA", ORA, IM_ABY, 4 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "ORA", ORA, IM_ABX, 4 },
	(Instruction) { "ASL", ASL, IM_ABX, 7 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "JSR", JSR, IM_ABS, 6 },
	(Instruction) { "AND", AND, IM_IZX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "BIT", BIT, IM_ZP0, 3 },
	(Instruction) { "AND", AND, IM_ZP0, 3 },
	(Instruction) { "ROL", ROL, IM_ZP0, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "PLP", PLP, IM_IMP, 4 },
	(Instruction) { "AND", AND, IM_IMM, 2 },
	(Instruction) { "ROL", ROL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "BIT", BIT, IM_ABS, 4 },
	(Instruction) { "AND", AND, IM_ABS, 4 },
	(Instruction) { "ROL", ROL, IM_ABS, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "BMI", BMI, IM_REL, 2 },
	(Instruction) { "AND", AND, IM_IZY, 5 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "AND", AND, IM_ZPX, 4 },
	(Instruction) { "ROL", ROL, IM_ZPX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "SEC", SEC, IM_IMP, 2 },
	(Instruction) { "AND", AND, IM_ABY, 4 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "AND", AND, IM_ABX, 4 },
	(Instruction) { "ROL", ROL, IM_ABX, 7 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "RTI", RTI, IM_IMP, 6 },
	(Instruction) { "EOR", EOR, IM_IZX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 3 },
	(Instruction) { "EOR", EOR, IM_ZP0, 3 },
	(Instruction) { "LSR", LSR, IM_ZP0, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "PHA", PHA, IM_IMP, 3 },
	(Instruction) { "EOR", EOR, IM_IMM, 2 },
	(Instruction) { "LSR", LSR, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "JMP", JMP, IM_ABS, 3 },
	(Instruction) { "EOR", EOR, IM_ABS, 4 },
	(Instruction) { "LSR", LSR, IM_ABS, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "BVC", BVC, IM_REL, 2 },
	(Instruction) { "EOR", EOR, IM_IZY, 5 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "EOR", EOR, IM_ZPX, 4 },
	(Instruction) { "LSR", LSR, IM_ZPX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "CLI", CLI, IM_IMP, 2 },
	(Instruction) { "EOR", EOR, IM_ABY, 4 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "EOR", EOR, IM_ABX, 4 },
	(Instruction) { "LSR", LSR, IM_ABX, 7 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "RTS", RTS, IM_IMP, 6 },
	(Instruction) { "ADC", ADC, IM_IZX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 3 },
	(Instruction) { "ADC", ADC, IM_ZP0, 3 },
	(Instruction) { "ROR", ROR, IM_ZP0, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "PLA", PLA, IM_IMP, 4 },
	(Instruction) { "ADC", ADC, IM_IMM, 2 },
	(Instruction) { "ROR", ROR, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "JMP", JMP, IM_IND, 5 },
	(Instruction) { "ADC", ADC, IM_ABS, 4 },
	(Instruction) { "ROR", ROR, IM_ABS, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "BVS", BVS, IM_REL, 2 },
	(Instruction) { "ADC", ADC, IM_IZY, 5 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "ADC", ADC, IM_ZPX, 4 },
	(Instruction) { "ROR", ROR, IM_ZPX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "SEI", SEI, IM_IMP, 2 },
	(Instruction) { "ADC", ADC, IM_ABY, 4 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "ADC", ADC, IM_ABX, 4 },
	(Instruction) { "ROR", ROR, IM_ABX, 7 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "STA", STA, IM_IZX, 6 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "STY", STY, IM_ZP0, 3 },
	(Instruction) { "STA", STA, IM_ZP0, 3 },
	(Instruction) { "STX", STX, IM_ZP0, 3 },
	(Instruction) { "---", NUL, IM_IMP, 3 },
	(Instruction) { "DEY", DEY, IM_IMP, 2 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "TXA", TXA, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "STY", STY, IM_ABS, 4 },
	(Instruction) { "STA", STA, IM_ABS, 4 },
	(Instruction) { "STX", STX, IM_ABS, 4 },
	(Instruction) { "---", NUL, IM_IMP, 4 },
	(Instruction) { "BCC", BCC, IM_REL, 2 },
	(Instruction) { "STA", STA, IM_IZY, 6 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "STY", STY, IM_ZPX, 4 },
	(Instruction) { "STA", STA, IM_ZPX, 4 },
	(Instruction) { "STX", STX, IM_ZPY, 4 },
	(Instruction) { "---", NUL, IM_IMP, 4 },
	(Instruction) { "TYA", TYA, IM_IMP, 2 },
	(Instruction) { "STA", STA, IM_ABY, 5 },
	(Instruction) { "TXS", TXS, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "---", NOP, IM_IMP, 5 },
	(Instruction) { "STA", STA, IM_ABX, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "LDY", LDY, IM_IMM, 2 },
	(Instruction) { "LDA", LDA, IM_IZX, 6 },
	(Instruction) { "LDX", LDX, IM_IMM, 2 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "LDY", LDY, IM_ZP0, 3 },
	(Instruction) { "LDA", LDA, IM_ZP0, 3 },
	(Instruction) { "LDX", LDX, IM_ZP0, 3 },
	(Instruction) { "---", NUL, IM_IMP, 3 },
	(Instruction) { "TAY", TAY, IM_IMP, 2 },
	(Instruction) { "LDA", LDA, IM_IMM, 2 },
	(Instruction) { "TAX", TAX, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "LDY", LDY, IM_ABS, 4 },
	(Instruction) { "LDA", LDA, IM_ABS, 4 },
	(Instruction) { "LDX", LDX, IM_ABS, 4 },
	(Instruction) { "---", NUL, IM_IMP, 4 },
	(Instruction) { "BCS", BCS, IM_REL, 2 },
	(Instruction) { "LDA", LDA, IM_IZY, 5 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "LDY", LDY, IM_ZPX, 4 },
	(Instruction) { "LDA", LDA, IM_ZPX, 4 },
	(Instruction) { "LDX", LDX, IM_ZPY, 4 },
	(Instruction) { "---", NUL, IM_IMP, 4 },
	(Instruction) { "CLV", CLV, IM_IMP, 2 },
	(Instruction) { "LDA", LDA, IM_ABY, 4 },
	(Instruction) { "TSX", TSX, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 4 },
	(Instruction) { "LDY", LDY, IM_ABX, 4 },
	(Instruction) { "LDA", LDA, IM_ABX, 4 },
	(Instruction) { "LDX", LDX, IM_ABY, 4 },
	(Instruction) { "---", NUL, IM_IMP, 4 },
	(Instruction) { "CPY", CPY, IM_IMM, 2 },
	(Instruction) { "CMP", CMP, IM_IZX, 6 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "CPY", CPY, IM_ZP0, 3 },
	(Instruction) { "CMP", CMP, IM_ZP0, 3 },
	(Instruction) { "DEC", DEC, IM_ZP0, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "INY", INY, IM_IMP, 2 },
	(Instruction) { "CMP", CMP, IM_IMM, 2 },
	(Instruction) { "DEX", DEX, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "CPY", CPY, IM_ABS, 4 },
	(Instruction) { "CMP", CMP, IM_ABS, 4 },
	(Instruction) { "DEC", DEC, IM_ABS, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "BNE", BNE, IM_REL, 2 },
	(Instruction) { "CMP", CMP, IM_IZY, 5 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "CMP", CMP, IM_ZPX, 4 },
	(Instruction) { "DEC", DEC, IM_ZPX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "CLD", CLD, IM_IMP, 2 },
	(Instruction) { "CMP", CMP, IM_ABY, 4 },
	(Instruction) { "NOP", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "CMP", CMP, IM_ABX, 4 },
	(Instruction) { "DEC", DEC, IM_ABX, 7 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "CPX", CPX, IM_IMM, 2 },
	(Instruction) { "SBC", SBC, IM_IZX, 6 },
	(Instruction) { "---", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "CPX", CPX, IM_ZP0, 3 },
	(Instruction) { "SBC", SBC, IM_ZP0, 3 },
	(Instruction) { "INC", INC, IM_ZP0, 5 },
	(Instruction) { "---", NUL, IM_IMP, 5 },
	(Instruction) { "INX", INX, IM_IMP, 2 },
	(Instruction) { "SBC", SBC, IM_IMM, 2 },
	(Instruction) { "NOP", NOP, IM_IMP, 2 },
	(Instruction) { "---", SBC, IM_IMP, 2 },
	(Instruction) { "CPX", CPX, IM_ABS, 4 },
	(Instruction) { "SBC", SBC, IM_ABS, 4 },
	(Instruction) { "INC", INC, IM_ABS, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "BEQ", BEQ, IM_REL, 2 },
	(Instruction) { "SBC", SBC, IM_IZY, 5 },
	(Instruction) { "---", NUL, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 8 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "SBC", SBC, IM_ZPX, 4 },
	(Instruction) { "INC", INC, IM_ZPX, 6 },
	(Instruction) { "---", NUL, IM_IMP, 6 },
	(Instruction) { "SED", SED, IM_IMP, 2 },
	(Instruction) { "SBC", SBC, IM_ABY, 4 },
	(Instruction) { "NOP", NOP, IM_IMP, 2 },
	(Instruction) { "---", NUL, IM_IMP, 7 },
	(Instruction) { "---", NOP, IM_IMP, 4 },
	(Instruction) { "SBC", SBC, IM_ABX, 4 },
	(Instruction) { "INC", INC, IM_ABX, 7 },
	(Instruction) { "---", NUL, IM_IMP, 7 }
};

// static functions

static bool get_flag(int flag) {
	return (st & flag) ? 1 : 0;
}

static void set_flag(int flag, bool value) {
	if (value) {
		st |= value;
	} else {
		st &= ~value;
	}
}

// public functions

void r6502_init() {
	a = 0;
	x = 0;
	y = 0;
	sp = 0;
	pc = 0;
	st = 0;

	fetched = 0;
	temp = 0;
	abs_addr = 0;
	rel_addr = 0;
	opcode = 0;
	cycles = 0;
	clock_count = 0;
}

void r6502_reset() {
	abs_addr = 0xFFFC;
	pc = ram_read_u16(abs_addr);

	a = 0;
	x = 0;
	y = 0;
	sp = 0;
	st = R6502_F_U;

	rel_addr = 0;
	abs_addr = 0;
	fetched = 0;

	cycles = 8;
}

void r6502_irq() {
	printfln("%s@%s:%d unimplemented", __func__, __FILE__, __LINE__);
}

void r6502_nmi() {
	printfln("%s@%s:%d unimplemented", __func__, __FILE__, __LINE__);
}

void r6502_clk() {
	if (cycles == 0) {
		opcode = ram_read_u8(pc);
		set_flag(R6502_F_U, true); // Maybe remove ??
		++pc;

		Instruction ins = instructions[opcode];

		printfln("%s:%d: opcode = %d, ins = { name = %s, cycles = %d, operand: %p, mode: %s }", __FILE__, __LINE__, opcode, ins.name, ins.cycles, ins.operand, instruction_mode_to_string(ins.mode));

		cycles = ins.cycles;

		uint8_t additional_cycles_1 = im(ins.mode); // from instruction addressing mode
		uint8_t additional_cycles_2 = ins.operand(); // from instruction itself

		cycles += (additional_cycles_1 & additional_cycles_2);
	}

	++clock_count;
	--cycles;
}

bool r6502_complete() {
	return cycles == 0;
}

