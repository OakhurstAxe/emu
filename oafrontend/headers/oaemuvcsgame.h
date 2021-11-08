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
            QString GetName() const;
            void SetName(QString name);
            QString GetMapper();
            void SetMapper(QString mapper);
            bool GetHasSuperChip();
            void SetHasSuperChip(bool hasSuperChip);
            QString GetController();
            void SetController(QString controller);
            QString GetCompany();
            void SetCompany(QString company);
            QString GetScreenType();
            void SetScreenType(QString screenType);
            QString GetRomFile();
            void SetRomFile(QString romFile);
            
            bool operator<(const VcsGame& other) const;
        private:
            QString name_;
            QString mapper_;
            bool hasSuperChip_;
            QString controller_;
            QString company_;
            QString screenType_;
            QString romFile_;
        };
        
    }
}

#endif
