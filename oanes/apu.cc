#include <QDebug>

#include "headers/apu.h"
#include "headers/nesapupulsechannel.h"
#include "headers/nesaputrianglechannel.h"
#include "headers/nesapunoisechannel.h"

namespace oa
{
    namespace nes
    {
        
        Apu::Apu(NesMemory *memory)
        {
            memory_ = memory;
            channels[0] = new NesApuPulseChannel();
            channels[1] = new NesApuPulseChannel();    
            channels[2] = new NesApuTriangleChannel();    
            //channels[3] = new NesApuNoiseChannel();    
            
        }

        Apu::~Apu()
        {
            delete channels[0];
            delete channels[1];
            delete channels[2];
            //delete channels[3];
        }

        void Apu::ExecuteTick()
        {
            for (int i=0; i<3; i++)
            {
                uint8_t byte = memory_->CpuRead(0x4000 + (i*4));
                uint8_t byte2 = memory_->CpuRead(0x4001 + (i*4));
                uint8_t byte3 = memory_->CpuRead(0x4002 + (i*4));
                uint8_t byte4 = memory_->CpuRead(0x4003 + (i*4));

                channels[i]->PlaySound(byte, byte2, byte3, byte4);
            }
        }

    }
}
    
