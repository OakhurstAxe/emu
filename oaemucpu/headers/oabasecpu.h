#ifndef BASECPU_H
#define BASECPU_H


class BaseCpu
{
public:
    virtual int executeTicks(int count) = 0;
};

#endif // BASECPU_H
