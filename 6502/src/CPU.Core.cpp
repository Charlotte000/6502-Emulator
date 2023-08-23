#include <iostream>
#include <stdexcept>
#include "Bus.h"
#include "CPU.h"

void CPU::step()
{
	// Get opcode
	Byte opcode = this->read(this->PC);
	this->PC++;

	// Get current command
	this->currentCommand = this->lookup[opcode];
	
	// Execute
	bool additionalCycle1 = (this->*currentCommand.addressingMode)();
	bool additionalCycle2 = (this->*currentCommand.instruction)();

	// Add cycles
	this->Cycles += this->currentCommand.cycles;
	this->Cycles += additionalCycle1 && additionalCycle2;
}

void CPU::reset()
{
	// Reset registers
	this->A = this->X = this->Y = 0;
	this->SP = 0xFF;
	this->SR = (CPU::Status)0;

	// JMP
	this->PC = this->readWord(CPU::RESVector);

	// Reset cycle count
	this->Cycles = 8;
}

/// <summary>
/// Interrupt Request
/// </summary>
void CPU::IRQ()
{
	if (this->getFlag(CPU::Status::I) != 0)
	{
		return;
	}

	// Push PC to the stack
	this->pushWordStack(this->PC);

	// Push SR to the stack
	this->setFlag(CPU::Status::B, false);
	this->setFlag(CPU::Status::U, true);
	this->setFlag(CPU::Status::I, true);
	this->pushStack((Byte)this->SR);
	this->SP--;

	// JMP
	this->PC = this->readWord(CPU::IRQVector);

	this->Cycles = 7;
}

void CPU::NMI()
{
	// Push PC to the stack
	this->pushWordStack(this->PC);

	// Push SR to the stack
	this->setFlag(CPU::Status::B, false);
	this->setFlag(CPU::Status::U, true);
	this->setFlag(CPU::Status::I, true);
	this->pushStack((Byte)this->SR);

	// JMP
	this->PC = this->readWord(CPU::NMIVector);

	this->Cycles = 8;
}

void CPU::setFlag(CPU::Status flag, bool value)
{
	if (value)
	{
		this->SR = (CPU::Status)(this->SR | flag);
	}
	else
	{
		this->SR = (CPU::Status)(this->SR & ~flag);
	}
}

bool CPU::getFlag(CPU::Status flag)
{
	return (this->SR & flag) > 0;
}

Word CPU::getWord(Byte lo, Byte hi)
{
	return (Word)lo | ((Word)hi << 8);
}

Byte CPU::read(Word address)
{
	if (this->bus == nullptr)
	{
		throw std::runtime_error("Bus not connected");
	}

	return this->bus->read(address);
}

void CPU::write(Word address, Byte data)
{
	if (this->bus == nullptr)
	{
		throw std::runtime_error("Bus not connected");
	}

	return this->bus->write(address, data);
}

Word CPU::readWord(Word address)
{
	if (this->bus == nullptr)
	{
		throw std::runtime_error("Bus not connected");
	}

	Byte lo = this->bus->read(address);
	Byte hi = this->bus->read(address + 1);
	return CPU::getWord(lo, hi);
}

void CPU::writeWord(Word address, Word data)
{
	Byte lo = (Byte)data;
	Byte hi = (Byte)(data >> 8);
	this->write(address, lo);
	this->write(address + 1, hi);
}

void CPU::pushStack(Byte data)
{
	this->write(CPU::StackOrigin + this->SP, data);
	this->SP--;
}

Byte CPU::popStack()
{
	this->SP++;
	return this->read(CPU::StackOrigin + this->SP);
}

void CPU::pushWordStack(Word data)
{
	this->write(CPU::StackOrigin + this->SP, (Byte)(data >> 8));
	this->SP--;
	this->write(CPU::StackOrigin + this->SP, (Byte)data);
	this->SP--;
}

Word CPU::popWordStack()
{
	this->SP++;
	Byte lo = this->read(CPU::StackOrigin + this->SP);

	this->SP++;
	Byte hi = this->read(CPU::StackOrigin + this->SP);
	return CPU::getWord(lo, hi);
}
