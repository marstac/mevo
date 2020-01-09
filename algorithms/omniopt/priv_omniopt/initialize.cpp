#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../OmniOptimizer.h"

void OmniOptimizer::initialize_pop (OmniOptimizer::population *pop)
{
    MEVO::CPopulation ipop;
    for(int i = 0;i < popsize;i++)
    {
       ipop.addIndividual(pop->ind[i]);

    }
    initializePopulation(&ipop);

    return;
}

void OmniOptimizer::initialize_random_pop (OmniOptimizer::population *pop)
{

}

void OmniOptimizer::initialize_latin_pop (OmniOptimizer::population *pop)
{
    int i, j;
    int *a;
    int temp;
    int rand;
    double *grid, *val;
    a = (int *)malloc(popsize*sizeof(int));
    grid = (double *)malloc(nreal*sizeof(double));
    val = (double *)malloc(popsize*sizeof(double));
    if (nreal!=0)
    {
        for (j=0; j<nreal; j++)
        {
            grid[j] = (max_realvar[j]-min_realvar[j])/(double)popsize;
        }
        for (i=0; i<popsize; i++)
        {
            a[i] = i;
        }
        for (j=0; j<nreal; j++)
        {
            for (i=0; i<popsize; i++)
            {
                rand = iRandGen->randomInteger(i, popsize-1);
                temp = a[rand];
                a[rand] = a[i];
                a[i] = temp;
            }
            for (i=0; i<popsize; i++)
            {
                val[i] = iRandGen->randomDouble(min_realvar[j]+grid[j]*i, min_realvar[j]+grid[j]*(i+1));
                if (val[i] < min_realvar[j])
                    val[i] = min_realvar[j];
                if (val[i] > max_realvar[j])
                    val[i] = max_realvar[j];
                pop->ind[a[i]]->getRealVar()[j] = val[i];
            }
        }

    }
    if (nbin!=0)
    {
        for (i=0; i<popsize; i++)
        {
            for (j=0; j<nbin; j++)
            {
                pop->ind[i]->getIntVar()[j] = iRandGen->randomInteger(min_binvar[j], max_binvar[j]);

            }
        }
    }
    free (a);
    free (grid);
    free (val);
    return;
}

void OmniOptimizer::initialize_file_pop (OmniOptimizer::population *pop)
{
    FILE *fpt;
    int i, j;
    fpt = fopen("input_var.in","r");
    if (fpt==NULL)
    {
        printf("\n Input data file for initialization-from-file not found.");
        printf("\n The file 'input_var.in' does not exist, exiting \n");
        exit(1);
    }
    for (i=0; i<popsize; i++)
    {
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fscanf(fpt,"%f\t",(pop->ind[i]->getRealVar()[j]));
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
              fscanf(fpt,"%i\t",(pop->ind[i]->getIntVar()[j]));
            }
        }
        fscanf(fpt,"\n");
    }
    fclose(fpt);
    return;
}
