#ifndef _OA_EMU_VCSSYSTEM_H
#define _OA_EMU_VCSSYSTEM_H

#include <QList>

#include "oaemuvcsgame.h"

namespace oa
{
    namespace emu
    {
        
        class VcsSystem
        {
        public:
            VcsSystem();
            ~VcsSystem();
            QString GetRomZipFile();
            void SetRomZipFile(QString fileName);
            QList<VcsGame*> GetVcsGames();
            void AppendVcsGame(VcsGame* vcsGame);
            QList<QString> GetMappers();
            void AppendMapper(QString mapper);
            QList<QString> GetCompanies();
            void AppendCompany(QString company);
            QList<QString> GetControllerTypes();
            void AppendControllerType(QString controllerType);
        private:
            QString romZipFile_;
            QList<VcsGame*> vcsGames_;
            QList<QString> mappers_;
            QList<QString> companies_;
            QList<QString> controllerTypes_;            
        };
        
    }
}

#endif
