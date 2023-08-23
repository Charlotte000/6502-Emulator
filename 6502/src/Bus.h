#pragma once

#include "CPU.h"

class RAM;

class Bus
{
public:
	CPU* cpu;
	RAM* ram;

	void connectCPU(CPU* cpu);

	void connectRAM(RAM* ram);

	Byte read(Word address);

	void write(Word address, Byte data);
private:
};