#ifndef M6502_H
#define M6502_H

#include "basecpu.h"

#include "../oanes/headers/nesmemory.h"

namespace oa
{
    namespace emu
    {
        
        class M6502 : BaseCpu
        {

        public:
            oa::nes::NesMemory memory;
            virtual int executeTicks(int count);
            virtual void reset();
            
            int nmiSet = 0;
            
        protected:
            void pushStack(unsigned char byte);
            unsigned char popStack(void);
            
            int prevInstruction = 0;
            unsigned short programCounter;
            unsigned short stackPointer;
            unsigned char accumulator;
            unsigned char registerX;
            unsigned char registerY;
            
            union
            {
                struct
                {
                    unsigned char carryFlag: 1;
                    unsigned char zeroFlag: 1;
                    unsigned char interruptDisable: 1;
                    unsigned char decimalMode: 1;
                    unsigned char breakCommand: 1;
                    unsigned char ignored: 1;
                    unsigned char overflowFlag: 1;
                    unsigned char negativeFlag: 1;
                };
                unsigned char reg;
            }
            status;
            
            // addressing modes
            unsigned short ZeroAddress();
            unsigned short ZeroXAddress();
            unsigned short ZeroYAddress();
            unsigned short AbsoluteAddress();
            unsigned short AbsoluteXAddress();
            unsigned short AbsoluteYAddress();
            unsigned short IndirectAddress();
            unsigned short IndirectXAddress();
            unsigned short IndirectYAddress();
            unsigned char readRelative();
            unsigned char readImmediate();
            unsigned char readZero();
            unsigned char readZeroX();
            unsigned char readZeroY();
            unsigned char readAbsolute();
            unsigned char readAbsoluteX();
            unsigned char readAbsoluteY();
            unsigned char readIndirectX();
            unsigned char readIndirectY();
            void writeZero(unsigned char byte);
            void writeZeroX(unsigned char byte);
            void writeZeroY(unsigned char byte);
            void writeAbsolute(unsigned char byte);
            void writeAbsoluteX(unsigned char byte);
            void writeAbsoluteY(unsigned char byte);
            void writeIndirectX(unsigned char byte);
            void writeIndirectY(unsigned char byte);
            
            // Load Store operations
            virtual int OpLDAImmediate();
            virtual int OpLDAZero();
            virtual int OpLDAZeroX();
            virtual int OpLDAAbsolute();
            virtual int OpLDAAbsoluteX();
            virtual int OpLDAAbsoluteY();
            virtual int OpLDAIndirectX();
            virtual int OpLDAIndirectY();
            
            virtual int OpLDXImmediate();
            virtual int OpLDXZero();
            virtual int OpLDXZeroY();
            virtual int OpLDXAbsolute();
            virtual int OpLDXAbsoluteY();
            
            virtual int OpLDYImmediate();
            virtual int OpLDYZero();
            virtual int OpLDYZeroX();
            virtual int OpLDYAbsolute();
            virtual int OpLDYAbsoluteX();
            
            virtual int OpSTAZero();
            virtual int OpSTAZeroX();
            virtual int OpSTAAbsolute();
            virtual int OpSTAAbsoluteX();
            virtual int OpSTAAbsoluteY();
            virtual int OpSTAIndirectX();
            virtual int OpSTAIndirectY();

            virtual int OpSTXZero();
            virtual int OpSTXZeroY();
            virtual int OpSTXAbsolute();
            
            virtual int OpSTYZero();
            virtual int OpSTYZeroX();
            virtual int OpSTYAbsolute();
            
            
            // Register transfers
            virtual int OpTAX();
            virtual int OpTAY();
            virtual int OpTXA();
            virtual int OpTYA();
            
            
            // Stack operaions
            virtual int OpTSX();
            virtual int OpTXS();
            virtual int OpPHA();
            virtual int OpPHP();
            virtual int OpPLA();
            virtual int OpPLP();

            
            // Logical operations
            virtual int OpANDImmediate();
            virtual int OpANDZero();
            virtual int OpANDZeroX();
            virtual int OpANDAbsolute();
            virtual int OpANDAbsoluteX();
            virtual int OpANDAbsoluteY();
            virtual int OpANDIndirectX();
            virtual int OpANDIndirectY();

