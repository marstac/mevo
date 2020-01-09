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

#include "CParticle.h"
#include "CPSOalgorithm.h"

#include <math.h>
#include <limits.h>
#include <stdlib.h>
namespace MEVO {
CParticle::CParticle(int _numVar, int _numIntVar, int _numObj, int _numConstr, double _w, double _c1, double _c2, CPSOAlgorithm* _parent) : CIndividual(_numVar,  _numIntVar, _numObj, _numConstr), CWithRNGen()
{
    FParent = _parent;
    FVelocity = new double[_numVar+ _numIntVar];
    FMaxVelocity = new double[_numVar+ _numIntVar];
    FMinVelocity = new double[_numVar+ _numIntVar];

    for (int i = 0; i< FNumRealVar; i++) {
        FMaxVelocity[i] = 0.25*(FParent->getProblem()->getRealHighLimit(i) - FParent->getProblem()->getRealLowLimit(i));
        FMinVelocity[i] = -1*FMaxVelocity[i];
        FVelocity[i]= 0;//iRandGen->randomDouble(FMinVelocity[i], FMaxVelocity[i]);
    }

    for (int i = 0; i< FNumIntVar; i++) {
        FMaxVelocity[i+FNumRealVar] = 1*(FParent->getProblem()->getIntegerHighLimit(i) - FParent->getProblem()->getIntegerLowLimit(i));
        FMinVelocity[i+FNumRealVar] = -1*FMaxVelocity[i+FNumRealVar];
        FVelocity[i+FNumRealVar]= 0;//iRandGen->randomDouble(FMinVelocity[i+FNumRealVar], FMaxVelocity[i+FNumRealVar]);
    }

    if (FNumRealVar) {
        FBestRealVar = new double[_numVar];
    }
    if (FInegerVar) {
        FBestIntVar = new int[_numIntVar];
    }
    FBestObj = new double[_numObj];
    for (int i=0; i<_numObj; i++) {
        FBestObj[i] = std::numeric_limits<double>::max();
    }
    FBestConstrVal = std::numeric_limits<double>::max();
    FLeader = NULL;
    W=_w;
    C1=_c1;
    C2=_c2;
    FBest =true;


}

CParticle::CParticle(const CParticle& _p) : CIndividual (_p), CWithRNGen(_p) {
    FVelocity = new double[FNumRealVar+ FNumIntVar];
    FMaxVelocity = new double[FNumRealVar+ FNumIntVar];
    FMinVelocity = new double[FNumRealVar+ FNumIntVar];

    if (FNumRealVar) {
        FBestRealVar = new double[FNumRealVar];
    }
    if (FInegerVar) {
        FBestIntVar = new int[FNumIntVar];
    }
    FBestObj = new double[FNumObj];

    CParticle::copyParams(&_p);
}

ICIndividual *CParticle::copy()
{
    CParticle* ind = new CParticle(*this);
    return ind;
}

void CParticle::copyParams(const CParticle *_p){

    for (int i=0; i<FNumRealVar+ FNumIntVar;i++){
        FVelocity[i] = _p->FVelocity[i];
        FMaxVelocity[i] = _p->FMaxVelocity[i];
        FMinVelocity[i] = _p->FMinVelocity[i];
    }
    for (int i=0; i<FNumRealVar;i++){
        FBestRealVar[i] = _p->FBestRealVar[i];
    }
    for (int i=0; i<FNumIntVar;i++){
        FBestIntVar[i] = _p->FBestIntVar[i];
    }
    for (int i=0; i<FNumObj;i++){
        FBestObj[i] = _p->FBestObj[i];
    }
    FBestConstrVal = _p->FBestConstrVal;
    W=_p->W;
    C1=_p->C1;
    C2=_p->C2;
    FLeader = _p->FLeader;
    FParent = _p->FParent;
    FBest = _p->FBest;

}

CParticle::~CParticle() {
    delete [] FVelocity;
    delete [] FMaxVelocity;
    delete [] FMinVelocity;

    if (FNumRealVar) {
        delete [] FBestRealVar;
    }
    if (FInegerVar) {
        delete [] FBestIntVar;
    }

    delete [] FBestObj;
}

double* CParticle::getVelocity() {
    return FVelocity;
}

double* CParticle::getBestRealVar() {
    return FBestRealVar;
}

int* CParticle::getBestIntVar(){
    return FBestIntVar;
}

double* CParticle::getBestObj() {
    return FBestObj;
}

void CParticle::prepare(){
    if(FLeader) updatePosition();
}

void CParticle::update() {
    CParticle p = CParticle(*this);
    for (int i=0; i<FNumObj; i++) {
        p.getObj()[i] = this->getBestObj()[i];
    }
    p.setConstrViolation(this->FBestConstrVal);
    std::deque<double>* epss = NULL;
    if (FEps.size()) {
        epss=&FEps;
    }

    int dom =checkDominance(&p, epss);

    if(dom>=0) {
        updateBest();
        FBest=true;
    } else {
        FBest=false;
    }
}

void CParticle::updateBest() {
    for (int i=0; i<FNumRealVar;i++){
        FBestRealVar[i] = FRealVar[i];
    }
    for (int i=0; i<FNumIntVar;i++){
        FBestIntVar[i] = FInegerVar[i];
    }
    for (int i=0; i<FNumObj;i++){
        FBestObj[i] = FObj[i];
    }
    FBestConstrVal = FConstrViolation;
}

double CParticle::getC2() const
{
    return C2;
}

void CParticle::setC2(double value)
{
    C2 = value;
}

double CParticle::getC1() const
{
    return C1;
}

void CParticle::setC1(double value)
{
    C1 = value;
}

double CParticle::getW() const
{
    return W;
}

void CParticle::setW(double value)
{
    W = value;
}

void CParticle::updatePosition() {
    for (int i = 0; i< FNumRealVar; i++) {
        FVelocity[i]= W*FVelocity[i]+C1*iRandGen->randomDouble(0,1)*(FLeader->getRealVar()[i]-FRealVar[i])+C2*iRandGen->randomDouble(0,1)*(FBestRealVar[i]-FRealVar[i]);

        if (FVelocity[i] > FMaxVelocity[i]) {
            FVelocity[i] = FMaxVelocity[i];
        } else if (FVelocity[i] < FMinVelocity[i]) {
            FVelocity[i] = FMinVelocity[i];
        }

        FRealVar[i] = FRealVar[i] + FVelocity[i];
        if(FRealVar[i]<FParent->getProblem()->getRealLowLimit(i)) {
            FRealVar[i]=FParent->getProblem()->getRealLowLimit(i);
            FVelocity[i]=0;//-1*FVelocity[i];
        } else if (FRealVar[i]>FParent->getProblem()->getRealHighLimit(i)){
            FRealVar[i]=FParent->getProblem()->getRealHighLimit(i);
            FVelocity[i]=0;//-1*FVelocity[i];
        }
    }
    for (int i =0; i< FNumIntVar; i++) {
        FVelocity[i+FNumRealVar] = W*FVelocity[i+FNumRealVar]+C1*iRandGen->randomDouble(0,1)*(FLeader->getIntVar()[i]-FInegerVar[i])
                                    +C2*iRandGen->randomDouble(0,1)*(FBestIntVar[i]-FInegerVar[i]);

        if (FVelocity[i+FNumRealVar]> FMaxVelocity[i+FNumRealVar]) {
            FVelocity[i+FNumRealVar]= FMaxVelocity[i+FNumRealVar];
        } else if (FVelocity[i+FNumRealVar]< FMinVelocity[i+FNumRealVar]) {
            FVelocity[i+FNumRealVar]= FMinVelocity[i+FNumRealVar];
        }

        double x = FInegerVar[i] + FVelocity[i+FNumRealVar];
        int y = floor(x);
        double p = x-y;
        if (iRandGen->randomDouble(0,1)<p) {
            FInegerVar[i] = y+1;
        } else {
            FInegerVar[i] = y;
        }
        if(FInegerVar[i]<FParent->getProblem()->getIntegerLowLimit(i)) {
            FInegerVar[i]=FParent->getProblem()->getIntegerLowLimit(i);
            FVelocity[i+FNumRealVar]=0;//-1*FVelocity[i+FNumRealVar];
        } else if (FInegerVar[i]>FParent->getProblem()->getIntegerHighLimit(i)){
            FInegerVar[i]=FParent->getProblem()->getIntegerHighLimit(i);
            FVelocity[i+FNumRealVar]=0;//-1*FVelocity[i+FNumRealVar];
        }
    }
}

void CParticle::setLeader(CParticle* _l) {
    FLeader = _l;
}


CParticle* CParticle::getLeader() {
    return FLeader;
}
bool CParticle::isBest() {
    return FBest;
}
}
