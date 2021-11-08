
#include "headers/oaemuvcsgame.h"

namespace oa
{
    namespace emu
    {

        QString VcsGame::GetName() const
        {
            return name_;
        }
        void VcsGame::SetName(QString name)
        {
            name_ = name;
        }
        QString VcsGame::GetController()
        {
            return controller_;
        }
        void VcsGame::SetMapper(QString mapper)
        {
            mapper_ = mapper;
        }
        QString VcsGame::GetMapper()
        {
            return mapper_;
        }
        void VcsGame::SetHasSuperChip(bool hasSuperChip)
        {
            hasSuperChip_ = hasSuperChip;
        }
        bool VcsGame::GetHasSuperChip()
        {
            return hasSuperChip_;
        }
        void VcsGame::SetController(QString controller)
        {
            controller_ = controller;
        }
        QString VcsGame::GetCompany()
        {
            return company_;
        }
        void VcsGame::SetCompany(QString company)
        {
            company_ = company;
        }
        QString VcsGame::GetScreenType()
        {
            return screenType_;
        }
        void VcsGame::SetScreenType(QString screenType)
        {
            screenType_ = screenType;
        }
        QString VcsGame::GetRomFile()
        {
            return romFile_;
        }
        void VcsGame::SetRomFile(QString romFile)
        {
            romFile_ = romFile;
        }
        
        bool VcsGame::operator<(const VcsGame& other) const
        {
           bool result = GetName() < other.GetName();
           return result;
        }        
    }
}
