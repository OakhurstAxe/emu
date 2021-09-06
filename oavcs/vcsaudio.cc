#include <QDebug>

#include "headers/vcsaudio.h"
#include "headers/vcsaudiochannel.h"

#define REG_AUDV0   0x19
#define REG_AUDV1   0x1A
#define REG_AUDF0   0x17
#define REG_AUDF1   0x18
#define REG_AUDC0   0x15
#define REG_AUDC1   0x16

namespace oa
{
    namespace vcs
    {
        
        VcsAudio::VcsAudio(VcsMemory *memory)
        {
            memory_ = memory;
            channels[0] = new VcsAudioChannel();
            channels[1] = new VcsAudioChannel();    
        }

        VcsAudio::~VcsAudio()
        {
            delete channels[0];
            delete channels[1];
        }

        void VcsAudio::ExecuteTick()
        {
            for (int i=0; i<2; i++)
            {
                uint8_t register1 = memory_->CpuRead(REG_AUDV0 + i);
                uint8_t register2 = memory_->CpuRead(REG_AUDF0 + i);
                uint8_t register3 = memory_->CpuRead(REG_AUDC0 + i);

                channels[i]->SetChannelSettings(register1, register2, register3);
            }
        }

    }
}
    
