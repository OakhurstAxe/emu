#include "headers/oam6502.h"

#include <QString>

int M6502::executeTicks(int count)
{
    int cycleCount = 0;
    while (cycleCount < count)
    {
        if (nmiSet == 1)
        {
            programCounter--;
            pushStack((programCounter & 0xff00) >> 8);
            pushStack(programCounter & 0x00ff);
            nmiSet = 0;
            cycleCount += OpPHP();
            unsigned char loadl = memory.cpuRead(0xfffa);
            unsigned char loadh = memory.cpuRead(0xfffb);
            programCounter = (loadh << 8) + loadl;
        }
        else
        {
            unsigned char instruction = memory.cpuRead(programCounter);
            
            switch(instruction) {
                case 0x01: { cycleCount += OpORAIndirectX(); break; }
                case 0x05: { cycleCount += OpORAZero(); break; }
                case 0x06: { cycleCount += OpASLZero(); break; }
                case 0x08: { cycleCount += OpPHP(); break; }
                case 0x09: { cycleCount += OpORAImmediate(); break; }
                case 0x0a: { cycleCount += OpASLAccumlator(); break; }
                case 0x0e: { cycleCount += OpASLAbsolute(); break; }
                case 0x0d: { cycleCount += OpORAAbsolute(); break; }
                case 0x10: { cycleCount += OpBPL(); break; }
                case 0x11: { cycleCount += OpORAIndirectY(); break; }
                case 0x15: { cycleCount += OpORAZeroX(); break; }
                case 0x16: { cycleCount += OpASLZeroX(); break; }
                case 0x18: { cycleCount += OpCLC(); break; }
                case 0x19: { cycleCount += OpORAAbsoluteY(); break; }
                case 0x1d: { cycleCount += OpORAAbsoluteX(); break; }
                case 0x1e: { cycleCount += OpASLAbsoluteX(); break; }
                case 0x20: { cycleCount += OpJSR(); break; }
                case 0x21: { cycleCount += OpANDIndirectX(); break; }
                case 0x24: { cycleCount += OpBITZero(); break; }
                case 0x25: { cycleCount += OpANDZero(); break; }
                case 0x26: { cycleCount += OpROLZero(); break; }                
                case 0x28: { cycleCount += OpPLP(); break; }
                case 0x29: { cycleCount += OpANDImmediate(); break; }
                case 0x2a: { cycleCount += OpROLAccululator(); break; }                
                case 0x2c: { cycleCount += OpBITAbsolute(); break; }
                case 0x2d: { cycleCount += OpANDAbsolute(); break; }
                case 0x2e: { cycleCount += OpROLAbsolute(); break; }                
                case 0x30: { cycleCount += OpBMI(); break; }
                case 0x31: { cycleCount += OpANDIndirectY(); break; }
                case 0x35: { cycleCount += OpANDZeroX(); break; }
                case 0x36: { cycleCount += OpROLZeroX(); break; }                
                case 0x38: { cycleCount += OpSEC(); break; }            
                case 0x39: { cycleCount += OpANDAbsoluteY(); break; }
                case 0x3d: { cycleCount += OpANDAbsoluteX(); break; }           
                case 0x3e: { cycleCount += OpROLAbsoluteX(); break; }   
                case 0x40: { cycleCount += OpRTI(); break; }
                case 0x41: { cycleCount += OpEORIndirectX(); break; }
                case 0x45: { cycleCount += OpEORZero(); break; }
                case 0x46: { cycleCount += OpLSRZero(); break; }                
                case 0x48: { cycleCount += OpPHA(); break; }
                case 0x49: { cycleCount += OpEORImmediate(); break; }
                case 0x4a: { cycleCount += OpLSRAccululator(); break; }                
                case 0x4c: { cycleCount += OpJMPAbsolute(); break; }
                case 0x4d: { cycleCount += OpEORAbsolute(); break; }
                case 0x4e: { cycleCount += OpLSRAbsolute(); break; }                
                case 0x50: { cycleCount += OpBVC(); break; }
                case 0x51: { cycleCount += OpEORIndirectY(); break; }
                case 0x55: { cycleCount += OpEORZeroX(); break; }
                case 0x56: { cycleCount += OpLSRZeroX(); break; }                
                case 0x58: { cycleCount += OpCLI(); break; }
                case 0x59: { cycleCount += OpEORAbsoluteY(); break; }
                case 0x5d: { cycleCount += OpEORAbsoluteX(); break; }
                case 0x5e: { cycleCount += OpLSRAbsoluteX(); break; }                
                case 0x60: { cycleCount += OpRTS(); break; }
                case 0x61: { cycleCount += OpADCIndirectX(); break; }
                case 0x65: { cycleCount += OpADCZero(); break; }
                case 0x66: { cycleCount += OpRORZero(); break; }
                case 0x68: { cycleCount += OpPLA(); break; }
                case 0x69: { cycleCount += OpADCImmediate(); break; }
                case 0x6a: { cycleCount += OpRORAccululator(); break; }
                case 0x6c: { cycleCount += OpJMPIndirect(); break; }
                case 0x6d: { cycleCount += OpADCAbsolute(); break; }
                case 0x6e: { cycleCount += OpRORAbsolute(); break; }
                case 0x70: { cycleCount += OpBVS(); break; }
                case 0x71: { cycleCount += OpADCIndirectY(); break; }
                case 0x75: { cycleCount += OpADCZeroX(); break; }
                case 0x76: { cycleCount += OpRORZeroX(); break; }
                case 0x78: { cycleCount += OpSEI(); break; }
                case 0x79: { cycleCount += OpADCAbsoluteY(); break; }
                case 0x7d: { cycleCount += OpADCAbsoluteX(); break; }
                case 0x7e: { cycleCount += OpRORAbsoluteX(); break; }
                case 0x81: { cycleCount += OpSTAIndirectX(); break; }
                case 0x84: { cycleCount += OpSTYZero(); break; }
                case 0x85: { cycleCount += OpSTAZero(); break; }
                case 0x86: { cycleCount += OpSTXZero(); break; }
                case 0x88: { cycleCount += OpDEY(); break; }
                case 0x8a: { cycleCount += OpTXA(); break; }
                case 0x8c: { cycleCount += OpSTYAbsolute(); break; }
                case 0x8d: { cycleCount += OpSTAAbsolute(); break; }
                case 0x8e: { cycleCount += OpSTXAbsolute(); break; }
                case 0x90: { cycleCount += OpBCC(); break; }
                case 0x91: { cycleCount += OpSTAIndirectY(); break; }
                case 0x94: { cycleCount += OpSTYZeroX(); break; }
                case 0x95: { cycleCount += OpSTAZeroX(); break; }
                case 0x96: { cycleCount += OpSTXZeroY(); break; }
                case 0x98: { cycleCount += OpTYA(); break; }
                case 0x99: { cycleCount += OpSTAAbsoluteY(); break; }
                case 0x9a: { cycleCount += OpTXS(); break; }
                case 0x9d: { cycleCount += OpSTAAbsoluteX(); break; }
                case 0xa0: { cycleCount += OpLDYImmediate(); break; }
                case 0xa1: { cycleCount += OpLDAIndirectX(); break; }
                case 0xa2: { cycleCount += OpLDXImmediate(); break; }
                case 0xa4: { cycleCount += OpLDYZero(); break; }
                case 0xa5: { cycleCount += OpLDAZero(); break; }
                case 0xa6: { cycleCount += OpLDXZero(); break; }
                case 0xa8: { cycleCount += OpTAY(); break; }
                case 0xa9: { cycleCount += OpLDAImmediate(); break; }
                case 0xaa: { cycleCount += OpTAX(); break; }
                case 0xac: { cycleCount += OpLDYAbsolute(); break; }
                case 0xad: { cycleCount += OpLDAAbsolute(); break; }
                case 0xae: { cycleCount += OpLDXAbsolute(); break; }
                case 0xb0: { cycleCount += OpBCS(); break; }
                case 0xb1: { cycleCount += OpLDAIndirectY(); break; }
                case 0xb4: { cycleCount += OpLDYZeroX(); break; }
                case 0xb5: { cycleCount += OpLDAZeroX(); break; }
                case 0xb6: { cycleCount += OpLDXZeroY(); break; }
                case 0xb8: { cycleCount += OpCLV(); break; }            
                case 0xb9: { cycleCount += OpLDAAbsoluteY(); break; }
                case 0xba: { cycleCount += OpTSX(); break; }
                case 0xbc: { cycleCount += OpLDYAbsoluteX(); break; }
                case 0xbd: { cycleCount += OpLDAAbsoluteX(); break; }
                case 0xbe: { cycleCount += OpLDXAbsoluteY(); break; }
                case 0xc0: { cycleCount += OpCPYImmediate(); break; }
                case 0xc1: { cycleCount += OpCMPIndirectX(); break; }
                case 0xc4: { cycleCount += OpCPYZero(); break; }
                case 0xc5: { cycleCount += OpCMPZero(); break; }
                case 0xc6: { cycleCount += OpDECZero(); break; }
                case 0xc8: { cycleCount += OpINY(); break; }
                case 0xc9: { cycleCount += OpCMPImmediate(); break; }
                case 0xca: { cycleCount += OpDEX(); break; }
                case 0xcc: { cycleCount += OpCPYAbsolute(); break; }
                case 0xcd: { cycleCount += OpCMPAbsolute(); break; }
                case 0xce: { cycleCount += OpDECAbsolute(); break; }
                case 0xd0: { cycleCount += OpBNE(); break; }
                case 0xd1: { cycleCount += OpCMPIndirectY(); break; }
                case 0xd5: { cycleCount += OpCMPZeroX(); break; }
                case 0xd6: { cycleCount += OpDECZeroX(); break; }
                case 0xd8: { cycleCount += OpCLD(); break; }
                case 0xd9: { cycleCount += OpCMPAbsoluteY(); break; }
                case 0xdd: { cycleCount += OpCMPAbsoluteX(); break; }
                case 0xde: { cycleCount += OpDECAbsoluteX(); break; }
                case 0xe0: { cycleCount += OpCPXImmediate(); break; }
                case 0xe1: { cycleCount += OpSBCIndirectX(); break; }
                case 0xe3: { cycleCount += OpISCIndirectX(); break; }
                case 0xe4: { cycleCount += OpCPXZero(); break; }
                case 0xe5: { cycleCount += OpSBCZero(); break; }
                case 0xe6: { cycleCount += OpINCZero(); break; }
                case 0xe7: { cycleCount += OpISCZero(); break; }
                case 0xe8: { cycleCount += OpINX(); break; }
                case 0xe9: { cycleCount += OpSBCImmediate(); break; }
                case 0xec: { cycleCount += OpCPXAbsolute(); break; }
                case 0xed: { cycleCount += OpSBCAbsolute(); break; }
                case 0xee: { cycleCount += OpINCAbsolute(); break; }
                case 0xef: { cycleCount += OpISCAbsolute(); break; }
                case 0xf0: { cycleCount += OpBEQ(); break; }
                case 0xf1: { cycleCount += OpSBCIndirectY(); break; }
                case 0xf2: { cycleCount += OpNOP(); break; }
                case 0xf3: { cycleCount += OpISCIndirectY(); break; }
                case 0xf5: { cycleCount += OpSBCZeroX(); break; }
                case 0xf6: { cycleCount += OpINCZeroX(); break; }
                case 0xf7: { cycleCount += OpISCZeroX(); break; }
                case 0xf8: { cycleCount += OpSED(); break; }
                case 0xf9: { cycleCount += OpSBCAbsoluteY(); break; }
                case 0xfa: { cycleCount += OpNOP(); break; }
                case 0xfb: { cycleCount += OpISCAbsoluteY(); break; }
                case 0xfd: { cycleCount += OpSBCAbsoluteX(); break; }
                case 0xfe: { cycleCount += OpINCAbsoluteX(); break; }
                case 0xff: { cycleCount += OpISCAbsoluteX(); break; }
                default:
                    throw QString("Invalid instruction %1 previous %2")
                    .arg(instruction).arg(prevInstruction).toLocal8Bit().data();
            }
            prevInstruction = instruction;
            programCounter++;
        }
    }
    return count;
}

