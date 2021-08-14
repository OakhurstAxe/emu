
#include "headers/r2a03.h"

#include <functional>

namespace oa
{
    namespace nes
    {
    
        R2A03::R2A03(emu::MemoryMapper *memory) : emu::M6502(memory)
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
                programCounter_--;
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
            
            opCodeLookup_[0xe3] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::IndirectXAddress};
            opCodeLookup_[0xe7] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::ZeroAddress};
            opCodeLookup_[0xef] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::AbsoluteAddress};
            opCodeLookup_[0xf3] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::IndirectYAddress};
            opCodeLookup_[0xf7] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::ZeroXAddress};
            opCodeLookup_[0xfb] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::AbsoluteYAddress};
            opCodeLookup_[0xff] = {(OperationMethod)&R2A03::OpISC, (AddressMethod)&R2A03::AbsoluteXAddress};
        }

        void R2A03::OpISC(AddressMethod addressMethod)
        {
            unsigned short address = CallAddressMethod(addressMethod);
            unsigned char byte = memory_->CpuRead(address);
            byte++;
            memory_->CpuWrite(address, byte);
            accumulator_ -= byte;
            statusRegister_.zeroFlag = (accumulator_ == 0);
            statusRegister_.negativeFlag = (accumulator_ & 0x80) > 0;
            overflowTicks_ += 5;
        }

    }
}
