
#include "headers/vcsfile.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <sys/stat.h>

namespace oa
{
    namespace vcs
    {
        uint8_t *VcsFile::GetProgRomData()
        {
            return progRomData_;
        }
        
        uint8_t VcsFile::GetProgRomSize()
        {
            return progRomSize_;
        }
        
        void VcsFile::LoadFile(QString fileName)
        {            
            std::ifstream inFile;
            inFile.open(fileName.toLocal8Bit(), std::ios::in | std::ios::binary | std::ios::ate);
            
            if (inFile.is_open() == false)
            {
                throw std::out_of_range("Can't find " + fileName.toLocal8Bit() + " file");
            }
            inFile.seekg(0, std::ios::end);
            int fileSize = inFile.tellg();
            if (fileSize > 0x1000)
            {
                progRomSize_ = 2;
            }
            else
            {
                progRomSize_ = 1;
            }
            char *fileData = new char[fileSize];

            inFile.seekg(0);
            if (!inFile.read(fileData, fileSize))
            {
                // error
                throw std::runtime_error("Error reading VCS file");
            }
            inFile.close();            

            memcpy(&progRomData_, &fileData[0], fileSize);
            
            delete []fileData;
        }
    }    
}

