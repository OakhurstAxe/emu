#ifndef OA_EMU_BASECPU_H
#define OA_EMU_BASECPU_H

namespace oa
{
    namespace emu
    {
        
        class BaseCpu
        {
        public:
            virtual void ExecuteTick() = 0;
            virtual void Reset();
        protected:
            int overflowTicks_ = 0;
        };

    }
}

#endif // BASECPU_H