void M6502::pushStack(unsigned char byte)
{
    if (stackPointer <= 255)
    {
        throw "Stack overflow";
    }
    memory.cpuWrite(stackPointer, byte);
    stackPointer--;
}

unsigned char M6502::popStack(void)
{
    if (stackPointer >= 512)
    {
        throw "Stack underflow";
    }
    stackPointer++;
    return memory.cpuRead(stackPointer);
}

void M6502::reset()
{
    stackPointer = 0x01ff;
    unsigned char pcl = memory.cpuRead(0xfffc);
    unsigned char pch = memory.cpuRead(0xfffd);
    programCounter = (pch << 8) + pcl;
    accumulator = 0;
    registerX = 0;
    registerY = 0;
    status.interruptDisable = 1;
}

unsigned short M6502::ZeroAddress()
{
    programCounter++;
    return memory.cpuRead(programCounter) % 0xFF;
}

unsigned short M6502::ZeroXAddress()
{
    programCounter++;
    return (memory.cpuRead(programCounter) + registerX) % 0xFF;
}
unsigned short M6502::ZeroYAddress()
{
    programCounter++;
    return (memory.cpuRead(programCounter) + registerY) % 0xFF;
}
unsigned short M6502::AbsoluteAddress()
{
    programCounter++;
    unsigned char loadl = memory.cpuRead(programCounter);
    programCounter++;
    unsigned char loadh = memory.cpuRead(programCounter);
    return (loadh << 8) + loadl;
}
unsigned short M6502::AbsoluteXAddress()
{
    programCounter++;
    unsigned char loadl = memory.cpuRead(programCounter);
    programCounter++;
    unsigned char loadh = memory.cpuRead(programCounter);
    return (loadh << 8) + loadl + registerX;
}
unsigned short M6502::AbsoluteYAddress()
{
    programCounter++;
    unsigned char loadl = memory.cpuRead(programCounter);
    programCounter++;
    unsigned char loadh = memory.cpuRead(programCounter);
    return (loadh << 8) + loadl + registerY;
}
unsigned short M6502::IndirectAddress()
{
    programCounter++;
    unsigned char loadl = memory.cpuRead(programCounter);
    programCounter++;
    unsigned char loadh = memory.cpuRead(programCounter);
    unsigned char load = (loadh << 8) + loadl;
    loadl = memory.cpuRead(load);
    load++;
    loadh = memory.cpuRead(load);
    return (loadh << 8) + loadl;
}
unsigned short M6502::IndirectXAddress()
{
    programCounter++;
    unsigned char indirect = (memory.cpuRead(programCounter) + registerX) % 0xff;
    unsigned char loadl = memory.cpuRead(indirect);
    indirect++;
    unsigned char loadh = memory.cpuRead(indirect);
    return (loadh << 8) + loadl;
}
unsigned short M6502::IndirectYAddress()
{
    programCounter++;
    unsigned char indirect = memory.cpuRead(programCounter) % 0xff;
    unsigned char loadl = memory.cpuRead(indirect);
    indirect++;
    unsigned char loadh = memory.cpuRead(indirect);
    unsigned short address = (loadh << 8) + loadl;
    address += registerY;
    return address;
}
unsigned char M6502::readRelative()
{
    programCounter++;
    return memory.cpuRead(programCounter);
}
unsigned char M6502::readImmediate()
{
    programCounter++;
    return memory.cpuRead(programCounter);
}
unsigned char M6502::readZero()
{
    return memory.cpuRead(ZeroAddress());
}
unsigned char M6502::readZeroX()
{
    return memory.cpuRead(ZeroXAddress());
}
unsigned char M6502::readZeroY()
{
    return memory.cpuRead(ZeroYAddress());
}
unsigned char M6502::readAbsolute()
{
    return memory.cpuRead(AbsoluteAddress());
}
unsigned char M6502::readAbsoluteX()
{
    return memory.cpuRead(AbsoluteXAddress());
}
unsigned char M6502::readAbsoluteY()
{
    return memory.cpuRead(AbsoluteYAddress());
}
unsigned char M6502::readIndirectX()
{
    return memory.cpuRead(IndirectXAddress());
}
unsigned char M6502::readIndirectY()
{
    return memory.cpuRead(IndirectYAddress());
}
void M6502::writeZero(unsigned char byte)
{
    memory.cpuWrite(ZeroAddress(), byte);
}
void M6502::writeZeroX(unsigned char byte)
{
    memory.cpuWrite(ZeroXAddress(), byte);
}
void M6502::writeZeroY(unsigned char byte)
{
    memory.cpuWrite(ZeroYAddress(), byte);
}
void M6502::writeAbsolute(unsigned char byte)
{
    memory.cpuWrite(AbsoluteAddress(), byte);
}
void M6502::writeAbsoluteX(unsigned char byte)
{
    memory.cpuWrite(AbsoluteXAddress(), byte);
}
void M6502::writeAbsoluteY(unsigned char byte)
{
    memory.cpuWrite(AbsoluteYAddress(), byte);
}
void M6502::writeIndirectX(unsigned char byte)
{
    memory.cpuWrite(IndirectXAddress(), byte);
}
void M6502::writeIndirectY(unsigned char byte)
{
    memory.cpuWrite(IndirectYAddress(), byte);
}

