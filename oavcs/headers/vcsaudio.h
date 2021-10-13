#ifndef _OA_VCS_AUDIO_H
#define _OA_VCS_AUDIO_H

#include "vcsaudiochannel.h"
#include "vcsmemory.h"

namespace oa
{
    namespace vcs
    {
        
        class VcsAudio
        {
        public:
            VcsAudio(VcsMemory *memory);
            virtual ~VcsAudio();
            void ExecuteTick();
        private:
            VcsMemory *memory_;
            VcsAudioChannel *channels_[2];
        };

    }
}

#endif
