#ifndef _OA_VCS_RIOT_H
#define _OA_VCS_RIOT_H

#include <QObject>
#include <QtGamepad/QGamepad>

#include "oaemumemory/headers/memoryram.h"

namespace oa
{
    namespace vcs
    {
 
        class VcsRiot : public QObject, public emu::MemoryRam
        {
            Q_OBJECT
            
        public:
            VcsRiot();
            void Reset();
            void ExecuteTick();
            virtual uint8_t Read(uint16_t location) override;
            virtual void Write(uint16_t location, uint8_t byte) override;
        public slots:            
            void LeftControllerReset(bool value);
            void LeftControllerSelect(bool value);
            void LeftControllerUpDown(double value);
            void LeftControllerLeftRight(double value);            
        private:
            uint16_t step_;
            uint16_t prevStep_;
            uint16_t stepCount_;
            
            QGamepad m_gamepad_;
            uint8_t regInpt0_;
            uint8_t regInpt1_;
            uint8_t regInpt2_;
            uint8_t regInpt3_;
            uint8_t regInpt4_;
            uint8_t regInpt5_;
            uint8_t regSwcha_;
            uint8_t regSwchb_;
            uint8_t regSwcnt_;            
        };
        
    }
}

#endif
