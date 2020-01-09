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

#include "CDECrossover.h"

#include "../core/CEvolOptimizer.h"

#include "COnePointCrossover.h"

namespace MEVO {
CDECrossover::CDECrossover()
{
    FCR = 0.5;
    FProb = 0.1;
    FArchive = NULL;
    FType = DE_2;
}
CDECrossover::~CDECrossover()
{

}

void CDECrossover::setCrossoverRate(double _c) {
    FCR = _c;
}

double CDECrossover::getCrossoverRate() {
    return FCR;
}

const char* CDECrossover::getDescripton(){
    std::stringstream desc;
    desc << "DE Crossover" << "\tProbability:\t"<<FProb<<"\tCrossover rate:\t"<<FCR;
    return desc.str().c_str();
}


void CDECrossover::init(ICEvolOptimizer *_opt){
    CCrossover::init(_opt);
    FLRLimits.clear();
    FHRLimits.clear();
    for (int i=0; i<FRealVarSize;i++) {
        FLRLimits.push_back(_opt->getProblem()->getRealLowLimit(i));
        FHRLimits.push_back(_opt->getProblem()->getRealHighLimit(i));
    }


    CAlgWithArchive* arch = dynamic_cast<CAlgWithArchive*>(_opt);
    if (arch) {
        FArchive = arch->getArchive();
    }

    CGeneticAlgorithm* gen = dynamic_cast<CGeneticAlgorithm*>(_opt);
    if(!gen) {return;}
    if (gen->getCrossover()&&gen->getProblem()->getIntegerVarSize()) {
        ICComputation* c= this->copy();
        ICCrossover* cr = dynamic_cast<ICCrossover*>(c);
        gen->setSeparateCrossovers(cr, new COnePointCrossover);
    }
}

void CDECrossover::crossover(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation){
    if (FArchive) {
        if (FType == DE) {
            crossDE(_parentPopulation, _offspringPopulation);
        } else if (FType == DE_1) {
            crossDE_1(_offspringPopulation);
        } else {
            crossDE_2(_parentPopulation, _offspringPopulation);
        }
    } else {
        crossDE(_parentPopulation, _offspringPopulation);
    }
}

void CDECrossover::crossDE(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation) {
    int no=_offspringPopulation->getSize();
    int np = _parentPopulation->getSize()-1;

    for (int i=0; i<no; i++) {
        ICIndividual* child = _offspringPopulation->getIndividual(i);

        ICIndividual* r1 = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
        ICIndividual* r2;
        ICIndividual* r3;
        do {
            r2 = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
        } while (r1==r2);

        do {
            r3 = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
        } while (r1==r3 || r3==r2);

        ICIndividual* parent;
        do {
            parent = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
        } while (parent==r1 || parent==r2 || parent==r3);

        crossIndividuals(r1, r2, r3, parent, child);
    }
}

void CDECrossover::crossDE_1(ICPopulation* _offspringPopulation){
    int no=_offspringPopulation->getSize();
    int np = FArchive->getSize()-1;

    for (int i=0; i<no; i++) {
        ICIndividual* child = _offspringPopulation->getIndividual(i);

        ICIndividual* r1 = FArchive->getIndividual(iRandGen->randomInteger(0, np));
        ICIndividual* r2;
        ICIndividual* r3;
        do {
            r2 = FArchive->getIndividual(iRandGen->randomInteger(0, np));
        } while (r1==r2);

        do {
            r3 = FArchive->getIndividual(iRandGen->randomInteger(0, np));
        } while (r1==r3 || r3==r2);

        ICIndividual* parent;
        do {
            parent = FArchive->getIndividual(iRandGen->randomInteger(0, np));
        } while (parent==r1 || parent==r2 || parent==r3);

        crossIndividuals(r1, r2, r3, parent, child);
    }
}

void CDECrossover::crossDE_2(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation) {
    int no=_offspringPopulation->getSize();
    int na = FArchive->getSize()-1;
    int np = _parentPopulation->getSize()-1;
    for (int i=0; i<no; i++) {
        ICIndividual* child = _offspringPopulation->getIndividual(i);

        ICIndividual* r1 = FArchive->getIndividual(iRandGen->randomInteger(0, na));
        ICIndividual* r2;
        ICIndividual* r3;
        do {
            r2 = FArchive->getIndividual(iRandGen->randomInteger(0, na));
        } while (r1==r2);

        do {
            r3 = FArchive->getIndividual(iRandGen->randomInteger(0, na));
        } while (r1==r3 || r3==r2);

        ICIndividual* parent;
        do {
            parent = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
        } while (parent==r1 || parent==r2 || parent==r3);

        crossIndividuals(r1, r2, r3, parent, child);
    }
}

ICComputation* CDECrossover::copy() {
    CDECrossover* c = new CDECrossover();
    c->copyParams(this);
    return c;
}

void CDECrossover::copyParams(CCrossover* _c) {
    CCrossover::copyParams(_c);
    CDECrossover* c = dynamic_cast<CDECrossover*>(_c);
    this->FCR= c->FCR;
    this->FHRLimits = c->FHRLimits;
    this->FLRLimits = c->FLRLimits;
    this->FArchive =  c->FArchive;
    this->FType = c->FType;
}

void CDECrossover::crossIndividuals(ICIndividual *_r1, ICIndividual *_r2, ICIndividual *_r3, ICIndividual *_parent, ICIndividual *_child) {
    unsigned int jrand = iRandGen->randomInteger(0, FRealVarSize - 1);
    for (register unsigned int j = 0; j < FRealVarSize; j++) {
      if (iRandGen->randomDouble(0.0, 1.0) < FProb || j == jrand) {
         double val = _r3->getRealVar()[j] + FCR * (_r1->getRealVar()[j] - _r2->getRealVar()[j]);
         _child->getRealVar()[j] = val;
      } else {
         _child->getRealVar()[j] = _parent->getRealVar()[j];
      }
      if (_child->getRealVar()[j]<FLRLimits[j]) {
          _child->getRealVar()[j]=FLRLimits[j];
      } else if (_child->getRealVar()[j]>FHRLimits[j]) {
          _child->getRealVar()[j]=FHRLimits[j];
      }
   }
}

void CDECrossover::setType(enDEType _t) {
    FType = _t;
}

CDECrossover::enDEType CDECrossover::getType() {
    return FType;
}
}
