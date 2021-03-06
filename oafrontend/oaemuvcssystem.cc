
#include "headers/oaemuvcssystem.h"

namespace oa
{
    namespace emu
    {
        VcsSystem::VcsSystem()
        {
        }
        
        VcsSystem::~VcsSystem()
        {
            vcsGames_.clear();
        }    
        
        QString VcsSystem::GetRomZipFile()
        {
            return romZipFile_;
        }
        void VcsSystem::SetRomZipFile(QString fileName)
        {
            romZipFile_ = fileName;
        }

        QList<VcsGame*> VcsSystem::VcsSystem::GetVcsGames()
        {
            return vcsGames_;
        }
        void VcsSystem::AppendVcsGame(VcsGame* vcsGame)
        {
            vcsGames_.append(vcsGame);
        }

        QList<QString> VcsSystem::GetMappers()
        {
            return mappers_;
        }
        void VcsSystem::AppendMapper(QString mapper)
        {
            mappers_.append(mapper);
        }
        
        bool GameSortFunc (VcsGame *i,VcsGame *j) 
        { 
            return (i->GetName()<j->GetName());             
        }

        void VcsSystem::SortVcsGames()
        {
            std::sort(vcsGames_.begin(), vcsGames_.end(), GameSortFunc);
        }

        QList<QString> VcsSystem::GetCompanies()
        {
            return companies_;
        }
        void VcsSystem::AppendCompany(QString company)
        {
            companies_.append(company);
        }
        
        QList<QString> VcsSystem::GetControllerTypes()
        {
            return controllerTypes_;
        }
        void VcsSystem::AppendControllerType(QString controllerType)
        {
            controllerTypes_.append(controllerType);
        }
        
    }
}

