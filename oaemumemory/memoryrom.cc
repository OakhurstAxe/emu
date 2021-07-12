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
            throw std::out_of_range(QString("Cannot write to RAM: address [%1]=%2 in memory module %3").arg(location).arg(byte).arg(name_).toLocal8Bit().data());
        }

    }
}
