#ifndef _OA_EMU_VCSGAME
#define _OA_EMU_VCSGAME

#include <QString>

namespace oa
{
    namespace emu
    {
        
        class VcsGame
        {
        public:
            QString GetName();
            void SetName(QString name);
            QString GetController();
            void SetController(QString controller);
            QString GetCompany();
            void SetCompany(QString company);
            QString GetScreenType();
            void SetScreenType(QString screenType);
            QString GetRomFile();
            void SetRomFile(QString romFile);
        private:
            QString name_;
            QString controller_;
            QString company_;
            QString screenType_;
            QString romFile_;
        };
        
    }
}

#endif
