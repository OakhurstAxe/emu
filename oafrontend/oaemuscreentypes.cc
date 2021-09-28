
#include "headers/oaemuscreentypes.h"

namespace oa
{
    namespace emu
    {
        
        QList<QString> ScreenTypes::GetScreenTypes()
        {
            return screenTypes_;
        }
        
        void ScreenTypes::AddScreenType(QString screenType)
        {
            screenTypes_.append(screenType);
        }
        
    }
}
