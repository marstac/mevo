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

#include "CPolynomialMutator.h"

#include "CFlipMutator.h"
#include <cmath>
namespace MEVO {
CPolynomialMutator::CPolynomialMutator() : CMutator()
{
    FEtaMut = 15;

}
CPolynomialMutator::~CPolynomialMutator() {
    FHLimits.clear();
    FLLimits.clear();
}

ICComputation* CPolynomialMutator::copy() {
    CPolynomialMutator* mut  = new CPolynomialMutator();
    mut->copyParams(this);
    return mut;
}

const char* CPolynomialMutator::getDescripton(){
    std::stringstream desc;
    desc << "Polynomial Mutator" << "\tProbability:\t"<<FProb<<"\tDistribution Index:\t"<<FEtaMut;
    return desc.str().c_str();
}

void CPolynomialMutator::copyParams(CMutator *_m) {
    CMutator::copyParams(_m);
    CPolynomialMutator* m = dynamic_cast<CPolynomialMutator*>(_m);
    this->FEtaMut = m->FEtaMut;
    this->FLLimits = m->FLLimits;
    this->FHLimits = m->FHLimits;
}

void CPolynomialMutator::mutatePopulation(ICPopulation *_population) {
    if(FProb<=0) {return;}
    for (unsigned int i=0; i< _population->getSize(); i++) {
            mutateIndividual(_population->getIndividual(i), FProb);
    }
}

void CPolynomialMutator::mutateIndividual(ICIndividual *_ind, double _p) {
      register double rnd = 0.0;
      register double delta1 = 0.0;
      register double delta2 = 0.0;
      register double mut_pow = 0.0;
      register double deltaq = 0.0;
      register double y = 0.0;
      register double yl = 0.0;
      register double yu = 0.0;
      register double val = 0.0;
      register double xy = 0.0;
      for (register int j = 0; j < FRealVarSize; j++) {
            //this is the equation (6) described at page 15 of the article!
           if (iRandGen->randomDouble(0.0, 1.0) <= _p) {
              y= _ind->getRealVar()[j];
              yl =FLLimits[j];

              yu =FHLimits[j];

            delta1 = (y - yl) / (yu - yl);
            delta2 = (yu - y) / (yu - yl);
            rnd = iRandGen->randomDouble(0.0, 1.0);
            mut_pow = 1.0 / (FEtaMut + 1.0);
            if (rnd <= 0.5) {
              xy = 1.0 - delta1;
              val = 2.0 * rnd + (1.0 - 2.0 * rnd) * (pow(xy, (FEtaMut + 1.0)));
              deltaq = pow(val, mut_pow) - 1.0;
            } else {
              xy = 1.0 - delta2;
              val = 2.0 * (1.0 - rnd) + 2.0 * (rnd - 0.5) * (pow(xy, (FEtaMut + 1.0)));
              deltaq = 1.0 - (pow(val, mut_pow));
            }
            y = y + deltaq * (yu - yl);
            if (y < yl) {
              y = yl;
            }
            if (y > yu) {
              y = yu;
            }

            _ind->getRealVar()[j] = y;
          }
     }
}


void CPolynomialMutator::init(ICEvolOptimizer* _opt){
    CMutator::init(_opt);

    if (_opt->getMutator()&&_opt->getProblem()->getIntegerVarSize()) {
        ICComputation* m= this->copy();
        ICMutator* mut = dynamic_cast<ICMutator*>(m);
        _opt->setSeparateMutators(mut, new CFlipMutator);
    } else {
        FHLimits.resize(FRealVarSize,0.0);
        FLLimits.resize(FRealVarSize,0.0);
        for (int i=0; i<FRealVarSize;i++) {
            FLLimits[i]=_opt->getProblem()->getRealLowLimit(i);
            FHLimits[i]=_opt->getProblem()->getRealHighLimit(i);
        }
    }
}
void CPolynomialMutator::setDistributionIndex(double _di) {
    FEtaMut = _di;
}

double CPolynomialMutator::getDistributionIndex(){
    return FEtaMut;
}

}
