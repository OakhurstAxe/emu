#ifndef _EMUAPU_H
#define _EMUAPU_H

#include "../emuapu/headers/emuapuchannel.h"
#include "../emumemory/headers/nesmemory.h"

class EmuApu
{
public:
    EmuApu(QObject *parent);
    NesMemory *memory;
    void executeTicks();
    QObject *parent;
private:
    int frequency[2];
    EmuApuChannel *channels[2];
};

#endif
