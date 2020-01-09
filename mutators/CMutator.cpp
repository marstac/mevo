#include "CMutator.h"
namespace MEVO {
CMutator::CMutator() : CWithRNGen()
{
    FProb=0.0;
    FRealVarSize=0;
    FIntVarSize=0;
}

CMutator::~CMutator() {

}

void CMutator::init(ICEvolOptimizer* _opt) {
    FRealVarSize = _opt->getProblem()->getRealVarSize();
    FIntVarSize = _opt->getProblem()->getIntegerVarSize();


    if(FProb==0){
        if(_opt->getMutator() && _opt->getMutator()==this) {
            if(FRealVarSize+FIntVarSize) FProb = 1.0/(FRealVarSize+FIntVarSize);
        } else if (FIntVarSize && _opt->getIntMutator()==this) {
            FProb = 1.0/(FIntVarSize);
        } else if (FRealVarSize && _opt->getRealMutator()==this) {
            FProb = 1.0/(FRealVarSize);
        }
    }
}

void CMutator::execute(void* _params) {
    ICPopulation *pop = (ICPopulation*)_params;
    mutatePopulation(pop);
}
void CMutator::copyParams(CMutator* _m) {
    this->FProb = _m->FProb;
    this->FRealVarSize = _m->FRealVarSize;
    this->FIntVarSize = _m->FIntVarSize;

}

void CMutator::setProbability(double _p) {
    FProb = _p;
}

double  CMutator::getProbability() {
    return FProb;
}
}