// Load Store operations
int M6502::OpLDAImmediate() 
{
    accumulator = readImmediate();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;
}
int M6502::OpLDAZero()
{
    accumulator = readZero();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 3;
}
int M6502::OpLDAZeroX()
{
    accumulator = readZeroX();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpLDAAbsolute() 
{
    accumulator = readAbsolute();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpLDAAbsoluteX()
{
    accumulator = readAbsoluteX();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpLDAAbsoluteY()
{
    accumulator = readAbsoluteY();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpLDAIndirectX()
{
    accumulator = readIndirectX();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 6;
}
int M6502::OpLDAIndirectY()
{
    accumulator = readIndirectY();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 5;
}

int M6502::OpLDXImmediate() 
{
    registerX = readImmediate();
    status.negativeFlag = (registerX & 0x80) > 0;
    status.zeroFlag = (registerX == 0);
    return 2;    
}
int M6502::OpLDXZero()
{
    registerX = readZero();
    status.negativeFlag = (registerX & 0x80) > 0;
    status.zeroFlag = (registerX == 0);
    return 3;
}
int M6502::OpLDXZeroY()
{
    registerX = readZeroY();
    status.negativeFlag = (registerX & 0x80) > 0;
    status.zeroFlag = (registerX == 0);
    return 3;
}
int M6502::OpLDXAbsolute()
{
    registerX = readAbsolute();
    status.negativeFlag = (registerX & 0x80) > 0;
    status.zeroFlag = (registerX == 0);
    return 4;
}
int M6502::OpLDXAbsoluteY()
{
    registerX = readAbsoluteY();
    status.negativeFlag = (registerX & 0x80) > 0;
    status.zeroFlag = (registerX == 0);
    return 4;
}

int M6502::OpLDYImmediate()
{
    registerY = readImmediate();
    status.negativeFlag = (registerY & 0x80) > 0;
    status.zeroFlag = (registerY == 0);
    return 2;
}
int M6502::OpLDYZero()
{
    registerY = readZero();
    status.negativeFlag = (registerY & 0x80) > 0;
    status.zeroFlag = (registerY == 0);
    return 3;
}
int M6502::OpLDYZeroX()
{
    registerY = readZeroX();
    status.negativeFlag = (registerY & 0x80) > 0;
    status.zeroFlag = (registerY == 0);
    return 4;
}
int M6502::OpLDYAbsolute()
{
    registerY = readAbsolute();
    status.negativeFlag = (registerY & 0x80) > 0;
    status.zeroFlag = (registerY == 0);
    return 4;
}
int M6502::OpLDYAbsoluteX()
{
    registerY = readAbsoluteX();
    status.negativeFlag = (registerY & 0x80) > 0;
    status.zeroFlag = (registerY == 0);
    return 4;
}

int M6502::OpSTAZero()
{
    writeZero(accumulator);
    return 3;
}
int M6502::OpSTAZeroX()
{
    writeZeroX(accumulator);
    return 4;
}
int M6502::OpSTAAbsolute()
{
    writeAbsolute(accumulator);
    return 4;
}
int M6502::OpSTAAbsoluteX()
{
    writeAbsoluteX(accumulator);
    return 5;
}
int M6502::OpSTAAbsoluteY()
{
    writeAbsoluteY(accumulator);
    return 5;
}
int M6502::OpSTAIndirectX()
{
    writeIndirectX(accumulator);
    return 6;
}
int M6502::OpSTAIndirectY()
{
    writeIndirectY(accumulator);
    return 6;
}
    
int M6502::OpSTXZero()
{
    writeZero(registerX);
    return 3;
}
int M6502::OpSTXZeroY()
{
    writeZeroY(registerX);
    return 4;
}
int M6502::OpSTXAbsolute()
{
    writeAbsolute(registerX);
    return 4;
}

int M6502::OpSTYZero()
{
    writeZero(registerY);
    return 3;
}
int M6502::OpSTYZeroX()
{
    writeZeroX(registerY);
    return 4;
}
int M6502::OpSTYAbsolute()
{
    writeAbsolute(registerY);
    return 4;
}
    
// Register transfers
int M6502::OpTAX() 
{
    registerX = accumulator;
    status.negativeFlag = (registerX & 0x80) > 0;
    status.zeroFlag = (registerX == 0);
    return 2;
}
int M6502::OpTAY() 
{
    registerY = accumulator;
    status.negativeFlag = (registerY & 0x80) > 0;
    status.zeroFlag = (registerY == 0);
    return 2;
}
int M6502::OpTXA() 
{
    accumulator = registerX;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;
}
int M6502::OpTYA()
{
    accumulator = registerY;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;
}

// Stack operaions
int M6502::OpTSX() 
{
    registerX = popStack();
    status.negativeFlag = (registerX & 0x80) > 0;
    status.zeroFlag = (registerX == 0);
    return 2;
}
int M6502::OpTXS() 
{
    pushStack(registerX);
    return 2;
}
int M6502::OpPHA() 
{
    pushStack(accumulator);
    return 3;
}
int M6502::OpPHP()
{
    pushStack(status.reg);
    status.reg = 0;
    return 3;
}
int M6502::OpPLA()
{
    accumulator = popStack();
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpPLP()
{
    status.reg = popStack();
    return 4;
}    

// Logical operations
int M6502::OpANDImmediate()
{
    unsigned char byte = readImmediate();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;
}
int M6502::OpANDZero()
{
    unsigned char byte = readZero();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 3;
}
int M6502::OpANDZeroX()
{
    unsigned char byte = readZeroX();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpANDAbsolute()
{
    unsigned char byte = readAbsolute();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpANDAbsoluteX()
{
    unsigned char byte = readAbsoluteX();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpANDAbsoluteY()
{
    unsigned char byte = readAbsoluteY();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpANDIndirectX()
{
    unsigned char byte = readIndirectX();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 6;
}
int M6502::OpANDIndirectY()
{
    unsigned char byte = readIndirectY();
    accumulator &= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 5;
}

int M6502::OpEORImmediate()
{
    unsigned char byte = readImmediate();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;
}
int M6502::OpEORZero()
{
    unsigned char byte = readZero();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 3;
}
int M6502::OpEORZeroX()
{
    unsigned char byte = readZeroX();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpEORAbsolute()
{
    unsigned char byte = readAbsolute();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpEORAbsoluteX()
{
    unsigned char byte = readAbsoluteX();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpEORAbsoluteY()
{
    unsigned char byte = readAbsoluteY();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;
}
int M6502::OpEORIndirectX()
{
    unsigned char byte = readIndirectX();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 6;
}
int M6502::OpEORIndirectY()
{
    unsigned char byte = readIndirectY();
    accumulator ^= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 5;
}

int M6502::OpORAImmediate()
{
    unsigned char byte = readImmediate();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;    
}
int M6502::OpORAZero()
{
    unsigned char byte = readZero();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 3;    
}
int M6502::OpORAZeroX()
{
    unsigned char byte = readZeroX();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;    
}
int M6502::OpORAAbsolute()
{
    unsigned char byte = readAbsolute();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;    
}
int M6502::OpORAAbsoluteX()
{
    unsigned char byte = readAbsoluteX();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;    
}
int M6502::OpORAAbsoluteY()
{
    unsigned char byte = readAbsoluteY();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 4;    
}
int M6502::OpORAIndirectX()
{
    unsigned char byte = readIndirectX();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 6;
}
int M6502::OpORAIndirectY()
{
    unsigned char byte = readIndirectY();
    accumulator |= byte;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 5;
}
    
int M6502::OpBITZero()
{
    unsigned char byte = readZero();
    status.zeroFlag = (((byte & accumulator) & 0xff) > 0);
    status.negativeFlag = (byte & 0x80) > 0;
    status.overflowFlag = (byte & 0x40);
    return 3;
}
int M6502::OpBITAbsolute()
{
    unsigned char byte = readAbsolute();
    status.zeroFlag = (((byte & accumulator) & 0xff) > 0);
    status.negativeFlag = (byte & 0x80) > 0;
    status.overflowFlag = (byte & 0x40);
    return 4;
}

    
// Arithmetic operations
int M6502::OpADCImmediate()
{
    unsigned short byte = readImmediate();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 3;
}    
int M6502::OpADCZero()
{
    unsigned short byte = readZero();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 3;
}
int M6502::OpADCZeroX()
{
    unsigned short byte = readZeroX();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpADCAbsolute()
{
    unsigned short byte = readAbsolute();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpADCAbsoluteX()
{
    unsigned short byte = readAbsoluteX();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpADCAbsoluteY()
{
    unsigned short byte = readAbsoluteY();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpADCIndirectX()
{
    unsigned short byte = readIndirectX();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 6;
}
int M6502::OpADCIndirectY()
{
    unsigned short byte = readIndirectY();
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 5;
}
    
int M6502::OpSBCImmediate()
{
    unsigned short byte = readImmediate() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 2;
}
int M6502::OpSBCZero()
{
    unsigned short byte = readZero() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 3;
}
int M6502::OpSBCZeroX()
{
    unsigned short byte = readZeroX() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpSBCAbsolute()
{
    unsigned short byte = readAbsolute() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpSBCAbsoluteX()
{
    unsigned short byte = readAbsoluteX() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpSBCAbsoluteY()
{
    unsigned short byte = readAbsoluteY() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 4;
}
int M6502::OpSBCIndirectX()
{
    unsigned short byte = readIndirectX() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 6;
}
int M6502::OpSBCIndirectY()
{
    unsigned short byte = readIndirectY() ^ 0x00ff;
    unsigned short value = accumulator + byte + status.carryFlag;
    status.negativeFlag = (value & 0x80) > 0;
    status.overflowFlag = ~((accumulator^byte)&(accumulator^value) & 0x80);
    status.carryFlag = (value > 255);
    status.zeroFlag = ((value & 0x00ff) == 0);
    accumulator = (value & 0xff);
    return 5;
}
    
int M6502::OpCMPImmediate()
{
    unsigned char byte = readImmediate();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 2;
}
int M6502::OpCMPZero()
{
    unsigned char byte = readZero();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 3;
}
int M6502::OpCMPZeroX()
{
    unsigned char byte = readZeroX();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 4;
}
int M6502::OpCMPAbsolute()
{
    unsigned char byte = readAbsolute();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 4;
}
int M6502::OpCMPAbsoluteX()
{
    unsigned char byte = readAbsoluteX();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 4;
}
int M6502::OpCMPAbsoluteY()
{
    unsigned char byte = readAbsoluteY();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 4;
}
int M6502::OpCMPIndirectX()
{
    unsigned char byte = readIndirectX();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 6;
}
int M6502::OpCMPIndirectY()
{
    unsigned char byte = readIndirectY();
    status.carryFlag = (accumulator >= byte);
    status.zeroFlag = (accumulator == byte);
    status.negativeFlag = ((accumulator - byte) & 0x80) > 0;
    return 5;
}
    
int M6502::OpCPXImmediate()
{
    unsigned char byte = readImmediate();
    status.carryFlag = (registerX >= byte);
    status.zeroFlag = (registerX == byte);
    status.negativeFlag = ((registerX - byte) & 0x80) > 0;
    return 2;
}
int M6502::OpCPXZero()
{
    unsigned char byte = readZero();
    status.carryFlag = (registerX >= byte);
    status.zeroFlag = (registerX == byte);
    status.negativeFlag = ((registerX - byte) & 0x80) > 0;
    return 3;
}
int M6502::OpCPXAbsolute()
{
    unsigned char byte = readAbsolute();
    status.carryFlag = (registerX >= byte);
    status.zeroFlag = (registerX == byte);
    status.negativeFlag = ((registerX - byte) & 0x80) > 0;
    return 4;
}
    
int M6502::OpCPYImmediate()
{
    unsigned char byte = readImmediate();
    status.carryFlag = (registerY >= byte);
    status.zeroFlag = (registerY == byte);
    status.negativeFlag = ((registerY - byte) & 0x80) > 0;
    return 2;
}
int M6502::OpCPYZero()
{
    unsigned char byte = readZero();
    status.carryFlag = (registerY >= byte);
    status.zeroFlag = (registerY == byte);
    status.negativeFlag = ((registerY - byte) & 0x80) > 0;
    return 3;
}
int M6502::OpCPYAbsolute()
{
    unsigned char byte = readAbsolute();
    status.carryFlag = (registerY >= byte);
    status.zeroFlag = (registerY == byte);
    status.negativeFlag = ((registerY - byte) & 0x80) > 0;
    return 4;
}

// Increment and decrement operations
int M6502::OpINCZero()
{
    unsigned short address = ZeroAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    return 5;
}
int M6502::OpINCZeroX()
{
    unsigned short address = ZeroXAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    return 6;
}
int M6502::OpINCAbsolute()
{
    unsigned short address = AbsoluteAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    return 6;
}
int M6502::OpINCAbsoluteX()
{
    unsigned short address = AbsoluteXAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    return 7;
}
int M6502::OpINX()
{
    registerX++;
    status.zeroFlag = (registerX == 0);
    status.negativeFlag = (registerX & 0x80) > 0;
    return 2;
}
int M6502::OpINY()
{
    registerY++;
    status.zeroFlag = (registerY == 0);
    status.negativeFlag = (registerY & 0x80) > 0;
    return 2;
}
int M6502::OpDECZero() 
{
    unsigned short address = ZeroAddress();
    unsigned char byte = memory.cpuRead(address);
    byte--;
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    memory.cpuWrite(address, byte);
    return 5;
}
int M6502::OpDECZeroX() 
{
    unsigned short address = ZeroXAddress();
    unsigned char byte = memory.cpuRead(address);
    byte--;
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    memory.cpuWrite(address, byte);
    return 6;
}
int M6502::OpDECAbsolute()
{
    unsigned short address = AbsoluteAddress();
    unsigned char byte = memory.cpuRead(address);
    byte--;
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    memory.cpuWrite(address, byte);
    return 6;
}
int M6502::OpDECAbsoluteX()
{
    unsigned short address = AbsoluteXAddress();
    unsigned char byte = memory.cpuRead(address);
    byte--;
    status.zeroFlag = (byte == 0);
    status.negativeFlag = (byte & 0x80) > 0;
    memory.cpuWrite(address, byte);
    return 7;
}
int M6502::OpDEX()
{
    registerX--;
    status.zeroFlag = (registerX == 0);
    status.negativeFlag = (registerX & 0x80) > 0;
    return 2;
}
int M6502::OpDEY() 
{
    registerY--;
    status.zeroFlag = (registerY == 0);
    status.negativeFlag = (registerY & 0x80) > 0;
    return 2;
}

// Shift operations
int M6502::OpASLAccumlator()
{
    status.carryFlag = (accumulator & 0x80) > 0;
    accumulator = accumulator << 1;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;
}
int M6502::OpASLZero()
{
    unsigned char address = ZeroAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x80) > 0;
    byte = byte << 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 5;
}    
int M6502::OpASLZeroX()
{
    unsigned char address = ZeroXAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x80) > 0;
    byte = byte << 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 6;
}    
int M6502::OpASLAbsolute()
{
    unsigned char address = AbsoluteAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x80) > 0;
    byte = byte << 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 6;
}
int M6502::OpASLAbsoluteX()
{
    unsigned char address = AbsoluteXAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x80) > 0;
    byte = byte << 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 7;
}    
    
int M6502::OpLSRAccululator()
{
    status.carryFlag = (accumulator & 0x01) > 0;
    accumulator = accumulator >> 1;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    return 2;    
}
int M6502::OpLSRZero()
{
    unsigned short address = ZeroAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x01) > 0;
    byte = byte >> 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 5;    
}
int M6502::OpLSRZeroX()
{
    unsigned short address = ZeroXAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x01) > 0;
    byte = byte >> 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 6;    
}
int M6502::OpLSRAbsolute()
{
    unsigned short address = AbsoluteAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x01) > 0;
    byte = byte >> 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 6;    
}
int M6502::OpLSRAbsoluteX()
{
    unsigned short address = AbsoluteXAddress();
    unsigned char byte = memory.cpuRead(address);
    status.carryFlag = (byte & 0x01) > 0;
    byte = byte >> 1;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    memory.cpuWrite(address, byte);
    return 7;    
}

int M6502::OpROLAccululator()
{
    unsigned char newCarry = accumulator & 0x80;
    accumulator = (accumulator << 1) + status.carryFlag;
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    status.carryFlag = newCarry > 0;
    return 2;
}
int M6502::OpROLZero()
{
    unsigned short address = ZeroAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x80;
    byte = (byte << 1) + status.carryFlag;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 5;
}
int M6502::OpROLZeroX()
{
    unsigned short address = ZeroXAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x80;
    byte = (byte << 1) + status.carryFlag;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 6;
}
int M6502::OpROLAbsolute()
{
    unsigned short address = AbsoluteAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x80;
    byte = (byte << 1) + status.carryFlag;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 6;
}
int M6502::OpROLAbsoluteX()
{
    unsigned short address = AbsoluteXAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x80;
    byte = (byte << 1) + status.carryFlag;
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 7;
}
    
int M6502::OpRORAccululator()
{
    unsigned char newCarry = accumulator & 0x01;
    accumulator = (accumulator >> 1) + (status.carryFlag << 7);
    status.negativeFlag = (accumulator & 0x80) > 0;
    status.zeroFlag = (accumulator == 0);
    status.carryFlag = newCarry > 0;
    return 2;
}
int M6502::OpRORZero()
{
    unsigned short address = ZeroAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x01;
    byte = (byte >> 1) + (status.carryFlag << 7);
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 5;
}
int M6502::OpRORZeroX()
{
    unsigned short address = ZeroXAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x01;
    byte = (byte >> 1) + (status.carryFlag << 7);
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 6;
}
int M6502::OpRORAbsolute()
{
    unsigned short address = AbsoluteAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x01;
    byte = (byte >> 1) + (status.carryFlag << 7);
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 6;
}
int M6502::OpRORAbsoluteX()
{
    unsigned short address = AbsoluteXAddress();
    unsigned char byte = memory.cpuRead(address);
    unsigned char newCarry = byte & 0x01;
    byte = (byte >> 1) + (status.carryFlag << 7);
    status.negativeFlag = (byte & 0x80) > 0;
    status.zeroFlag = (byte == 0);
    status.carryFlag = newCarry > 0;
    memory.cpuWrite(address, byte);
    return 7;
}

// Jumps and Call operaions
int M6502::OpJMPAbsolute()
{
    unsigned short address = AbsoluteAddress();
    programCounter = address-1;
    return 3;
}
int M6502::OpJMPIndirect()
{
    unsigned short address = IndirectAddress();
    programCounter = address-1;
    return 5;
}

int M6502::OpJSR() 
{
    unsigned short jumpAddress = AbsoluteAddress();
    pushStack((programCounter & 0xff00) >> 8);
    pushStack(programCounter & 0xff);
    programCounter = jumpAddress - 1;
    return 6;
}
int M6502::OpRTS() 
{
    unsigned char loadl = popStack();
    unsigned char loadh = popStack();
    unsigned short load = (loadh << 8) + loadl;
    programCounter = load;
    return 6;
}

// branch operations
int M6502::OpBCC() 
{
    char relativeAddress = readRelative();
    if (status.carryFlag == false)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}
int M6502::OpBCS() 
{
    char relativeAddress = readRelative();
    if (status.carryFlag == true)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}
int M6502::OpBEQ() 
{
    char relativeAddress = readRelative();
    if (status.zeroFlag == true)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}
int M6502::OpBMI() 
{
    char relativeAddress = readRelative();
    if (status.negativeFlag == true)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}    
int M6502::OpBNE() 
{ 
    char relativeAddress = readRelative();
    if (status.zeroFlag == false)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}
int M6502::OpBPL() 
{
    char relativeAddress = readRelative();
    if (status.negativeFlag == false)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}
int M6502::OpBVC() 
{
    char relativeAddress = readRelative();
    if (status.overflowFlag == false)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}
int M6502::OpBVS() 
{
    char relativeAddress = readRelative();
    if (status.overflowFlag == true)
    {
        programCounter += relativeAddress;
        return 3;
    }
    return 2;
}

// Status Flag operations
int M6502::OpCLC() 
{
    status.carryFlag = 0;
    return 2;
}
int M6502::OpCLD() 
{
    status.decimalMode = 0;
    return 2;
}
int M6502::OpCLI() 
{
    status.interruptDisable = 0;
    return 2;
}
int M6502::OpCLV() 
{
    status.overflowFlag = 0;
    return 2;
}
int M6502::OpSEC() 
{
    status.carryFlag = 1;
    return 2;
}
int M6502::OpSED() 
{
    status.decimalMode = 1;
    return 2;
}
int M6502::OpSEI() 
{
    status.interruptDisable = 1;
    return 2;    
}

// System operations
int M6502::OpBRK() { throw "Not implemented"; }

int M6502::OpNOP() 
{
    return 2;
}

int M6502::OpRTI()
{
    OpPLP();
    unsigned char loadl = popStack();
    unsigned char loadh = popStack();
    programCounter = (loadh << 8) + loadl;
    return 6;
}

int M6502::OpISCZero()
{
    unsigned short address = ZeroAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    accumulator -= byte;
    status.zeroFlag = (accumulator == 0);
    status.negativeFlag = (accumulator & 0x80) > 0;
    return 5;
}
int M6502::OpISCZeroX()
{
    unsigned short address = ZeroXAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    accumulator -= byte;
    status.zeroFlag = (accumulator == 0);
    status.negativeFlag = (accumulator & 0x80) > 0;
    return 6;
}
int M6502::OpISCAbsolute()
{
    unsigned short address = AbsoluteAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    accumulator -= byte;
    status.zeroFlag = (accumulator == 0);
    status.negativeFlag = (accumulator & 0x80) > 0;
    return 6;
}
int M6502::OpISCAbsoluteX()
{
    unsigned short address = AbsoluteXAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    accumulator -= byte;
    status.zeroFlag = (accumulator == 0);
    status.negativeFlag = (accumulator & 0x80) > 0;
    return 7;
}
int M6502::OpISCAbsoluteY()
{
    unsigned short address = AbsoluteYAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    accumulator -= byte;
    status.zeroFlag = (accumulator == 0);
    status.negativeFlag = (accumulator & 0x80) > 0;
    return 7;
}
int M6502::OpISCIndirectX()
{
    unsigned short address = IndirectXAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    accumulator -= byte;
    status.zeroFlag = (accumulator == 0);
    status.negativeFlag = (accumulator & 0x80) > 0;
    return 8;
}
int M6502::OpISCIndirectY()
{
    unsigned short address = IndirectYAddress();
    unsigned char byte = memory.cpuRead(address);
    byte++;
    memory.cpuWrite(address, byte);
    accumulator -= byte;
    status.zeroFlag = (accumulator == 0);
    status.negativeFlag = (accumulator & 0x80) > 0;
    return 4;
}
    
