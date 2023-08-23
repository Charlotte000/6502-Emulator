#include <stdexcept>
#include "CPU.h"
#include "Ram.h"
#include "Bus.h"

void Bus::connectCPU(CPU* cpu)
{
	this->cpu = cpu;
	this->cpu->bus = this;
}

void Bus::connectRAM(RAM* ram)
{
	this->ram = ram;
	this->ram->bus = this;
}

Byte Bus::read(Word address)
{
	if (address >= 0x00 && address <= 0xFFFF)
	{
		if (this->ram == nullptr)
		{
			throw std::runtime_error("RAM not connected");
		}

		return this->ram->read(address);
	}

	throw std::runtime_error("Invalid address");
}

void Bus::write(Word address, Byte data)
{
	if (address >= 0x00 && address <= 0xFFFF)
	{
		if (this->ram == nullptr)
		{
			throw std::runtime_error("RAM not connected");
		}

		return this->ram->write(address, data);
	}

	throw std::runtime_error("Invalid address");
}