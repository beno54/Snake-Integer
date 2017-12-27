#ifndef NUMGENERATOR_H
#define NUMGENERATOR_H


class NumGenerator
{
    public:
        NumGenerator();
        virtual ~NumGenerator();
        int nexNum();
    protected:

    private:
        float seed1,seed2,seed3;
        float nextSeed(int minNum,int maxNum,float *seed);
};

#endif // NUMGENERATOR_H
