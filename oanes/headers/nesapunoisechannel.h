#ifndef _OA_NES_APUNOISECHANNEL_H
#define _OA_NES_APUNOISECHANNEL_H

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuNoiseChannel : public NesApuChannel
        {
        public:
            NesApuNoiseChannel();
            virtual ~NesApuNoiseChannel() override;

            void SetChannelSettings(uint8_t register1, bool register1flag,
                                    uint8_t register2, bool register2flag,
                                    uint8_t register3, bool register3flag,
                                    uint8_t register4, bool register4flag);
        protected:
            float *GenerateBufferData(int frequency) override;
            int frequency = 0;
        };

    }
}
#endif


