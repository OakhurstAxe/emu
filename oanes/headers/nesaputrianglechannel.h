#ifndef _OA_NES_APUTRIANGLECHANNEL_H
#define _OA_NES_APUTRIANGLECHANNEL_H

#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "headers/nesapuchannel.h"

namespace oa
{
    namespace nes
    {
        
        class NesApuTriangleChannel : public NesApuChannel
        {
            Q_OBJECT

        public:
            NesApuTriangleChannel();
            virtual ~NesApuTriangleChannel();

            void PlaySound(uint8_t register1, uint8_t register2, uint8_t register3, uint8_t register4) override;
        protected:
            void GenerateBufferData(const QAudioFormat &format, int frequency);
            int frequency = 0;
        };

    }
}
#endif

