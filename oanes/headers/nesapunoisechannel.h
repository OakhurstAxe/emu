#ifndef _OA_NES_APUNOISECHANNEL_H
#define _OA_NES_APUNOISECHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuNoiseChannel : public NesApuChannel
        {
            Q_OBJECT

        public:
            NesApuNoiseChannel();
            virtual ~NesApuNoiseChannel();

            void PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) override;
        protected:
            void GenerateBufferData(const QAudioFormat &format, int frequency);
            int frequency = 0;
        };

    }
}
#endif


