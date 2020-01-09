#include "CPertutbation.h"

#include <math.h>
namespace MEVO {
CPerturbation::CPerturbation() : CMutator()
{
    FMaxGen = 1000;
    FGen=0;
}

CPerturbation::~CPerturbation()
{
    FHILimits.clear();
    FLILimits.clear();

    FHRLimits.clear();
    FLRLimits.clear();
}

const char *CPerturbation::getDescripton()
{
    std::stringstream desc;
    desc << "Perturbation" << "\tProbability:\t"<<FProb;
    return desc.str().c_str();
}

void CPerturbation::init(ICEvolOptimizer *_opt)
{
    CMutator::init(_opt);
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
    FGen=0;
}

void CPerturbation::mutatePopulation(ICPopulation *_population)
{
    if(FProb<=0) {return;}
    if(FGen<FMaxGen) {
        for (int i=0; i< _population->getSize(); i++) {
            mutateIndividual(_population->getIndividual(i));
        }
    }
    FGen++;
}

ICComputation *CPerturbation::copy()
{
    CPerturbation* mut  = new CPerturbation();
    mut->copyParams(this);
    return mut;

}

void CPerturbation::mutateIndividual(ICIndividual *_ind)
{
//    int dimension=0;
//    double  pM=pow(1-(double)FGen/FMaxGen, 5.0/0.5);
//    if(iRandGen->randomDouble(0,1)<pM){

//        dimension= iRandGen->randomInteger(0, FRealVarSize + FIntVarSize-1);
//        if(dimension>=FRealVarSize) {
//           dimension-=FRealVarSize;
//            int MutRange=(FHILimits[dimension] - FLILimits[dimension])*pM;
//            if(MutRange<1){MutRange=1;}
//            double hb = _ind->getIntVar()[dimension]+MutRange;
//            double lb = _ind->getIntVar()[dimension]-MutRange;
//            if(lb<FLILimits[dimension]) {
//                lb=FLILimits[dimension];
//            } else if (hb>FHILimits[dimension]){
//                hb=FHILimits[dimension];
//            }
//            _ind->getIntVar()[dimension] = iRandGen->randomInteger(lb, hb);
//        } else {
//            double MutRange=(FHRLimits[dimension] -FLRLimits[dimension])*pM;
//            double hb = _ind->getRealVar()[dimension]+MutRange;
//            double lb = _ind->getRealVar()[dimension]-MutRange;
//            if(lb<FLRLimits[dimension]) {
//                lb=FLRLimits[dimension];
//            } else if (hb>FHRLimits[dimension]){
//                hb=FHRLimits[dimension];
//            }
//            _ind->getRealVar()[dimension] = iRandGen->randomDouble(lb, hb);
//        }
//    }
        int dimension=0;
        double  temp=FGen/FMaxGen;
        double pM = pow(temp,1.7)-2.0*temp+1;
        if(iRandGen->randomDouble(0,1)<pM){
            dimension= iRandGen->randomInteger(0, FRealVarSize + FIntVarSize-1);
            if(dimension>=FRealVarSize) {
               dimension-=FRealVarSize;
                _ind->getIntVar()[dimension] = iRandGen->randomInteger(FLILimits[dimension], FHILimits[dimension]);
            } else {
                _ind->getRealVar()[dimension] = iRandGen->randomDouble(FLRLimits[dimension], FHRLimits[dimension]);
            }
        }
}

unsigned int CPerturbation::getMaxGen() const
{
    return FMaxGen;
}

void CPerturbation::setMaxGen(unsigned int _value)
{
    FMaxGen = _value;
}

void CPerturbation::copyParams(CMutator *_m)
{
    CMutator::copyParams(_m);
    CPerturbation* m = dynamic_cast<CPerturbation*>(_m);

    this->FHILimits = m->FHILimits;
    this->FLILimits = m->FLILimits;

    this->FHRLimits = m->FHRLimits;
    this->FLRLimits = m->FLRLimits;
    this->FMaxGen = m->FMaxGen;
    this->FGen = m->FGen;
}


}

