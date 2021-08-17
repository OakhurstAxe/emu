
#include "headers/inesfile.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <sys/stat.h>

namespace oa
{
    namespace nes
    {
        uint8_t *INesFile::GetProgRomData()
        {
            return progRomData_;
        }
        
        uint8_t INesFile::GetProgRomSize()
        {
            return progRomSize_;
        }
        
        uint8_t *INesFile::GetCharRomData()
        {
            return charRomData_;
        }
            
        uint8_t INesFile::GetCharRomSize()
        {
            return charRomSize_;
        }
        
        uint16_t INesFile::GetMemoryMapper()
        {
            return memoryMapper_;
        }

        void INesFile::LoadFile(QString fileName)
        {            
            std::ifstream inFile;
            inFile.open(fileName.toLocal8Bit(), std::ios::in | std::ios::binary | std::ios::ate);
            
            if (inFile.is_open() == false)
            {
                throw std::out_of_range("Can't find " + fileName.toLocal8Bit() + " file");
            }
            inFile.seekg(0, std::ios::end);
            int fileSize = inFile.tellg();
            char *fileData = new char[fileSize];

            inFile.seekg(0);
            if (!inFile.read(fileData, fileSize))
            {
                // error
                throw std::runtime_error("Error reading iNES file: ");
            }
            inFile.close();            

            int position = 0;
            memcpy(header_, &fileData[position], 4);
            position += 4;
            memcpy(&progRomSizeLsb_, &fileData[position], 1);
            position += 1;
            memcpy(&charRomSizeLsb_, &fileData[position], 1);
            position += 1;
            memcpy(&cartridgeFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&consoleTypeFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&mapperFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&romSizeFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&ramEpromSizeFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&charRomSizeFlags_.reg, &fileData[position], 1);
            position += 5;
            
            memoryMapper_ = (mapperFlags_.mapperMsb_ << 8) +
                (consoleTypeFlags_.MapperCsb_ << 4) + 
                cartridgeFlags_.mapperLsb_;
            progRomSize_ = (romSizeFlags_.prgRomSizeMsb_ << 8) + progRomSizeLsb_;
            charRomSize_ = (romSizeFlags_.cahrRomSizeMsb_ << 8) + charRomSizeLsb_;
            if (charRomSize_ == 0)
            {
                charRomSize_ = 1;
            }

            if (cartridgeFlags_.hasTrainer_)
            {
                memcpy(&trainer_, &fileData[position], 512);
                position += 512;
            }
            progRomData_ = new uint8_t[0x4000 * progRomSize_];
            memcpy(progRomData_, &fileData[position], 0x4000 * progRomSize_);
            position += 0x4000 * charRomSize_;
            charRomData_ = new uint8_t[0x2000 * charRomSize_];
            memcpy(charRomData_, &fileData[position], 0x2000 * charRomSize_);
            
            delete []fileData;
            // Skip play choice for now
        }
    }    
}
