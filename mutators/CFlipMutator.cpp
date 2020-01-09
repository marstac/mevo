/*  Author:
  Marcin A. Stachura <m.stachura@mchtr.pw.edu.pl>

  Copyright (c) 2015 Marcin A. Stachura

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CFlipMutator.h"
#include <iostream>
namespace MEVO {
CFlipMutator::CFlipMutator() : CMutator()
{


}
CFlipMutator::~CFlipMutator() {
    FHILimits.clear();
    FLILimits.clear();

    FHRLimits.clear();
    FLRLimits.clear();
}

ICComputation* CFlipMutator::copy() {
    CFlipMutator* mut  = new CFlipMutator();
    mut->copyParams(this);
    return mut;
}
 void CFlipMutator::copyParams(CMutator* _m) {
    CMutator::copyParams(_m);
    CFlipMutator* m = dynamic_cast<CFlipMutator*>(_m);

    this->FHILimits = m->FHILimits;
    this->FLILimits = m->FLILimits;

    this->FHRLimits = m->FHRLimits;
    this->FLRLimits = m->FLRLimits;
}

const char* CFlipMutator::getDescripton() {
     std::stringstream desc;
     desc << "Flip mutator" << "\tProbability:\t"<<FProb;
     return desc.str().c_str();
}

void CFlipMutator::mutatePopulation(ICPopulation *_population) {
    if(FProb<=0) {return;}
    for (int i=0; i< _population->getSize(); i++) {
        mutateIndividual(_population->getIndividual(i), FProb);
    }

}

void CFlipMutator::mutateIndividual(ICIndividual* _ind, double _p) {
    int length = FIntVarSize + FRealVarSize-1;
    for (int i=0; i<=length;i++) {
        if (iRandGen->randomDouble(0,1) <= _p){
            flip(_ind, i);
        }
    }
}

void CFlipMutator::flip(ICIndividual *_ind, int _x) {
    if (_x< FRealVarSize) {
        _ind->getRealVar()[_x] = iRandGen->randomDouble(FLRLimits[_x], FHRLimits[_x]);
    } else {
        int p;
        do{
            p = iRandGen->randomInteger(FLILimits[_x-FRealVarSize], FHILimits[_x-FRealVarSize]);
        } while (p==_ind->getIntVar()[_x-FRealVarSize]);
        _ind->getIntVar()[_x-FRealVarSize] =p;
    }
}

void CFlipMutator::init(ICEvolOptimizer* _opt){
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


}


}
