#include <stdexcept>
#include "CPU.h"

bool CPU::ADC()
{
    if (this->getFlag(CPU::Status::D))
    {
        Word lo = (this->A & 0x0F) + (this->M & 0x0F) + this->getFlag(CPU::Status::C);
        Word hi = (this->A & 0xF0) + (this->M & 0xF0);

        this->setFlag(CPU::Status::Z, ((lo + hi) & 0xFF) == 0);

        if (lo > 0x09)
        {
            hi += 0x10;
            lo += 0x06;
        }

        this->setFlag(CPU::Status::N, hi & 0x0080);
        this->setFlag(CPU::Status::V, ~(this->A ^ this->M) & (this->A ^ hi) & 0x0080);
        if (hi > 0x90)
        {
            hi += 0x60;
        }

        this->setFlag(CPU::Status::C, hi & 0xFF00);

        this->A = (lo & 0x0F) | (hi & 0xF0);
        return false;
    }

    Word sum = this->A + this->M + this->getFlag(CPU::Status::C);

    this->setFlag(CPU::Status::Z, (sum & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, sum & 0x0080);
    this->setFlag(CPU::Status::C, sum & 0xFF00);
    this->setFlag(CPU::Status::V, ~(this->A ^ this->M) & (this->A ^ sum) & 0x0080);

    this->A = (Byte)sum;
    return true;
}

bool CPU::AND()
{
    this->A &= this->M;
    this->setFlag(CPU::Status::Z, (this->A & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->A & 0x0080);
    return true;
}

bool CPU::ASL()
{
    Word temp = this->M << 1;
    this->setFlag(CPU::Status::C, temp & 0xFF00);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);

    if (this->currentCommand.addressingMode == &CPU::IMP)
    {
        this->A = (Byte)temp;
    }
    else
    {
        this->write(this->Address, (Byte)temp);
    }

    return false;
}

bool CPU::BCC()
{
    if (this->getFlag(CPU::Status::C) == 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::BCS()
{
    if (this->getFlag(CPU::Status::C) != 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::BEQ()
{
    if (this->getFlag(CPU::Status::Z) != 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::BIT()
{
    Byte temp = this->A & this->M;

    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->M & 0x0080);
    this->setFlag(CPU::Status::V, this->M & 0x0040);
    return false;
}

bool CPU::BMI()
{
    if (this->getFlag(CPU::Status::N) != 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::BNE()
{
    if (this->getFlag(CPU::Status::Z) == 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::BPL()
{
    if (this->getFlag(CPU::Status::N) == 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::BRK()
{
    // Push PC to the stack
    this->PC++;
    this->pushWordStack(this->PC);

    // Push SP to the stack
    this->pushStack((Byte)(this->SR | CPU::Status::B | CPU::Status::U));

    // Reset
    this->setFlag(CPU::Status::B, true);
    this->setFlag(CPU::Status::I, true);
    this->PC = this->readWord(CPU::IRQVector);
    return false;
}

bool CPU::BVC()
{
    if (this->getFlag(CPU::Status::V) == 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::BVS()
{
    if (this->getFlag(CPU::Status::V) != 0)
    {
        this->Cycles++;
        if ((this->Address & 0xFF00) != (this->PC & 0xFF00))
        {
            this->Cycles++;
        }

        this->PC = this->Address;
    }

    return false;
}

bool CPU::CLC()
{
    this->setFlag(CPU::Status::C, false);
    return false;
}

bool CPU::CLD()
{
    this->setFlag(CPU::Status::D, false);
    return false;
}

bool CPU::CLI()
{
    this->setFlag(CPU::Status::I, false);
    return false;
}

bool CPU::CLV()
{
    this->setFlag(CPU::Status::V, false);
    return false;
}

bool CPU::CMP()
{
    Word temp = (Word)this->A - this->M;
    this->setFlag(CPU::Status::C, this->A >= this->M);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);
    return true;
}

bool CPU::CPX()
{
    Word temp = (Word)this->X - this->M;
    this->setFlag(CPU::Status::C, this->X >= this->M);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);
    return false;
}

bool CPU::CPY()
{
    Word temp = (Word)this->Y - this->M;
    this->setFlag(CPU::Status::C, this->Y >= this->M);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);
    return false;
}

bool CPU::DEC()
{
    Word temp = this->M - 1;
    this->write(this->Address, (Byte)temp);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);
    return false;
}

bool CPU::DEX()
{
    this->X--;
    this->setFlag(CPU::Status::Z, (this->X & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->X & 0x0080);
    return false;
}

bool CPU::DEY()
{
    this->Y--;
    this->setFlag(CPU::Status::Z, (this->Y & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->Y & 0x0080);
    return false;
}

bool CPU::EOR()
{
    this->A ^= this->M;
    this->setFlag(CPU::Status::Z, (this->A & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->A & 0x0080);
    return true;
}

bool CPU::INC()
{
    Word temp = this->M + 1;
    this->write(this->Address, (Byte)temp);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);
    return false;
}

bool CPU::INX()
{
    this->X++;
    this->setFlag(CPU::Status::Z, (this->X & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->X & 0x0080);
    return false;
}

bool CPU::INY()
{
    this->Y++;
    this->setFlag(CPU::Status::Z, (this->Y & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->Y & 0x0080);
    return false;
}

bool CPU::JMP()
{
    this->PC = this->Address;
    return false;
}

bool CPU::JSR()
{
    this->PC--;
    this->pushWordStack(this->PC);
    this->PC = this->Address;
    return false;
}

bool CPU::LDA()
{
    this->A = this->M;
    this->setFlag(CPU::Status::Z, (this->A & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->A & 0x0080);
    return true;
}

bool CPU::LDX()
{
    this->X = this->M;
    this->setFlag(CPU::Status::Z, (this->X & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->X & 0x0080);
    return true;
}

bool CPU::LDY()
{
    this->Y = (Byte)this->M;
    this->setFlag(CPU::Status::Z, (this->Y & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->Y & 0x0080);
    return true;
}

bool CPU::LSR()
{
    this->setFlag(CPU::Status::C, this->M & 0x0001);
    Word temp = this->M >> 1;
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);
    if (this->currentCommand.addressingMode == &CPU::IMP)
    {
        this->A = (Byte)temp;
    }
    else
    {
        this->write(this->Address, (Byte)temp);
    }

    return false;

}

bool CPU::NOP()
{
    return false;
}

bool CPU::ORA()
{
    this->A |= this->M;
    this->setFlag(CPU::Status::Z, (this->A & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->A & 0x0080);
    return true;
}

bool CPU::PHA()
{
    this->pushStack(this->A);
    return false;
}

bool CPU::PHP()
{
    this->pushStack((Byte)(this->SR | CPU::Status::B | CPU::Status::U));
    return false;
}

bool CPU::PLA()
{
    this->A = this->popStack();
    this->setFlag(CPU::Status::Z, (this->A & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->A & 0x0080);
    return false;
}

bool CPU::PLP()
{
    this->SR = (CPU::Status)this->popStack();
    this->setFlag(CPU::Status::B, false);
    this->setFlag(CPU::Status::U, true);
    return false;
}

bool CPU::ROL()
{
    Word temp = (this->M << 1) | (Word)this->getFlag(CPU::Status::C);
    this->setFlag(CPU::Status::C, temp & 0xFF00);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);

    if (this->currentCommand.addressingMode == &CPU::IMP)
    {
        this->A = (Byte)temp;
    }
    else
    {
        this->write(this->Address, (Byte)temp);
    }

    return false;
}

bool CPU::ROR()
{
    Word temp = (this->getFlag(CPU::Status::C) << 7) | (this->M >> 1);
    this->setFlag(CPU::Status::C, this->M & 0x0001);
    this->setFlag(CPU::Status::Z, (temp & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, temp & 0x0080);

    if (this->currentCommand.addressingMode == &CPU::IMP)
    {
        this->A = (Byte)temp;
    }
    else
    {
        this->write(this->Address, (Byte)temp);
    }

    return false;
}

bool CPU::RTI()
{
    // Pop SR
    this->SR = (CPU::Status)this->popStack();
    this->setFlag(CPU::Status::B, false);
    this->setFlag(CPU::Status::U, false);

    // Pop PC
    this->PC = this->popWordStack();
    return false;
}

bool CPU::RTS()
{
    this->PC = this->popWordStack();
    this->PC++;
    return false;
}

bool CPU::SBC()
{
    Word sub = this->A - this->M - !this->getFlag(CPU::Status::C);

    if (this->getFlag(CPU::Status::D))
    {
        Word lo = (this->A & 0x0F) - (this->M & 0x0F) - !this->getFlag(CPU::Status::C);
        Word hi = (this->A & 0xF0) - (this->M & 0xF0);

        if (lo & 0x10)
        {
            lo -= 6;
            hi--;
        }

        this->setFlag(CPU::Status::V, (sub ^ this->A) & (this->M ^ this->A) & 0x0080);
        this->setFlag(CPU::Status::C, sub < 0x0100);
        this->setFlag(CPU::Status::Z, (sub & 0x00FF) == 0);
        this->setFlag(CPU::Status::N, sub & 0x0080);

        if (hi & 0x0100)
        {
            hi -= 0x60;
        }

        this->A = (lo & 0x0F) | (hi & 0xF0);
        return true;
    }

    this->setFlag(CPU::Status::Z, (sub & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, sub & 0x0080);
    this->setFlag(CPU::Status::V, (sub ^ this->A) & (this->M ^ this->A) & 0x0080);
    this->setFlag(CPU::Status::C, sub < 0x0100);

    this->A = (Byte)sub;
    return true;
}

bool CPU::SEC()
{
    this->setFlag(CPU::Status::C, true);
    return false;
}

bool CPU::SED()
{
    this->setFlag(CPU::Status::D, true);
    return false;
}

bool CPU::SEI()
{
    this->setFlag(CPU::Status::I, true);
    return false;
}

bool CPU::STA()
{
    this->write(this->Address, this->A);
    return false;
}

bool CPU::STX()
{
    this->write(this->Address, this->X);
    return false;
}

bool CPU::STY()
{
    this->write(this->Address, this->Y);
    return false;
}

bool CPU::TAX()
{
    this->X = this->A;
    this->setFlag(CPU::Status::Z, (this->X & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->X & 0x0080);
    return false;
}

bool CPU::TAY()
{
    this->Y = this->A;
    this->setFlag(CPU::Status::Z, (this->Y & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->Y & 0x0080);
    return false;
}

bool CPU::TSX()
{
    this->X = this->SP;
    this->setFlag(CPU::Status::Z, (this->X & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->X & 0x0080);
    return false;
}

bool CPU::TXA()
{
    this->A = this->X;
    this->setFlag(CPU::Status::Z, (this->A & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->A & 0x0080);
    return false;
}

bool CPU::TXS()
{
    this->SP = this->X;
    return false;
}

bool CPU::TYA()
{
    this->A = this->Y;
    this->setFlag(CPU::Status::Z, (this->A & 0x00FF) == 0);
    this->setFlag(CPU::Status::N, this->A & 0x0080);
    return false;
}

bool CPU::ILL()
{
    throw std::runtime_error("Illegal opcode");
}