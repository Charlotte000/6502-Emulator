#pragma once

#include <cstdint>
#include <vector>

using Byte = uint8_t;
using Word = uint16_t;

class Bus;

class CPU
{
public:
	static const Word StackOrigin = 0x0100;
	static const Word NMIVector = 0xFFFA;
	static const Word RESVector = 0xFFFC;
	static const Word IRQVector = 0xFFFE;

	struct Command
	{
		bool (CPU::* instruction)(void);
		bool (CPU::* addressingMode)(void);
		uint16_t cycles;
	};

	enum Status : Byte
	{
		C = (1 << 0),
		Z = (1 << 1),
		I = (1 << 2),
		D = (1 << 3),
		B = (1 << 4),
		U = (1 << 5),
		V = (1 << 6),
		N = (1 << 7),
	};

	/// <summary>
	/// A register
	/// </summary>
	Byte A;

	/// <summary>
	/// X register
	/// </summary>
	Byte X;

	/// <summary>
	/// Y register
	/// </summary>
	Byte Y;

	/// <summary>
	/// Stack pointer
	/// </summary>
	Byte SP;

	/// <summary>
	/// Status register
	/// </summary>
	CPU::Status SR;

	/// <summary>
	/// Program counter
	/// </summary>
	Word PC;

	uint64_t Cycles;

	Bus* bus = nullptr;

	CPU();

	/// <summary>
	/// Execute one command
	/// </summary>
	void step();

	/// <summary>
	/// Initialize the microprocessor and start program execution from <see cref="CPU::ResetVector"/> address
	/// </summary>
	void reset();

	/// <summary>
	/// Interrupt Request
	/// </summary>
	void IRQ();

	/// <summary>
	/// Non-Maskable Interrupt
	/// </summary>
	void NMI();

#pragma region RAM manipulation
	static Word getWord(Byte lo, Byte hi);
	Byte read(Word address);
	void write(Word address, Byte data);
	Word readWord(Word address);
	void writeWord(Word address, Word data);
	void pushStack(Byte data);
	Byte popStack();
	void pushWordStack(Word data);
	Word popWordStack();
#pragma endregion
private:
	std::vector<CPU::Command> lookup;
	Byte M;
	Word Address;
	Command currentCommand;

	void setFlag(CPU::Status flag, bool value);

	bool getFlag(CPU::Status flag);

#pragma region Addressing modes
	bool ABS();
	bool ABX();
	bool ABY();
	bool IMM();
	bool IMP();
	bool IND();
	bool IZX();
	bool IZY();
	bool REL();
	bool ZP0();
	bool ZPX();
	bool ZPY();
#pragma endregion

#pragma region Instructions
	bool ADC();
	bool AND();
	bool ASL();
	bool BCC();
	bool BCS();
	bool BEQ();
	bool BIT();
	bool BMI();
	bool BNE();
	bool BPL();
	bool BRK();
	bool BVC();
	bool BVS();
	bool CLC();
	bool CLD();
	bool CLI();
	bool CLV();
	bool CMP();
	bool CPX();
	bool CPY();
	bool DEC();
	bool DEX();
	bool DEY();
	bool EOR();
	bool INC();
	bool INX();
	bool INY();
	bool JMP();
	bool JSR();
	bool LDA();
	bool LDX();
	bool LDY();
	bool LSR();
	bool NOP();
	bool ORA();
	bool PHA();
	bool PHP();
	bool PLA();
	bool PLP();
	bool ROL();
	bool ROR();
	bool RTI();
	bool RTS();
	bool SBC();
	bool SEC();
	bool SED();
	bool SEI();
	bool STA();
	bool STX();
	bool STY();
	bool TAX();
	bool TAY();
	bool TSX();
	bool TXA();
	bool TXS();
	bool TYA();
	bool ILL();
#pragma endregion
};