#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"


void OmniOptimizer::define_epsilon (OmniOptimizer::population *pop, int size, double *eps)
{
    MEVO::CPopulation population;
    for (int i=0; i< size; i++) {
        population.addIndividual(pop->ind[i]);
    }
    FEpsilon = population.calcEpsi(FEps);


    return;
}
