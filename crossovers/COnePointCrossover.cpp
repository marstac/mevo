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

#include "COnePointCrossover.h"
namespace MEVO {
COnePointCrossover::COnePointCrossover() : CCrossover()
{

}

COnePointCrossover::~COnePointCrossover() {

}

void COnePointCrossover::init(ICEvolOptimizer* _opt) {
    CCrossover::init(_opt);
}
ICComputation* COnePointCrossover::copy() {
    COnePointCrossover* c = new COnePointCrossover();
    c->copyParams(this);
    return c;
}

const char* COnePointCrossover::getDescripton(){
    std::stringstream desc;
    desc << "One point Crossover" << "\tProbability:\t"<<FProb;
    return desc.str().c_str();
}

void COnePointCrossover::copyParams(CCrossover* _c) {
    CCrossover::copyParams(_c);
}

void COnePointCrossover::crossover(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation) {
    int no=_offspringPopulation->getSize();
    int np = _parentPopulation->getSize()-1;
    for (int i=0; i<no; i+=2) {
        ICIndividual* child1 = _offspringPopulation->getIndividual(i);
        ICIndividual* child2 = _offspringPopulation->getIndividual(i+1);

        ICIndividual* parent1;
        ICIndividual* parent2;
        do {
            parent1 = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
            parent2 = _parentPopulation->getIndividual(iRandGen->randomInteger(0, np));
        } while (parent1==parent2);

        crossTwoIndividuals(parent1,parent2,child1, child2);
    }
}
void COnePointCrossover::crossTwoIndividuals(ICIndividual *_parent1, ICIndividual *_parent2, ICIndividual *_child1, ICIndividual *_child2) {
    if (iRandGen->randomDouble(0,1) <= FProb) {
        int size = FRealVarSize+FIntVarSize;
        int place = iRandGen->randomInteger(0, size-1);
        for (int i=0; i<place;i++) {
            if (i< FRealVarSize) {
                _child1->getRealVar()[i]=_parent1->getRealVar()[i];
                _child2->getRealVar()[i]=_parent2->getRealVar()[i];
            } else {
                _child1->getIntVar()[i-FRealVarSize]=_parent1->getIntVar()[i-FRealVarSize];
                _child2->getIntVar()[i-FRealVarSize]=_parent2->getIntVar()[i-FRealVarSize];
            }
        }
        for (int i=place; i<size; i++) {
            if (i< FRealVarSize) {
                _child1->getRealVar()[i]=_parent2->getRealVar()[i];
                _child2->getRealVar()[i]=_parent1->getRealVar()[i];
            } else {
                _child1->getIntVar()[i-FRealVarSize]=_parent2->getIntVar()[i-FRealVarSize];
                _child2->getIntVar()[i-FRealVarSize]=_parent1->getIntVar()[i-FRealVarSize];
            }
        }
    } else {
        for (int i=0; i< FRealVarSize; i++) {
            _child1->getRealVar()[i] = _parent1->getRealVar()[i];
            _child2->getRealVar()[i] = _parent2->getRealVar()[i];
        }
        for (int i=0; i< FIntVarSize; i++) {
            _child1->getIntVar()[i] = _parent1->getIntVar()[i];
            _child2->getIntVar()[i] = _parent2->getIntVar()[i];
        }
    }
}
}
