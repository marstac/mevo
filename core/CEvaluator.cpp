#include "CEvaluator.h"
#include "CEvolOptimizer.h"
namespace MEVO {
CEvaluator::CEvaluator()
{
    FProblem=NULL;
}
CEvaluator::~CEvaluator() {
    if (FProblem)
        delete FProblem;
}

void CEvaluator::init(ICEvolOptimizer* _opt) {
    if (FProblem)
        delete FProblem;

    FProblem = _opt->getProblem()->copy();
}


ICComputation* CEvaluator::copy() {
    CEvaluator* eval = new CEvaluator();
    eval->FProblem = this->FProblem->copy();
    return eval;
}

void CEvaluator::execute(void* _params) {
    ICPopulation *population = (ICPopulation*)_params;
    evaluatePopulation(population);
}

void CEvaluator::evaluatePopulation(ICPopulation *_population) {
    for (int i =0; i< _population->getSize();i++) {
        evaluateIndividual(_population->getIndividual(i));
    }
}

void CEvaluator::evaluateIndividual(ICIndividual *_ind) {
    _ind->prepare();
    _ind->setConstrViolation(0);
    double cv=0;
    double vv=0;
    for (int i=0; i<FProblem->getObjectiveFncCnt();i++) {
        _ind->getObj()[i] = FProblem->getObjectiveFnc(i)->evaluate(_ind->getRealVar(),
                                                            _ind->getNumRealVar(),
                                                            _ind->getIntVar(),
                                                            _ind->getNumIntVar());
    }
    for (int i=0; i<FProblem->getConstraintFncCnt();i++) {
       vv = FProblem->getConstraintFnc(i)->evaluate(_ind->getRealVar(),
                                   _ind->getNumRealVar(),
                                   _ind->getIntVar(),
                                   _ind->getNumIntVar());
       if (vv>0) {
           _ind->getConstr()[i]=vv;
           cv+=vv;

       } else {
           _ind->getConstr()[i]=0;
       }

    }
    _ind->setConstrViolation(cv);
    _ind->update();
}

}

