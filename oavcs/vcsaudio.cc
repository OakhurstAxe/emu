
#include "headers/vcsaudio.h"

#include <QDebug>

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
        
        VcsAudio::VcsAudio(VcsTia *vcsTia)
        {
            vcsTia_ = vcsTia;
            
            PaError err = Pa_Initialize();
            if (err != paNoError)
            {
                qDebug() << "Error starting port audio";
            }
            channels_[0] = new VcsAudioChannel();
            channels_[1] = new VcsAudioChannel();
        }

        VcsAudio::~VcsAudio()
        {
            delete channels_[0];
            delete channels_[1];

            PaError err = Pa_Terminate();
            if (err != paNoError)
            {
                qDebug() << "Error terminating port audio";
            }
        }

        void VcsAudio::ExecuteTick()
        {
            uint8_t register1 = vcsTia_->GetAudioV0();
            uint8_t register2 = vcsTia_->GetAudioF0();
            uint8_t register3 = vcsTia_->GetAudioC0();
            channels_[0]->SetChannelSettings(register1, register2, register3);

            register1 = vcsTia_->GetAudioV1();
            register2 = vcsTia_->GetAudioF1();
            register3 = vcsTia_->GetAudioC1();
            channels_[1]->SetChannelSettings(register1, register2, register3);            
        }

    }
}
    
