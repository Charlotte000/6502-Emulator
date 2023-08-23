# 6502 Microprocessor Emulator

This project is an emulator for the MOS 6502 microprocessor written in C++.
It aims to faithfully replicate the behavior of the 6502 microprocessor by implementing all legal opcodes and addressing modes, except for the decimal mode which has not been implemented at this time.

![](https://upload.wikimedia.org/wikipedia/commons/thumb/f/fb/BreakNES_MOS_6502.jpg/220px-BreakNES_MOS_6502.jpg)

## Features
- Full support for all legal opcodes of the 6502 microprocessor.
- Implementation of various addressing modes for opcodes.
- Accurate emulation of the 6502 microprocessor behavior.
- Ability to inspect the number of cycles required to execute a program.

## Testing
The emulator has successfully passed the functional tests provided by Klaus2m5.
These tests can be found in the [6502_65C02_functional_tests](https://github.com/Klaus2m5/6502_65C02_functional_tests) repository.
Note that the decimal mode tests are currently not passing, as the decimal mode is not implemented yet.

## Acknowledgments
This project draws inspiration from the excellent work done by the authors of the [olcNES](https://github.com/OneLoneCoder/olcNES/) and [p6502Emulator](https://github.com/davepoo/6502Emulator/) repositories.
Their dedication to accurate emulation has been instrumental in the development of this emulator.
The comprehensive [6502 Instruction Set](https://www.masswerk.at/6502/6502_instruction_set.html) reference for opcode information.

## Contributions
Contributions to this project are welcome.
If you find any issues or want to add enhancements, feel free to open a pull request or create an issue on the GitHub repository.