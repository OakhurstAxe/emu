
#include "headers/nescartridge.h"
#include "headers/nescartridge000.h"
#include "headers/nescartridge001.h"

namespace oa
{
    namespace nes
    {
        NesCartridge* NesCartridge::GetCartridge(uint16_t mapper)
        {
            switch (mapper)
            {
                case (0):
                {
                    return new NesCartridge000();
                    break;
                }
                case (1):
                {
                    return new NesCartridge001();
                    break;
                }
            }
            throw std::out_of_range(QString("Invalid NES memory mapper %1").arg(mapper).toLocal8Bit().data());
        }

    }
}
