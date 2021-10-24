#ifndef _OA_VCS_AUDIO_H
#define _OA_VCS_AUDIO_H

#include "vcsaudiochannel.h"
#include "vcsmemory.h"
#include "vcstia.h"

namespace oa
{
    namespace vcs
    {
        class VcsTia;
        
        class VcsAudio
        {
        public:
            VcsAudio(VcsTia *vcsTia);
            virtual ~VcsAudio();
            void ExecuteTick();
        private:
            VcsTia *vcsTia_;
            VcsAudioChannel *channels_[2];
        };

    }
}

#endif
