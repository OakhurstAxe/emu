#ifndef _OA_NES_APUPULSECHANNEL_H
#define _OA_NES_APUPULSECHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuPulseChannel : public NesApuChannel
        {
            Q_OBJECT

        public:
            NesApuPulseChannel();
            virtual ~NesApuPulseChannel();

            void PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) override;
        protected:
            void GenerateBufferData(const QAudioFormat &format, int frequency, int duty);
            int frequency = 0;
        };

    }
}
#endif

