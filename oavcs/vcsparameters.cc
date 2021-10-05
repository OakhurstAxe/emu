#include "headers/vcsparameters.h"

#include <string.h>
#include <stdlib.h>

namespace oa
{
    namespace vcs
    {
        VcsParameters::VcsParameters()
        {
            consoleType_ = ConsoleType::NTSC;
            cartData_ = NULL;
            cartSize_ = 0;
        }
        
        VcsParameters::~VcsParameters()
        {
            if (cartData_ != NULL)
            {
                free(cartData_);
            }
        }
        
        ConsoleType VcsParameters::GetConsoleType()
        {
            return consoleType_;
        }
        
        void VcsParameters::SetConsoleType(ConsoleType consoleType)
        {
            consoleType_ = consoleType;
        }
        
        uint8_t* VcsParameters::GetCartData()
        {
            return cartData_;
        }
        
        void VcsParameters::SetCartData(uint8_t* cartData, uint32_t size)
        {
            if (cartData_ == NULL)
            {
                cartData_ = (uint8_t*)malloc(size);
            }
            else
            {
                //cartData_ = (uint8_t*)realloc(cartData_, size);
            }
            memcpy(cartData_, cartData, size);
            cartSize_ = size;
        }
        
        uint32_t VcsParameters::GetCartSize()
        {
            return cartSize_;
        }
        
        void VcsParameters::SetMapper(QString mapper)
        {
            mapper_ = mapper;
        }
        
        QString VcsParameters::GetMapper()
        {
            return mapper_;
        }
        
    }
}
