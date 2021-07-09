#ifndef _OA_NES_EMUAPU_H
#define _OA_NES_EMUAPU_H

#include "nesapuchannel.h"
#include "nesmemory.h"

namespace oa
{
    namespace nes
    {
        
        class EmuApu
        {
        public:
            EmuApu(QObject *parent);
            virtual ~EmuApu();
            oa::nes::NesMemory *memory;
            void executeTicks();
            QObject *parent;
        private:
            int frequency[2];
            EmuApuChannel *channels[2];
        };

    }
}

#endif
