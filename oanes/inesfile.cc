
#include "headers/inesfile.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <sys/stat.h>

namespace oa
{
    namespace nes
    {
        unsigned char *INesFile::GetProgRomData()
        {
            return progRomData_;
        }
        
        uint8_t INesFile::GetProgRomSize()
        {
            return programRomSize_;
        }
        
        unsigned char *INesFile::GetCharRomData()
        {
            return charRomData_;
        }
            
        uint8_t INesFile::GetCharRomSize()
        {
            return characterRomSize_;
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
            memcpy(&programRomSize_, &fileData[position], 1);
            position += 1;
            memcpy(&characterRomSize_, &fileData[position], 1);
            position += 1;
            memcpy(&cartridgeFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&playChoiceFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&programRamSize_, &fileData[position], 1);
            position += 1;
            memcpy(&unusedFlags_.reg, &fileData[position], 1);
            position += 1;
            memcpy(&tvSystemFlags_.reg, &fileData[position], 1);
            position += 1;
            position += 5;
            
            if (cartridgeFlags_.hasTrainer)
            {
                memcpy(&trainer_, &fileData[position], 512);
                position += 512;
            }
            
            programRomSize_ = 0x4000 * programRomSize_;
            progRomData_ = new uint8_t[programRomSize_];
            memcpy(progRomData_, &fileData[position], programRomSize_);
            position += programRomSize_;
            characterRomSize_ = 0x2000 * characterRomSize_;
            charRomData_ = new uint8_t[characterRomSize_];
            memcpy(charRomData_, &fileData[position], characterRomSize_);
            
            delete []fileData;
            // Skip play choice for now
        }
    }    
}
