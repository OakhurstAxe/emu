
#include "headers/memoryramflagged.h"

namespace oa
{
    namespace emu
    {
        
        MemoryRamFlagged::MemoryRamFlagged(uint16_t size, QString name) : MemoryRam (size, name)
        {
            isReadFlagged_ = new bool[memorySize_];
            isWriteFlagged_ = new bool[memorySize_];
            
            memset(isReadFlagged_, 0, memorySize_);
            memset(isWriteFlagged_, 0, memorySize_);
        }

        uint8_t MemoryRamFlagged::Read(uint16_t location)
        {
            CheckLocation(location);
            isReadFlagged_[location] = true;
            return MemoryRam::Read(location);
        }
        
        void MemoryRamFlagged::Write(uint16_t location, uint8_t byte)
        {
            CheckLocation(location);
            isWriteFlagged_[location] = true;
            MemoryRam::Write(location, byte);
        }

        bool MemoryRamFlagged::IsReadFlagSet(uint16_t location)
        {
            CheckLocation(location);
            bool result = isReadFlagged_[location];
            isReadFlagged_[location] = false;
            return result;
        }
        
        bool MemoryRamFlagged::IsWriteFlagSet(uint16_t location)
        {
            CheckLocation(location);
            bool result = isWriteFlagged_[location];
            isWriteFlagged_[location] = false;
            return result;
        }
    }
}