            virtual int OpEORImmediate();
            virtual int OpEORZero();
            virtual int OpEORZeroX();
            virtual int OpEORAbsolute();
            virtual int OpEORAbsoluteX();
            virtual int OpEORAbsoluteY();
            virtual int OpEORIndirectX();
            virtual int OpEORIndirectY();
            
            virtual int OpORAImmediate();
            virtual int OpORAZero();
            virtual int OpORAZeroX();
            virtual int OpORAAbsolute();
            virtual int OpORAAbsoluteX();
            virtual int OpORAAbsoluteY();
            virtual int OpORAIndirectX();
            virtual int OpORAIndirectY();
            
            virtual int OpBITZero();
            virtual int OpBITAbsolute();
            
            
            // Arithmetic operations
            virtual int OpADCImmediate();
            virtual int OpADCZero();
            virtual int OpADCZeroX();
            virtual int OpADCAbsolute();
            virtual int OpADCAbsoluteX();
            virtual int OpADCAbsoluteY();
            virtual int OpADCIndirectX();
            virtual int OpADCIndirectY();
            
            virtual int OpSBCImmediate();
            virtual int OpSBCZero();
            virtual int OpSBCZeroX();
            virtual int OpSBCAbsolute();
            virtual int OpSBCAbsoluteX();
            virtual int OpSBCAbsoluteY();
            virtual int OpSBCIndirectX();
            virtual int OpSBCIndirectY();
            
            virtual int OpCMPImmediate();
            virtual int OpCMPZero();
            virtual int OpCMPZeroX();
            virtual int OpCMPAbsolute();
            virtual int OpCMPAbsoluteX();
            virtual int OpCMPAbsoluteY();
            virtual int OpCMPIndirectX();
            virtual int OpCMPIndirectY();
            
            virtual int OpCPXImmediate();
            virtual int OpCPXZero();
            virtual int OpCPXAbsolute();

            virtual int OpCPYImmediate();
            virtual int OpCPYZero();
            virtual int OpCPYAbsolute();

            
            // Increment and decrement operations
            virtual int OpINCZero();
            virtual int OpINCZeroX();
            virtual int OpINCAbsolute();
            virtual int OpINCAbsoluteX();
            virtual int OpINX();
            virtual int OpINY();
            
            virtual int OpDECZero();
            virtual int OpDECZeroX();
            virtual int OpDECAbsolute();
            virtual int OpDECAbsoluteX();
            
            virtual int OpDEX();
            virtual int OpDEY();
            
            
            // Shift operations
            virtual int OpASLAccumlator();
            virtual int OpASLZero();
            virtual int OpASLZeroX();
            virtual int OpASLAbsolute();
            virtual int OpASLAbsoluteX();
            
            virtual int OpLSRAccululator();
            virtual int OpLSRZero();
            virtual int OpLSRZeroX();
            virtual int OpLSRAbsolute();
            virtual int OpLSRAbsoluteX();
            
            virtual int OpROLAccululator();
            virtual int OpROLZero();
            virtual int OpROLZeroX();
            virtual int OpROLAbsolute();
            virtual int OpROLAbsoluteX();
            
            virtual int OpRORAccululator();
            virtual int OpRORZero();
            virtual int OpRORZeroX();
            virtual int OpRORAbsolute();
            virtual int OpRORAbsoluteX();
            
            // Jumps and Call operaions
            virtual int OpJMPAbsolute();
            virtual int OpJMPIndirect();
            virtual int OpJSR();
            virtual int OpRTS();

            
            // branch operations
            virtual int OpBCC();
            virtual int OpBCS();
            virtual int OpBEQ();
            virtual int OpBMI();
            virtual int OpBNE();
            virtual int OpBPL();
            virtual int OpBVC();
            virtual int OpBVS();
            
            
            // Status Flag operations
            virtual int OpCLC();
            virtual int OpCLD();
            virtual int OpCLI();
            virtual int OpCLV();
            virtual int OpSEC();
            virtual int OpSED();
            virtual int OpSEI();
            
            
            // System operations
            virtual int OpBRK();
            virtual int OpNOP();
            virtual int OpRTI();
            
            
            // ISC (Discouraged opcodes)
            virtual int OpISCZero();
            virtual int OpISCZeroX();
            virtual int OpISCAbsolute();
            virtual int OpISCAbsoluteX();
            virtual int OpISCAbsoluteY();
            virtual int OpISCIndirectX();
            virtual int OpISCIndirectY();
        };

    }
}
#endif // M6502_H
