#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

void OmniOptimizer::allocate_memory_pop (OmniOptimizer::population* &pop, int size)
{
    int i;
    pop->ind = new OmniIndividual*[size];
    for (i=0; i<size; i++)
    {
        allocate_memory_ind (pop->ind[i]);
    }
    return;
}

void OmniOptimizer::allocate_memory_ind (OmniIndividual* &ind)
{
    ind = new OmniIndividual(nreal, nbin, nobj, ncon);

    return;
}

void OmniOptimizer::deallocate_memory_pop (OmniOptimizer::population *pop, int size)
{

    int i;

    for (i=0; i<size; i++)
    {
        deallocate_memory_ind (pop->ind[i]);
    }
    delete [] pop->ind;
	return;
}

void OmniOptimizer::deallocate_memory_ind (OmniIndividual *ind)
{
    delete ind;
	return;
}
