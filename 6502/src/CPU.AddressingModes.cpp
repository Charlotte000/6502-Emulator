#include "CPU.h"

bool CPU::ABS()
{
	this->Address = this->readWord(this->PC);
	this->PC += 2;

	this->M = this->read(this->Address);
	return false;
}

bool CPU::ABX()
{
	Word abs = this->readWord(this->PC);
	this->PC += 2;

	this->Address = abs + this->X;
	this->M = this->read(this->Address);
	return (this->Address & 0xFF00) != (abs & 0xFF00);
}

bool CPU::ABY()
{
	Word abs = this->readWord(this->PC);
	this->PC += 2;

	this->Address = abs + this->Y;
	this->M = this->read(this->Address);
	return (this->Address & 0xFF00) != (abs & 0xFF00);
}

bool CPU::IMM()
{
	this->Address = this->PC;
	this->M = this->read(this->Address);
	this->PC++;
	return false;
}

bool CPU::IMP()
{
	this->Address = this->A;
	this->M = (Byte)this->Address;
	return false;
}

bool CPU::IND()
{
	Word ptr = this->readWord(this->PC);
	this->PC += 2;

	if ((ptr & 0x00FF) == 0x00FF)
	{
		// Simulate 6502 page boundary hardware bug
		this->Address = CPU::getWord(this->read(ptr), this->read(ptr & 0xFF00));
	}
	else
	{
		this->Address = this->readWord(ptr);
	}

	this->M = this->read(this->Address);
	return false;
}

bool CPU::IZX()
{
	Byte ptr = this->read(this->PC) + this->X;
	this->PC++;

	this->Address = this->readWord(ptr);
	this->M = this->read(this->Address);
	return false;
}

bool CPU::IZY()
{
	Byte ptr = this->read(this->PC);
	this->PC++;

	Word ind = this->readWord(ptr);
	this->Address = ind + this->Y;
	this->M = this->read(this->Address);
	return (this->Address & 0xFF00) != (ind & 0xFF00);
}

bool CPU::REL()
{
	Word delta = this->read(this->PC);
	this->PC++;

	if (delta & 0x0080)
	{
		delta |= 0xFF00;
	}

	this->Address = this->PC + delta;
	this->M = (Byte)this->Address;
	return false;
}

bool CPU::ZP0()
{
	this->Address = this->read(this->PC);
	this->PC++;

	this->M = this->read(this->Address);
	return false;
}

bool CPU::ZPX()
{
	this->Address = (Byte)(this->read(this->PC) + this->X);
	this->PC++;

	this->M = this->read(this->Address);
	return false;
}

bool CPU::ZPY()
{
	this->Address = (Byte)(this->read(this->PC) + this->Y);
	this->PC++;

	this->M = this->read(this->Address);
	return false;
}
