#include "NumGenerator.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>

NumGenerator::NumGenerator()
{
    srand (time(NULL));
    seed1 = 1 + rand() % 9999999 ;
    seed2 = 1 + rand() % 9999999 ;
    seed3 = 1 + rand() % 9999999 ;
}

NumGenerator::~NumGenerator()
{
    //dtor
}

float NumGenerator::nextSeed(int minNum,int maxNum,float *seed)
{
    *seed = int(*seed*125)%2796203;
	return int(*seed)%(maxNum-minNum+1)+minNum;
}

int NumGenerator::nexNum()
{
    float  which, nextnb;
    which = nextSeed(0,1,&seed1);
	if(int(which)%2==0)
		nextnb=nextSeed(1,3,&seed2);
	else
		nextnb=nextSeed(1,3,&seed3);
	return nextnb;
}
