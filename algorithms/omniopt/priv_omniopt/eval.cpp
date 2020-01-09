#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

void OmniOptimizer::evaluate_pop (OmniOptimizer::population *pop)
{
    MEVO::CPopulation epop;
    for (int i=0; i<popsize; i++)
    {
        epop.addIndividual(pop->ind[i]);
    }
    evaluatePopulation(&epop);
    return;
}




