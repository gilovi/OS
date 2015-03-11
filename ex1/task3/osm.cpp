#include "osm.h"
#include <sys/time.h>
#include<cmath>

#define DEFAULT_ITERATIONS 50000
#define ROOLING_FACTOR 10

double osm_operation_time(unsigned int osm_iterations)
{
    unsigned int iterations = osm_iterations > 0  ? osm_iterations : DEFAULT_ITERATIONS;
    iterations = std::ceil(iterations/ROOLING_FACTOR);
    timeval t1, t2;
    double runningTime = 0;
    int a = 0;

    gettimeofday(&t1, NULL);
    for(unsigned int i = 0; i < iterations; i++)
    {
        a=0;
        a=1;
        a=2;
        a=3;
        a=4;
        a=5;
        a=6;
        a=7;
        a=8;
        a=9;
    }
    gettimeofday(&t2, NULL);
    runningTime = (t2.tv_usec - t1.tv_usec);
    double avgRunningTime = (runningTime/(iterations*ROOLING_FACTOR) )/1000;
    return avgRunningTime;
}
