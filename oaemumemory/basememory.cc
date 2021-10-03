
#include "headers/basememory.h"

namespace oa
{
    namespace emu
    {

        BaseMemory::BaseMemory(uint16_t size, QString name)
        {
            name_ = name;
            memorySize_ = size;
            memory_ = (uint8_t*)malloc(memorySize_);
        }

        BaseMemory::~BaseMemory()
        {
            free(memory_);
        }
        
        void BaseMemory::LoadData(uint8_t* data, uint16_t size)
        {
            memcpy(memory_, data, size);
        }
        
        uint8_t BaseMemory::Read(uint16_t location)
        {
            CheckLocation(location);
            return memory_[location];
        }

        void BaseMemory::Write(uint16_t location, uint8_t byte)
        {
            CheckLocation(location);
            memory_[location] = byte;
        }
        
        void BaseMemory::CheckLocation(uint16_t location)
        {
            if (location > memorySize_)
            {
                throw std::out_of_range(QString("Bad location address [%1] in memory module %2").arg(location).arg(name_).toLocal8Bit().data());
            }
        }

    }
}
