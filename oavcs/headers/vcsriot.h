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
            void ClearTIMnnTUnderflow();
            void ClearInstatUnderflow();
            uint16_t step_;
            uint16_t stepCount_;
            bool overflowTick_;
            
            QGamepad m_gamepad_;
        };
        
    }
}

#endif
