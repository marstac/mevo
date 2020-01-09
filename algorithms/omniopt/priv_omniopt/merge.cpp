#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

void OmniOptimizer::merge(OmniOptimizer::population *pop1, OmniOptimizer::population *pop2, OmniOptimizer::population *pop3)
{
    int i, k;
    for (i=0; i<popsize; i++)
    {
        copy_ind ((pop1->ind[i]), (pop3->ind[i]));
    }
    for (i=0, k=popsize; i<popsize; i++, k++)
    {
        copy_ind ((pop2->ind[i]), (pop3->ind[k]));
    }
    return;
}

void OmniOptimizer::copy_ind (OmniOptimizer::OmniIndividual *ind1, OmniOptimizer::OmniIndividual *ind2)
{
    ind2->copyParams(ind1);
    ind2->rank = ind1->rank;
    ind2->setCrowdingDistance(ind1->getCrowdingDistance());
    ind2->realvar_crowd_dist = ind1->realvar_crowd_dist;
    ind2->binvar_crowd_dist = ind1->binvar_crowd_dist;
    ind2->setCrowdingDistance(ind1->getCrowdingDistance());

    return;
}
