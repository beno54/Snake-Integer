#ifndef NUMGENERATOR_H
#define NUMGENERATOR_H


using namespace std;

class NumGenerator
{
    public:
        NumGenerator();
        NumGenerator(float seed1, float seed2, float seed3);
        virtual ~NumGenerator();
        int nexNum();
        void copy_NumGenerator(NumGenerator* numG2Copy);
        float get_seed1();
        float get_seed2();
        float get_seed3();
    protected:

    private:
        float seed1,seed2,seed3;
        float nextSeed(int minNum,int maxNum,float *seed);
};

#endif // NUMGENERATOR_H
