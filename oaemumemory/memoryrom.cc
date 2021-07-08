#include "headers/memoryrom.h"

#include <QString>

namespace oa
{
    namespace emu
    {
        
        MemoryRom::MemoryRom(uint16_t size, QString name) : BaseMemory (size, name)
        {
        }

        void MemoryRom::Write(uint16_t location, uint8_t byte)
        {
            throw std::out_of_range(QString("Cannot write to RAM: address [%1] in memory module %2").arg(location).toLocal8Bit().data());
        }

    }
}
