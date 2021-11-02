#include <QDebug>

#include "headers/nesapu.h"
#include "headers/nesapupulsechannel.h"
#include "headers/nesaputrianglechannel.h"
#include "headers/nesapunoisechannel.h"

namespace oa
{
    namespace nes
    {
        
        NesApu::NesApu(NesMemory *memory)
        {
            memory_ = memory;
            channels[0] = new NesApuPulseChannel();
            channels[1] = new NesApuPulseChannel();    
            channels[2] = new NesApuTriangleChannel();    
            channels[3] = new NesApuNoiseChannel();    
            
        }

        NesApu::~NesApu()
        {
            delete channels[0];
            delete channels[1];
            delete channels[2];
            delete channels[3];
        }

        void NesApu::ExecuteTick()
        {
            for (int i=0; i<4; i++)
            {
                uint8_t register1 = memory_->CpuRead(0x4000 + (i*4));
                bool register1flag = memory_->CpuWriteFlagged(0x4000 + (i*4));
                uint8_t register2 = memory_->CpuRead(0x4001 + (i*4));
                bool register2flag = memory_->CpuWriteFlagged(0x4001 + (i*4));
                uint8_t register3 = memory_->CpuRead(0x4002 + (i*4));
                bool register3flag = memory_->CpuWriteFlagged(0x4002 + (i*4));
                uint8_t register4 = memory_->CpuRead(0x4003 + (i*4));
                bool register4flag = memory_->CpuWriteFlagged(0x4003 + (i*4));

                channels[i]->SetChannelSettings(register1, register1flag,
                                                register2, register2flag,
                                                register3, register3flag,
                                                register4, register4flag);
            }
        }

    }
}
    
