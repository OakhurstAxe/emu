#ifndef _OA_EMU_SYSTEMDATA_H
#define _OA_EMU_SYSTEMDATA_H

#include "oaemuscreentypes.h"
#include "oaemuvcssystem.h"

namespace oa
{
    namespace emu
    {
        
        class SystemData
        {
        public:
            SystemData();
            ~SystemData();
            void LoadFile(QString fileName);
            void SaveFile(QString fileName);
            ScreenTypes* GetScreenTypes();
            VcsSystem*  GetVcsSystem();
        private:
            ScreenTypes *screenTypes_;
            VcsSystem *vcsSystem_;
        };
    
    }
}

#endif


