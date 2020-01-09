#include "OmniOptimizer.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "../../mutators/CPolynomialMutator.h"
#include "../../crossovers/CSBXCrossover.h"
#include "../../utils/messages.h"

#include "OmniOptimizer.h"

OmniOptimizer::OmniIndividual::OmniIndividual(int _numVar, int _numIntVar, int _numObj, int _numConstr) : CIndividual(_numVar, _numIntVar, _numObj, _numConstr) {

}

MEVO::ICIndividual *OmniOptimizer::OmniIndividual::copy()
{
    OmniIndividual* ind = new OmniIndividual(*this);
    return ind;
}
OmniOptimizer::OmniIndividual::~OmniIndividual(){};
OmniOptimizer::OmniOptimizer() : CGeneticAlgorithm(){

    this->setPopSize(40);

    //genetic parameters for fine tuning
    //problem independent

	nbin = 0;
    popsize = 0;
    nobj = 0;
    ncon = 0;
    nreal = 0;

    mate = 1; //printf("\n Enter the choice for selection restriction, 0 for normal selection, 1 for restricted selection : ");
    var_option = 1; //space niching one of them must be 1
    obj_option = 1; //objective niching
    input_type = 1; //printf("\n Enter the choice for OmniOptimizer::population initialization, 0 for random, 1 for latin-hypercube based sampling, 2 for reading initial OmniOptimizer::population from a file : ");
    frequency = 2; //printf("\n Enter the frequency with which the OmniOptimizer::population information (only the variables) is to be stored : ");
    run_mode =1; //printf("\n Enter the simulation mode, 0 for Analysis mode, 1 for Turbo mode : ");

    setMutator(new MEVO::CPolynomialMutator);
    setCrossover(new MEVO::CSBXCrossover);


    min_obj = NULL;
    max_obj = NULL;

    min_realvar = NULL;
    max_realvar = NULL;

    min_binvar =NULL;
    max_binvar = NULL;

}

OmniOptimizer::~OmniOptimizer() {
//    if (min_obj) {
//        delete [] min_obj;
//        delete [] max_obj;
//    }

//    if (min_realvar) {
//        delete [] min_realvar;
//        delete [] max_realvar;
//    }

//    if (min_binvar) {
//        delete [] min_binvar;
//        delete [] max_binvar;
//    }

}
bool OmniOptimizer::checkConf() {
    return CGeneticAlgorithm::checkConf();

}

void OmniOptimizer::alocateMemory() {
	popsize = this->getPopSize();
    nobj = this->FProblem->getObjectiveFncCnt();
    ncon = this->FProblem->getConstraintFncCnt();
    nreal = this->FProblem->getRealVarSize();
    nbin= this->FProblem->getIntegerVarSize();


//    if (min_obj) {
//        delete [] min_obj;
//        delete [] max_obj;
//    }

//    if (min_realvar) {
//        delete [] min_realvar;
//        delete [] max_realvar;
//    }

//    if (min_binvar) {
//        delete [] min_binvar;
//        delete [] max_binvar;
//    }

    min_obj = new double [nobj];
    max_obj = new double [nobj];

    min_realvar = new double[nreal];
    max_realvar = new double[nreal];

	min_binvar = new double[nbin];
	max_binvar = new double[nbin ];

    for (int i=0; i <  nreal; i++) {
      min_realvar[i] = FProblem->getRealLowLimit(i);
      max_realvar[i] = FProblem->getRealHighLimit(i);
	}

    for (int i=0; i <  nbin; i++) {
      min_binvar[i] = FProblem->getIntegerLowLimit(i);
      max_binvar[i] = FProblem->getIntegerHighLimit(i);
	}
}
void OmniOptimizer::run() {

    initVariables();
	alocateMemory();

    int i;

    OmniOptimizer::population *parent_pop;
    OmniOptimizer::population *child_pop;
    OmniOptimizer::population *mixed_pop;
	std::string befs = "\n Omni-optimizer code developed at KanGAL, IIT Kanpur. \n Copyright (C) 2006 by Dr. Kalyanmoy Deb and Santosh Tiwari\n";
    fireOnBeforeStartNotify(befs.c_str(), this);

	fflush(stdout);

	pdefinit();


    parent_pop = new population;
    child_pop = new population;
    mixed_pop = new population;
    allocate_memory_pop (parent_pop, popsize);
    allocate_memory_pop (child_pop, popsize);
    allocate_memory_pop (mixed_pop, 2*popsize);
    initialize_pop (parent_pop);

    evaluate_pop (parent_pop);
    define_epsilon (parent_pop, popsize, NULL);
    assign_rank_and_crowding_distance (parent_pop);
    MEVO::CPopulation spop;
       i=2;

        do
        {
            if (mate==0)
            {
                selection (parent_pop, child_pop);
            }
            else
            {
                restricted_selection(parent_pop, child_pop);
            }
            mutation_pop (child_pop);
            evaluate_pop(child_pop);
            merge (parent_pop, child_pop, mixed_pop);
            define_epsilon (mixed_pop, 2*popsize, NULL);
            fill_nondominated_sort (mixed_pop, parent_pop);

            spop.clearIndividuals();
            for (int j=0;j<popsize;j++) {
                spop.addIndividual(parent_pop->ind[j]);
            }
            i++;
            fireOnGenerationDoneNotify(i, &spop);
        } while (!checkStop(&spop));

    setLastPop(&spop);
    fireOnAfterStopNotify("Omni Optimizer Finished", FLastPop);

    free (min_obj);
    free (max_obj);
    if (nreal!=0)
    {
        free (min_realvar);
        free (max_realvar);
    }
    if (nbin!=0)
    {
        free (min_binvar);
        free (max_binvar);
    }

	deallocate_memory_pop (parent_pop, popsize);
	deallocate_memory_pop (child_pop, popsize);
	deallocate_memory_pop (mixed_pop, 2*popsize);
    delete parent_pop;
    delete child_pop;
    delete mixed_pop;

}





