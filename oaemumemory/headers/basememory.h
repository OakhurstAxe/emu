#ifndef _OA_EMU_BASEMEMORY_H
#define _OA_EMU_BASEMEMORY_H

#include <stdint.h>
#include <QString>

namespace oa
{
    namespace emu
    {
    
        class BaseMemory
        {
        public:
            BaseMemory(uint16_t size, QString name);
            virtual ~BaseMemory();
            void LoadData(uint8_t* data, uint16_t size);
            virtual uint8_t Read(uint16_t location);
            virtual void Write(uint16_t location, uint8_t byte);
        protected:
            void CheckLocation(uint16_t location);
            uint16_t memorySize_ = 0;
            uint8_t* memory_;
            QString name_;
        };
        
    }
}
#endif
