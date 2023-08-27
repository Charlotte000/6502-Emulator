#include "pch.h"
#include <fstream>
#include <CPU.h>
#include <RAM.h>
#include <Bus.h>

using namespace std;

// Klaus2m5 6502 emulator test
// https://github.com/Klaus2m5/6502_65C02_functional_tests
TEST(FunctionalTest, AllOpcodesAndAddressingModes)
{
	CPU cpu;
	RAM ram;
	Bus bus;

	bus.connectCPU(&cpu);
	bus.connectRAM(&ram);

	cpu.reset();

	ifstream file("tests/6502_functional_test.bin", ios::binary);
	file.unsetf(ios::skipws);

	std::copy(istream_iterator<Byte>(file), istream_iterator<Byte>(), ram.data.begin() + 0x000a);
	cpu.PC = 0x0400;

	while (cpu.PC != 0x3469 && cpu.Cycles < 100000000)
	{
		cpu.step();
	}

	EXPECT_EQ(cpu.PC, 0x3469);
}

// Bruce Clark 6502 emulator test
// https://github.com/Klaus2m5/6502_65C02_functional_tests
TEST(DecimalTest, DecimalMode)
{
	//FAIL();
	CPU cpu;
	RAM ram;
	Bus bus;

	bus.connectCPU(&cpu);
	bus.connectRAM(&ram);

	ifstream file("tests/6502_decimal_test.bin", ios::binary);
	file.unsetf(ios::skipws);
	std::copy(istream_iterator<Byte>(file), istream_iterator<Byte>(), ram.data.begin() + 0x0200);

	cpu.writeWord(CPU::RESVector, 0x0200);
	cpu.reset();

	while (cpu.PC != 0x024b && cpu.Cycles < 54000000)
	{
		cpu.step();
	}

	EXPECT_EQ(cpu.read(0x000b), 0);
}

// Klaus2m5 6502 interrupt test
// https://github.com/Klaus2m5/6502_65C02_functional_tests
TEST(InterruptTest, IRQandNMItest)
{
	CPU cpu;
	RAM ram;
	Bus bus;

	bus.connectCPU(&cpu);
	bus.connectRAM(&ram);

	ifstream file("tests/6502_interrupt_test.bin", ios::binary);
	file.unsetf(ios::skipws);
	std::copy(istream_iterator<Byte>(file), istream_iterator<Byte>(), ram.data.begin() + 0x000a);

	cpu.writeWord(CPU::RESVector, 0x0400);
	cpu.reset();

	Byte oldFeedback = cpu.read(0xbffc);
	Byte feedback = oldFeedback;
	while (cpu.PC != 0x06f5 && cpu.Cycles < 4000)
	{
		cpu.step();

		oldFeedback = feedback;
		feedback = cpu.read(0xbffc);

		if ((feedback & 0x80) == 0)
		{
			if (feedback & 0x01)
			{
				cpu.IRQ();
			}

			if ((oldFeedback & 0x02) == 0 && feedback & 0x02)
			{
				cpu.NMI();
			}
		}
	}

	EXPECT_EQ(cpu.PC, 0x06f5);
}
