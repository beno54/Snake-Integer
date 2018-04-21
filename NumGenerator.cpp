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
//    seed1 = 22495;
//    seed2 = 23073;
//    seed3 = 10585;
//
//    cout << "Seed 1 : " << seed1 << endl;
//    cout << "Seed 2 : " << seed2 << endl;
//    cout << "Seed 3 : " << seed3 << endl;
}

NumGenerator::NumGenerator(float seed1, float seed2, float seed3)
{
    srand (time(NULL));
//    seed1 = 1 + rand() % 9999999 ;
//    seed2 = 1 + rand() % 9999999 ;
//    seed3 = 1 + rand() % 9999999 ;
    if (seed1 == 0.0f)seed1 ++;
    if (seed2 == 0.0f)seed2 ++;
    if (seed3 == 0.0f)seed3 ++;

    this->seed1 = seed1;
    this->seed2 = seed2;
    this->seed3 = seed3;

//    cout << "Seed 1 : " << seed1 << endl;
//    cout << "Seed 2 : " << seed2 << endl;
//    cout << "Seed 3 : " << seed3 << endl;
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

void NumGenerator::copy_NumGenerator(NumGenerator* numG2Copy)
{
    this->seed1 = numG2Copy->get_seed1();
    this->seed2 = numG2Copy->get_seed2();
    this->seed3 = numG2Copy->get_seed3();

//    cout << "Copy Seed 1 : " << seed1 << endl;
//    cout << "Copy Seed 2 : " << seed2 << endl;
//    cout << "Copy Seed 3 : " << seed3 << endl;
}

float NumGenerator::get_seed1()
{
    return seed1;
}

float NumGenerator::get_seed2()
{
    return seed2;
}
float NumGenerator::get_seed3()
{
    return seed3;
}
