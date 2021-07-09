#ifndef BASECPU_H
#define BASECPU_H

namespace oa
{
    namespace emu
    {
        
        class BaseCpu
        {
        public:
            virtual int executeTicks(int count) = 0;
        };

    }
}

#endif // BASECPU_H
