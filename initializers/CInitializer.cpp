#include "CInitializer.h"


namespace MEVO {

CInitializer::CInitializer() : CWithRNGen()
{
}

CInitializer::~CInitializer(){
    FHILimits.clear();
    FHRLimits.clear();
    FLILimits.clear();
    FLRLimits.clear();
}

void CInitializer::init(ICEvolOptimizer* _opt) {
    FRealVarSize = _opt->getProblem()->getRealVarSize();
    FIntVarSize = _opt->getProblem()->getIntegerVarSize();

    FHRLimits.resize(FRealVarSize,0.0);
    FLRLimits.resize(FRealVarSize,0.0);
    for (int i=0; i<FRealVarSize;i++) {
        FLRLimits[i]=_opt->getProblem()->getRealLowLimit(i);
        FHRLimits[i]=_opt->getProblem()->getRealHighLimit(i);
    }
    FHILimits.resize(FIntVarSize,0.0);
    FLILimits.resize(FIntVarSize,0.0);
    for (int i=0; i<FIntVarSize;i++) {
        FLILimits[i]=_opt->getProblem()->getIntegerLowLimit(i);
        FHILimits[i]=_opt->getProblem()->getIntegerHighLimit(i);
    }
}
void CInitializer::copyParams(CInitializer* _i) {
    this->FRealVarSize = _i->FRealVarSize;
    this->FIntVarSize = _i->FIntVarSize;

    this->FHILimits = _i->FHILimits;
    this->FLILimits = _i->FLILimits;

    this->FHRLimits = _i->FHRLimits;
    this->FLRLimits = _i->FLRLimits;
}

void CInitializer::execute(void* _params) {
    ICPopulation *pop = (ICPopulation*)_params;
    initialize(pop);
}


}
