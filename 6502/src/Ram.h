#pragma once

#include <array>
#include "CPU.h"

class Bus;

class RAM
{
public:
	std::array<Byte, 1024 * 64> data;
	Bus* bus;

	Byte read(Word address);

	void write(Word address, Byte data);
private:
};