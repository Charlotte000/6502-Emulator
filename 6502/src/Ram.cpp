#include "CPU.h"
#include "Bus.h"
#include "Ram.h"

Byte RAM::read(Word address)
{
	return this->data[address];
}

void RAM::write(Word address, Byte data)
{
	this->data[address] = data;
}
