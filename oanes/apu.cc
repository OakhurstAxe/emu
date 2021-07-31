
#include "headers/apu.h"

namespace oa
{
    namespace nes
    {
        
        Apu::Apu(NesMemory *memory)
        {
            memory_ = memory;
            channels[0] = new NesApuChannel();
            channels[1] = new NesApuChannel();    
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
                
                uint8_t byte = memory_->CpuRead(0x4000 + (i*4));
                volume = ((qreal)(byte & 0x0f)) / 16;
                channels[i]->setVolume(volume);
                
                byte = memory_->CpuRead(0x4002 + (i*4));
                timer += byte;
                byte = (memory_->CpuRead(0x4003 + (i * 4)) & 0x07);
                timer += (byte << 8);
                if (timer < 8)
                {
                    return;
                }
                int newFrequency = 1789773/(16 * (timer + 1));
                if (newFrequency < 100)
                {
                    return;
                }
                channels[i]->playSound(newFrequency);
            }
        }

    }
}
    
