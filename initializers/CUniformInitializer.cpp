#include "CUniformInitializer.h"

namespace MEVO {
CUniformInitializer::CUniformInitializer(): CInitializer() {

}


CUniformInitializer::~CUniformInitializer(){

};


void CUniformInitializer::initialize(ICPopulation *_population) {
    for (int i=0; i<_population->getSize();i++) {
        ICIndividual* ind = _population->getIndividual(i);
        for (int j=0; j< FRealVarSize;j++) {
            ind->getRealVar()[j] = iRandGen->randomDouble(FLRLimits[j], FHRLimits[j]);
        }
        for (int j=0; j< FIntVarSize;j++) {
            ind->getIntVar()[j] = iRandGen->randomInteger(FLILimits[j], FHILimits[j]);
        }
    }
}

ICComputation* CUniformInitializer::copy() {
    CUniformInitializer* u = new CUniformInitializer();
    u->copyParams(this);
    return u;
}

}
