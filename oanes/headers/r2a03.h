#ifndef _OA_NES_R2A02
#define _OA_NES_R2A02

#include "../oaemucpu/headers/m6502.h"
#include "../oaemumemory/headers/memorymapper.h"

namespace oa
{
    namespace nes
    {
        
        class R2A03: public emu::M6502
        {
        public:
            R2A03(emu::MemoryMapper *memory);
            virtual ~R2A03();
            void ExecuteTick() override;
            void Reset() override;
            void SetNmi();
            void DmaSuspend();
        protected:
            // ISC (Discouraged opcodes)
            virtual void OpISC(AddressMethod addressMethod);
            void SetOpCodes() override;
        private:
            bool isNmiSet_;
        };
        
    }
}

#endif

