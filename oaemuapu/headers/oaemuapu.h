#ifndef _EMUAPU_H
#define _EMUAPU_H

#include "../oaemuapu/headers/oaemuapuchannel.h"
#include "../oaemumemory/headers/nesmemory.h"

class EmuApu
{
public:
    EmuApu(QObject *parent);
    oa::emu::NesMemory *memory;
    void executeTicks();
    QObject *parent;
private:
    int frequency[2];
    EmuApuChannel *channels[2];
};

#endif
