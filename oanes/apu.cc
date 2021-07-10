
#include "headers/apu.h"

namespace oa
{
    namespace nes
    {
        
        Apu::Apu()
        {
            channels[0] = new EmuApuChannel();
            channels[1] = new EmuApuChannel();    
        }

        Apu::~Apu()
        {
            delete channels[0];
            delete channels[1];
        }

        void Apu::ExecuteTick()
        {
            //return;
            for (int i=0; i<2; i++)
            {
                int timer = 0;
                qreal volume = 0;
                
                uint8_t byte = memory->CpuRead(0x4000 + (i*4));
                volume = ((qreal)(byte & 0x0f)) / 16;
                channels[i]->setVolume(volume);
                
                byte = memory->CpuRead(0x4002 + (i*4));
                timer |= byte;
                byte = (memory->CpuRead(0x4003 + (i * 4)) & 0x07);
                timer |= byte << 8;
                if (timer < 8)
                {
                    channels[i]->stop();
                    return;
                }
                int newFrequency = 1789773/(16 * (timer + 1));
                if (newFrequency < 100)
                {
                    channels[i]->stop();
                    return;
                }
                if (frequency[i] != newFrequency)
                {
                    channels[i]->stop();
                    frequency[i] = newFrequency;
                    channels[i]->playSound(25000, frequency[i]);
                }
            }
        }

    }
}
    
