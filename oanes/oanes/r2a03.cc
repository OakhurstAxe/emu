
#include "headers/r2a03.h"

#include <functional>

namespace oa
{
    namespace nes
    {
    
        R2A03::R2A03(emu::IMemoryMapper *memory) : emu::M6502(memory)
        {
            SetOpCodes();
        }
        
        R2A03::~R2A03()
        {
        }
        
        void R2A03::ExecuteTick()
        {
            if (isNmiSet_)
            {
                isNmiSet_ = false;
                //programCounter_--;
                PushStack((programCounter_ & 0xff00) >> 8);
                PushStack(programCounter_ & 0x00ff);
                OpPHP(&R2A03::NullAddress);
                unsigned char loadl = memory_->CpuRead(0xfffa);
                unsigned char loadh = memory_->CpuRead(0xfffb);
                programCounter_ = (loadh << 8) + loadl;
            }
            else
            {
                M6502::ExecuteTick();
            }
        }
        
        void R2A03::Reset()
        {
            isNmiSet_ = false;
            M6502::Reset();
        }
        
        void R2A03::SetNmi()
        {
            isNmiSet_ = true;
        }
        
        void R2A03::DmaSuspend()
        {
            overflowTicks_ += 154;
        }
        
        void R2A03::SetOpCodes()
        {
            M6502::SetOpCodes();
            
            opCodeLookup_[0x03] = {(OperationMethod)&R2A03::OpSLO, (AddressMethod)&R2A03::IndirectXAddress, 8};
            opCodeLookup_[0x04] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroAddress, 2};
            opCodeLookup_[0x07] = {(OperationMethod)&R2A03::OpSLO, (AddressMethod)&R2A03::ZeroAddress, 5};
            opCodeLookup_[0x0c] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::AbsoluteAddress, 2};
            opCodeLookup_[0x0f] = {(OperationMethod)&R2A03::OpSLO, (AddressMethod)&R2A03::AbsoluteAddress, 6};

