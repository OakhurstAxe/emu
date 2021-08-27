#ifndef OA_EMU_M6502_H
#define OA_EMU_M6502_H

#include "basecpu.h"

#include "../oaemumemory/headers/memorymapper.h"

#define CallAddressMethod(methodName) std::__invoke(methodName, this)
#define CallOpMethod(methodNane, addressMethod) std::__invoke(methodNane, this, addressMethod);

namespace oa
{
    namespace emu
    {

        class M6502 : public BaseCpu
        {
        public:
            M6502(MemoryMapper *memory);
            virtual ~M6502();
            virtual void ExecuteTick();
            virtual void Reset();
        protected:
            typedef uint16_t (M6502::*AddressMethod)();
            typedef void (M6502::*OperationMethod)(AddressMethod addressMethod);
            int prevInt1 = 0;
            int prevInt2 = 0;
            int prevInt3 = 0;
            int prevInt4 = 0;
            int prevInt5 = 0;
            int prevInt6 = 0;
            int prevInt7 = 0;
            int prevInt8 = 0;
            int prevInt9 = 0;
            int prevInt10 = 0;

            struct OperationStruct
            {
                OperationMethod operation_;
                AddressMethod addressMethod_;
                uint8_t ticks;
            };
            OperationStruct opCodeLookup_[0x100];
            
            virtual void SetOpCodes();
            virtual void PushStack(uint8_t byte);
            virtual uint8_t PopStack(void);
            
            MemoryMapper *memory_;
            uint16_t stackPointerMax_;
            uint16_t stackPointerMin_;
            
            uint16_t programCounter_;
            uint16_t stackPointer_;
            uint8_t accumulator_;
            uint8_t registerX_;
            uint8_t registerY_;
            
            union
            {
                struct
                {
                    uint8_t carryFlag: 1;
                    uint8_t zeroFlag: 1;
                    uint8_t interruptDisable: 1;
                    uint8_t decimalMode: 1;
                    uint8_t breakCommand: 1;
                    uint8_t ignored: 1;
                    uint8_t overflowFlag: 1;
                    uint8_t negativeFlag: 1;
                };
                uint8_t register_;
            }
            statusRegister_;
            
            // addressing modes
            uint16_t NullAddress();
            uint16_t Accumlator();
            uint16_t ImmediateAddress();
            uint16_t ZeroAddress();
            uint16_t ZeroXAddress();
            uint16_t ZeroYAddress();
            uint16_t AbsoluteAddress();
            uint16_t AbsoluteXAddress();
            uint16_t AbsoluteYAddress();
            uint16_t IndirectAddress();
            uint16_t IndirectXAddress();
            uint16_t IndirectYAddress();
            
            // Load Store operations
            virtual void OpLDA(AddressMethod addressMethod);
            virtual void OpLDX(AddressMethod addressMethod);
            virtual void OpLDY(AddressMethod addressMethod);
            virtual void OpSTA(AddressMethod addressMethod);
            virtual void OpSTX(AddressMethod addressMethod);
            virtual void OpSTY(AddressMethod addressMethod);
            
            // Register transfers
            virtual void OpTAX(AddressMethod addressMethod);
            virtual void OpTAY(AddressMethod addressMethod);
            virtual void OpTXA(AddressMethod addressMethod);
            virtual void OpTYA(AddressMethod addressMethod);
            
            // Stack operaions
            virtual void OpTSX(AddressMethod addressMethod);
            virtual void OpTXS(AddressMethod addressMethod);
            virtual void OpPHA(AddressMethod addressMethod);
            virtual void OpPHP(AddressMethod addressMethod);
            virtual void OpPLA(AddressMethod addressMethod);
            virtual void OpPLP(AddressMethod addressMethod);
            
            // Logical operations
            virtual void OpAND(AddressMethod addressMethod);
            virtual void OpEOR(AddressMethod addressMethod);
            virtual void OpORA(AddressMethod addressMethod);
            virtual void OpBIT(AddressMethod addressMethod);
            
            // Arithmetic operations
            virtual void OpADC(AddressMethod addressMethod);
            virtual void OpSBC(AddressMethod addressMethod);
            virtual void OpCMP(AddressMethod addressMethod);
            virtual void OpCPX(AddressMethod addressMethod);
            virtual void OpCPY(AddressMethod addressMethod);
            
            // Increment and decrement operations
            virtual void OpINC(AddressMethod addressMethod);
            virtual void OpINX(AddressMethod addressMethod);
            virtual void OpINY(AddressMethod addressMethod);
            virtual void OpDEC(AddressMethod addressMethod);
            virtual void OpDEX(AddressMethod addressMethod);
            virtual void OpDEY(AddressMethod addressMethod);
            
            // Shift operations
            virtual void OpASL(AddressMethod addressMethod);
            virtual void OpLSR(AddressMethod addressMethod);
            virtual void OpROL(AddressMethod addressMethod);
            virtual void OpROR(AddressMethod addressMethod);
            
            // Jumps and Call operaions
            virtual void OpJMP(AddressMethod addressMethod);
            virtual void OpJSR(AddressMethod addressMethod);
            virtual void OpRTS(AddressMethod addressMethod);
            
            // branch operations
            virtual void OpBCC(AddressMethod addressMethod);
            virtual void OpBCS(AddressMethod addressMethod);
            virtual void OpBEQ(AddressMethod addressMethod);
            virtual void OpBMI(AddressMethod addressMethod);
            virtual void OpBNE(AddressMethod addressMethod);
            virtual void OpBPL(AddressMethod addressMethod);
            virtual void OpBVC(AddressMethod addressMethod);
            virtual void OpBVS(AddressMethod addressMethod);
            
            // Status Flag operations
            virtual void OpCLC(AddressMethod addressMethod);
            virtual void OpCLD(AddressMethod addressMethod);
            virtual void OpCLI(AddressMethod addressMethod);
            virtual void OpCLV(AddressMethod addressMethod);
            virtual void OpSEC(AddressMethod addressMethod);
            virtual void OpSED(AddressMethod addressMethod);
            virtual void OpSEI(AddressMethod addressMethod);
            
            // System operations
            virtual void OpBRK(AddressMethod addressMethod);
            virtual void OpNOP(AddressMethod addressMethod);
            virtual void OpRTI(AddressMethod addressMethod);
        };

    }
}
#endif // M6502_H
