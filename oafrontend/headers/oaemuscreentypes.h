#ifndef _OA_EMU_SCREENTYPES_H
#define _OA_EMU_SCREENTYPES_H

#include <QList>

namespace oa
{
    namespace emu
    {
        
        class ScreenTypes
        {
        public:
            QList<QString> GetScreenTypes();
            void AddScreenType(QString screenType);
        private:
            QList<QString> screenTypes_;
        };
        
    }
}

#endif