            opCodeLookup_[0x13] = {(OperationMethod)&R2A03::OpSLO, (AddressMethod)&R2A03::IndirectYAddress, 8};
            opCodeLookup_[0x14] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroXAddress, 2};
            opCodeLookup_[0x17] = {(OperationMethod)&R2A03::OpSLO, (AddressMethod)&R2A03::ZeroXAddress, 6};
            opCodeLookup_[0x1a] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::NullAddress, 2};
            opCodeLookup_[0x1b] = {(OperationMethod)&R2A03::OpSLO, (AddressMethod)&R2A03::AbsoluteYAddress, 7};
            opCodeLookup_[0x1c] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::AbsoluteXAddress, 2};
            opCodeLookup_[0x1f] = {(OperationMethod)&R2A03::OpSLO, (AddressMethod)&R2A03::AbsoluteXAddress, 7};

            opCodeLookup_[0x23] = {(OperationMethod)&R2A03::OpRLA, (AddressMethod)&R2A03::IndirectXAddress, 8};
            opCodeLookup_[0x27] = {(OperationMethod)&R2A03::OpRLA, (AddressMethod)&R2A03::ZeroAddress, 5};
            opCodeLookup_[0x2f] = {(OperationMethod)&R2A03::OpRLA, (AddressMethod)&R2A03::AbsoluteAddress, 6};

            opCodeLookup_[0x33] = {(OperationMethod)&R2A03::OpRLA, (AddressMethod)&R2A03::IndirectYAddress, 8};
            opCodeLookup_[0x34] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroXAddress, 2};
            opCodeLookup_[0x37] = {(OperationMethod)&R2A03::OpRLA, (AddressMethod)&R2A03::ZeroXAddress, 6};
            opCodeLookup_[0x3a] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::NullAddress, 2};
            opCodeLookup_[0x3b] = {(OperationMethod)&R2A03::OpRLA, (AddressMethod)&R2A03::AbsoluteYAddress, 7};
            opCodeLookup_[0x3c] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::AbsoluteXAddress, 2};
            opCodeLookup_[0x3f] = {(OperationMethod)&R2A03::OpRLA, (AddressMethod)&R2A03::AbsoluteXAddress, 7};

            opCodeLookup_[0x43] = {(OperationMethod)&R2A03::OpSRE, (AddressMethod)&R2A03::IndirectXAddress, 8};
            opCodeLookup_[0x44] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroAddress, 2};
            opCodeLookup_[0x47] = {(OperationMethod)&R2A03::OpSRE, (AddressMethod)&R2A03::ZeroAddress, 5};
            opCodeLookup_[0x4f] = {(OperationMethod)&R2A03::OpSRE, (AddressMethod)&R2A03::AbsoluteAddress, 6};

            opCodeLookup_[0x53] = {(OperationMethod)&R2A03::OpSRE, (AddressMethod)&R2A03::IndirectYAddress, 8};
            opCodeLookup_[0x54] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroXAddress, 2};
            opCodeLookup_[0x57] = {(OperationMethod)&R2A03::OpSRE, (AddressMethod)&R2A03::ZeroXAddress, 6};
            opCodeLookup_[0x5a] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::NullAddress, 2};
            opCodeLookup_[0x5b] = {(OperationMethod)&R2A03::OpSRE, (AddressMethod)&R2A03::AbsoluteYAddress, 7};
            opCodeLookup_[0x5c] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::AbsoluteXAddress, 2};
            opCodeLookup_[0x5f] = {(OperationMethod)&R2A03::OpSRE, (AddressMethod)&R2A03::AbsoluteXAddress, 7};

            opCodeLookup_[0x63] = {(OperationMethod)&R2A03::OpRRA, (AddressMethod)&R2A03::IndirectXAddress, 8};
            opCodeLookup_[0x64] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroAddress, 2};
            opCodeLookup_[0x67] = {(OperationMethod)&R2A03::OpRRA, (AddressMethod)&R2A03::ZeroAddress, 5};
            opCodeLookup_[0x6f] = {(OperationMethod)&R2A03::OpRRA, (AddressMethod)&R2A03::AbsoluteAddress, 6};

            opCodeLookup_[0x73] = {(OperationMethod)&R2A03::OpRRA, (AddressMethod)&R2A03::IndirectYAddress, 8};
            opCodeLookup_[0x74] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroXAddress, 2};
            opCodeLookup_[0x77] = {(OperationMethod)&R2A03::OpRRA, (AddressMethod)&R2A03::ZeroXAddress, 6};
            opCodeLookup_[0x7a] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::NullAddress, 2};
            opCodeLookup_[0x7b] = {(OperationMethod)&R2A03::OpRRA, (AddressMethod)&R2A03::AbsoluteYAddress, 7};
            opCodeLookup_[0x7c] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::AbsoluteXAddress, 2};
            opCodeLookup_[0x7f] = {(OperationMethod)&R2A03::OpRRA, (AddressMethod)&R2A03::AbsoluteXAddress, 7};

            opCodeLookup_[0x80] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ImmediateAddress, 6};
            opCodeLookup_[0x82] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ImmediateAddress, 6};
            opCodeLookup_[0x83] = {(OperationMethod)&R2A03::OpSAX, (AddressMethod)&R2A03::IndirectXAddress, 6};
            opCodeLookup_[0x87] = {(OperationMethod)&R2A03::OpSAX, (AddressMethod)&R2A03::ZeroAddress, 3};
            opCodeLookup_[0x89] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ImmediateAddress, 6};
            opCodeLookup_[0x8f] = {(OperationMethod)&R2A03::OpSAX, (AddressMethod)&R2A03::AbsoluteAddress, 4};

            opCodeLookup_[0x97] = {(OperationMethod)&R2A03::OpSAX, (AddressMethod)&R2A03::ZeroYAddress, 4};

            opCodeLookup_[0xa3] = {(OperationMethod)&R2A03::OpLAX, (AddressMethod)&R2A03::IndirectXAddress, 6};
            opCodeLookup_[0xa7] = {(OperationMethod)&R2A03::OpLAX, (AddressMethod)&R2A03::ZeroAddress, 3};
            opCodeLookup_[0xab] = {(OperationMethod)&R2A03::OpLAX, (AddressMethod)&R2A03::ImmediateAddress, 3};
            opCodeLookup_[0xaf] = {(OperationMethod)&R2A03::OpLAX, (AddressMethod)&R2A03::AbsoluteAddress, 4};

            opCodeLookup_[0xb3] = {(OperationMethod)&R2A03::OpLAX, (AddressMethod)&R2A03::IndirectYAddress, 5};
            opCodeLookup_[0xb7] = {(OperationMethod)&R2A03::OpLAX, (AddressMethod)&R2A03::ZeroYAddress, 4};
            opCodeLookup_[0xbf] = {(OperationMethod)&R2A03::OpLAX, (AddressMethod)&R2A03::AbsoluteYAddress, 4};

            opCodeLookup_[0xc2] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ImmediateAddress, 6};
            opCodeLookup_[0xc3] = {(OperationMethod)&R2A03::OpDCP, (AddressMethod)&R2A03::IndirectXAddress, 8};
            opCodeLookup_[0xc7] = {(OperationMethod)&R2A03::OpDCP, (AddressMethod)&R2A03::ZeroAddress, 5};
            opCodeLookup_[0xcf] = {(OperationMethod)&R2A03::OpDCP, (AddressMethod)&R2A03::AbsoluteAddress, 6};

            opCodeLookup_[0xd3] = {(OperationMethod)&R2A03::OpDCP, (AddressMethod)&R2A03::IndirectYAddress, 8};
            opCodeLookup_[0xd4] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroXAddress, 2};
            opCodeLookup_[0xd7] = {(OperationMethod)&R2A03::OpDCP, (AddressMethod)&R2A03::ZeroXAddress, 6};
            opCodeLookup_[0xda] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::NullAddress, 2};
            opCodeLookup_[0xdb] = {(OperationMethod)&R2A03::OpDCP, (AddressMethod)&R2A03::AbsoluteYAddress, 7};
            opCodeLookup_[0xdc] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::AbsoluteXAddress, 2};
            opCodeLookup_[0xdf] = {(OperationMethod)&R2A03::OpDCP, (AddressMethod)&R2A03::AbsoluteXAddress, 7};

            opCodeLookup_[0xe2] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ImmediateAddress, 6};
            opCodeLookup_[0xe3] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::IndirectXAddress, 8};
            opCodeLookup_[0xe7] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::ZeroAddress, 5};
            opCodeLookup_[0xea] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::NullAddress, 6};
            opCodeLookup_[0xeb] = {(OperationMethod)&R2A03::OpSBC, (AddressMethod)&R2A03::ImmediateAddress, 3};
            opCodeLookup_[0xef] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::AbsoluteAddress, 6};

            opCodeLookup_[0xf3] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::IndirectYAddress, 8};
            opCodeLookup_[0xf4] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::ZeroXAddress, 2};
            opCodeLookup_[0xf7] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::ZeroXAddress, 6};
            opCodeLookup_[0xfa] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::NullAddress, 2};
            opCodeLookup_[0xfb] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::AbsoluteYAddress, 7};
            opCodeLookup_[0xfc] = {(OperationMethod)&R2A03::OpNOP, (AddressMethod)&R2A03::AbsoluteXAddress, 2};
            opCodeLookup_[0xff] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::AbsoluteXAddress, 7};
        }

        void R2A03::OpLAX(AddressMethod addressMethod)
        {
            OpLDA(addressMethod);
            registerX_ = accumulator_;
            statusRegister_.negativeFlag = (registerX_ & 0x80) > 0;
            statusRegister_.zeroFlag = (registerX_ == 0);            
        }
        
        void R2A03::OpSAX(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = accumulator_ & registerX_;
            memory_->CpuWrite(address, byte);
        }
        
        void R2A03::OpDCP(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = memory_->CpuRead(address);
            byte--;
            if (addressMethod != &R2A03::ImmediateAddress)
            {
                memory_->CpuWrite(address, byte);
            }
            statusRegister_.zeroFlag = (byte == 0);
            statusRegister_.negativeFlag = (byte & 0x80) > 0;
            byte = memory_->CpuRead(address);
            statusRegister_.carryFlag = (accumulator_ >= byte);
            statusRegister_.zeroFlag = (accumulator_ == byte);
            statusRegister_.negativeFlag = ((accumulator_ - byte) & 0x80) > 0;            
        }

        void R2A03::OpISC(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = memory_->CpuRead(address);
            byte++;
            memory_->CpuWrite(address, byte);
            statusRegister_.zeroFlag = (byte == 0);
            statusRegister_.negativeFlag = (byte & 0x80) > 0;
            
            byte = byte ^ 0xff;            
            uint16_t value = accumulator_ + byte + statusRegister_.carryFlag;
            statusRegister_.carryFlag = (value & 0xff00) > 0;
            statusRegister_.zeroFlag = (value & 0xff) == 0;
            statusRegister_.overflowFlag  = ((value ^ accumulator_) & (value ^ byte) & 0x80) > 0;
            statusRegister_.negativeFlag = (value & 0x80) > 0;
            accumulator_ = value & 0xff;            
        }

        void R2A03::OpSLO(AddressMethod addressMethod)
        {
            uint16_t address = CallAddressMethod(addressMethod);
            uint8_t byte = memory_->CpuRead(address);
            statusRegister_.carryFlag = (byte & 0x80) > 0;
            byte = byte << 1;
            statusRegister_.negativeFlag = (byte & 0x80) > 0;
            statusRegister_.zeroFlag = (byte == 0);
            memory_->CpuWrite(address, byte);
            
            accumulator_ |= byte;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) > 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);            
        }

        void R2A03::OpRLA(AddressMethod addressMethod)
        {
            uint8_t byte = 0;
            uint16_t address = 0;
            address = CallAddressMethod(addressMethod);
            byte = memory_->CpuRead(address);
            uint8_t temp = (byte << 1) + statusRegister_.carryFlag;
            statusRegister_.carryFlag = (byte & 0x80) > 0;
            byte = temp;
            statusRegister_.negativeFlag = (byte & 0x80) > 0;
            statusRegister_.zeroFlag = (byte == 0);
            memory_->CpuWrite(address, byte);

            accumulator_ &= byte;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) > 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);            
        }
        
        void R2A03::OpSRE(AddressMethod addressMethod)
        {
            uint8_t byte = 0;
            uint16_t address = 0;
            address = CallAddressMethod(addressMethod);
            byte = memory_->CpuRead(address);
            statusRegister_.carryFlag = (byte & 0x01) > 0;
            byte = byte >> 1;
            statusRegister_.negativeFlag = (byte & 0x80) > 0;
            statusRegister_.zeroFlag = (byte == 0);
            memory_->CpuWrite(address, byte);

            accumulator_ ^= byte;
            statusRegister_.negativeFlag = (accumulator_ & 0x80) > 0;
            statusRegister_.zeroFlag = (accumulator_ == 0);
        }
        
        void R2A03::OpRRA(AddressMethod addressMethod)
        {
            uint8_t byte = 0;
            uint16_t address = 0;
            address = CallAddressMethod(addressMethod);
            byte = memory_->CpuRead(address);
            uint8_t temp = (byte >> 1) + (statusRegister_.carryFlag << 7);
            statusRegister_.carryFlag = (byte & 0x01) > 0;
            byte = temp;
            statusRegister_.negativeFlag = (byte & 0x80) > 0;
            statusRegister_.zeroFlag = (byte == 0);
            memory_->CpuWrite(address, byte);

            uint16_t value = accumulator_ + byte + statusRegister_.carryFlag;
            statusRegister_.carryFlag = (value > 255);
            statusRegister_.zeroFlag = ((value & 0xff) == 0);
            statusRegister_.overflowFlag = (~(accumulator_ ^ byte) & (accumulator_ ^ value) & 0x80) > 0;
            statusRegister_.negativeFlag = (value & 0x80) > 0;
            accumulator_ = (value & 0xff);
        }
    }
}
