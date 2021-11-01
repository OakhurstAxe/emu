#ifndef _OA_NES_R2A02
#define _OA_NES_R2A02

#include "oaemucpu/headers/m6502.h"
#include "oaemumemory/headers/imemorymapper.h"

namespace oa
{
    namespace nes
    {
        
        class R2A03: public emu::M6502
        {
        public:
            R2A03(emu::IMemoryMapper *memory);
            virtual ~R2A03();
            void ExecuteTick() override;
            void Reset() override;
            void SetNmi();
            void DmaSuspend();
        protected:
            void SetOpCodes() override;
            void OpSED(AddressMethod addressMethod) override;
            virtual void OpISC(AddressMethod addressMethod);
            virtual void OpLAX(AddressMethod addressMethod);
            virtual void OpSAX(AddressMethod addressMethod);
            virtual void OpDCP(AddressMethod addressMethod);
            virtual void OpSLO(AddressMethod addressMethod);
            virtual void OpRLA(AddressMethod addressMethod);
            virtual void OpSRE(AddressMethod addressMethod);
            virtual void OpRRA(AddressMethod addressMethod);
        private:
            bool isNmiSet_;
        };
        
    }
}

#endif
