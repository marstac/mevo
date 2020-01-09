#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"
#include "../../../utils/functions.h"
void OmniOptimizer::mutation_pop (OmniOptimizer::population *pop)
{

    MEVO::CPopulation epop;
    for(int i = 0;i < popsize;i++)
    {
       epop.addIndividual(pop->ind[i]);

    }
    mutatePopulation(&epop);
    return;
}

void OmniOptimizer::mutation_ind (OmniOptimizer::OmniIndividual *ind)
{

}

void OmniOptimizer::real_mutate_ind (OmniOptimizer::OmniIndividual *ind)
{

    return;
}
