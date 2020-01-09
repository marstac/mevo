#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

void OmniOptimizer::report_pop (OmniOptimizer::population *pop, FILE *fpt)
{
    int i, j;
    for (i=0; i<popsize; i++)
    {
        for (j=0; j<nobj; j++)
        {
            fprintf(fpt,"%e\t",pop->ind[i]->getObj()[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i]->getConstr()[j]);
            }
        }
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i]->getRealVar()[j]);
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {

                    fprintf(fpt,"%i\t",pop->ind[i]->getIntVar()[j]);

            }
        }
        fprintf(fpt,"%e\t",pop->ind[i]->getConstrViolation());
        fprintf(fpt,"%d\t",pop->ind[i]->rank);
        fprintf(fpt,"%e\n",pop->ind[i]->getCrowdingDistance());
    }
    return;
}

void OmniOptimizer::report_feasible (OmniOptimizer::population *pop, FILE *fpt)
{
    int i, j;
    for (i=0; i<popsize; i++)
    {
        if (pop->ind[i]->getConstrViolation() == 0.0 && pop->ind[i]->rank==1)
        {
            for (j=0; j<nobj; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i]->getObj()[j]);
            }
            if (ncon!=0)
            {
                for (j=0; j<ncon; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i]->getConstr()[j]);
                }
            }
            if (nreal!=0)
            {
                for (j=0; j<nreal; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i]->getRealVar()[j]);
                }
            }
            if (nbin!=0)
            {
                for (j=0; j<nbin; j++)
                {
//                    for (k=0; k<nbits[j]; k++)
//                    {
//                        fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
//                    }
                }
            }
            fprintf(fpt,"%e\t",pop->ind[i]->getConstrViolation());
            fprintf(fpt,"%d\t",pop->ind[i]->rank);
            fprintf(fpt,"%e\n",pop->ind[i]->getCrowdingDistance());
        }
    }
    return;
}

void OmniOptimizer::report_var (OmniOptimizer::population *pop, FILE *fpt)
{
    int i, j;
    for (i=0; i<popsize; i++)
    {
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i]->getRealVar()[j]);
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
//				fprintf(fpt,"%e\t",pop->ind[i].getNumIntVar()[j]);
            }
        }
        fprintf(fpt,"\n");
    }
    return;
}

void OmniOptimizer::report_obj (OmniOptimizer::population *pop, FILE *fpt)
{
    MEVO::CPopulation resPop;

    for (int i = 0; i != popsize; i++ ) {
        if (pop->ind[i]->rank < 2) {
            resPop.addIndividual(pop->ind[i]);
        }
    }
    setLastPop(&resPop);

}
