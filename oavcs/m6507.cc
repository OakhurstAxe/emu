
#include "headers/m6507.h"

#include <QDebug>

namespace oa
{
    namespace vcs
    {
        
        M6507::M6507(emu::IMemoryMapper *memory) : M6502(memory)
        {
            stackPointerMax_ = 0xff;
            stackPointerMin_ = 0x80;
        }
        
        void M6507::PushStack(uint8_t byte)
        {
            memory_->CpuWrite(stackPointer_, byte);
            stackPointer_--;
        }
        
        uint8_t M6507::PopStack(void)
        {
            stackPointer_++;
            return memory_->CpuRead(stackPointer_);            
        }

        void M6507::OpTSX(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            registerX_ = stackPointer_;
            statusRegister_.negativeFlag = (registerX_ & 0x80) > 0;
            statusRegister_.zeroFlag = (registerX_ == 0);
        }
        void M6507::OpTXS(AddressMethod addressMethod) 
        {
            Q_UNUSED(addressMethod);
            stackPointer_ = registerX_;
        }
    }
}
