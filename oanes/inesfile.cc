
#include "headers/inesfile.h"

#include <iostream>
#include <fstream>
#include <ostream>

namespace oa
{
    namespace nes
    {
        void INesFile::LoadFile(QString fileName)
        {
            std::ifstream inFile<unsigned char>;
            inFile.open(fileName.toLocal8Bit(), std::ios::in | std::ios::binary | std::ios::ate);
            
            if (inFile.is_open() == false)
            {
                throw std::out_of_range("Can't find " + fileName.toLocal8Bit() + " file");
            }
            
            inFile.seekg(0, std::ios::beg);
            inFile.read(header_, 4);
            inFile >> programRomSize_;
            inFile >> characterRomSize_;
            inFile >> cartridgeFlags_.reg;
            inFile >> playChoiceFlags_.reg;
            inFile >> programRamSize_;
            inFile >> unusedFlags_.reg;
            inFile >> tvSystemFlags_.reg;
            
            if (cartridgeFlags_.hasTrainer)
            {
                inFile.read(trainer_, 512);
            }
            
            progRomData_ = new unsigned char[0x4000 * programRomSize_];
            inFile.read(progRomData_, 0x4000 * programRomSize_);
            charRomData_ = new unsigned char[0x2000 * characterRomSize_];
            inFile.read(charRomData_, 0x2000 * characterRomSize_);
            
            // Skip play choice for now
            
            inFile.close();
        }
    }    
}
