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

#include "CSBXCrossover.h"
#include "COnePointCrossover.h"

#include "../core/CGeneticAlgorithm.h"

#include <math.h>

namespace MEVO {

CSBXCrossover::CSBXCrossover()
{
    FEta = 15;
}
CSBXCrossover::~CSBXCrossover()
{
}

void CSBXCrossover::init(ICEvolOptimizer* _opt){
    CCrossover::init(_opt);
    FHRLimits.resize(FRealVarSize,0.0);
    FLRLimits.resize(FRealVarSize,0.0);
    for (int i=0; i<FRealVarSize;i++) {
        FLRLimits[i]=_opt->getProblem()->getRealLowLimit(i);
        FHRLimits[i]=_opt->getProblem()->getRealHighLimit(i);
    }

    CGeneticAlgorithm* gen = dynamic_cast<CGeneticAlgorithm*>(_opt);

    if(!gen) {return;}
    if (gen->getCrossover()&&gen->getProblem()->getIntegerVarSize()) {
        ICComputation* c= this->copy();
        ICCrossover* cr = dynamic_cast<ICCrossover*>(c);
        gen->setSeparateCrossovers(cr, new COnePointCrossover);
    }
}

void CSBXCrossover::crossover(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation){
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

ICComputation* CSBXCrossover::copy() {
    CSBXCrossover* c = new CSBXCrossover();
    c->copyParams(this);
    return c;
}
const char* CSBXCrossover::getDescripton(){
    std::stringstream desc;
    desc << "SBX Crossover" << "\tProbability:\t"<<FProb<<"\tDistribution index:\t"<<FEta;
    return desc.str().c_str();
}
void CSBXCrossover::copyParams(CCrossover* _c) {
    CCrossover::copyParams(_c);

    CSBXCrossover* c = dynamic_cast<CSBXCrossover*>(_c);
    this->FHRLimits = c->FHRLimits;
    this->FLRLimits = c->FLRLimits;
    this->FEta = c->FEta;
}

void CSBXCrossover::crossTwoIndividuals(ICIndividual *_parent1, ICIndividual *_parent2, ICIndividual *_child1, ICIndividual *_child2){
    int i;
    double rand;
    double y1, y2, yl, yu;
    double c1, c2;
    double alpha, beta, betaq;

    if (iRandGen->randomDouble(0,1) <= FProb)
    {
        for (i=0; i<FRealVarSize; i++)
        {
            if (iRandGen->randomDouble(0,1)<=0.5 )
            {
                if (fabs(_parent1->getRealVar()[i]-_parent2->getRealVar()[i]) > SBX_EPS)
                {
                    if (_parent1->getRealVar()[i] < _parent2->getRealVar()[i])
                    {
                        y1 = _parent1->getRealVar()[i];
                        y2 = _parent2->getRealVar()[i];
                    }
                    else
                    {
                        y1 = _parent2->getRealVar()[i];
                        y2 = _parent1->getRealVar()[i];
                    }
                    yl = FLRLimits[i];
                    yu = FHRLimits[i];
                    rand = iRandGen->randomDouble(0,1);
                    beta = 1.0 + (2.0*(y1-yl)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(FEta+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(FEta+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(FEta+1.0)));
                    }
                    c1 = 0.5*((y1+y2)-betaq*(y2-y1));
                    beta = 1.0 + (2.0*(yu-y2)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(FEta+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(FEta+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(FEta+1.0)));
                    }
                    c2 = 0.5*((y1+y2)+betaq*(y2-y1));
                    if (c1<yl)
                        c1=yl;
                    if (c2<yl)
                        c2=yl;
                    if (c1>yu)
                        c1=yu;
                    if (c2>yu)
                        c2=yu;
                    if (iRandGen->randomDouble(0,1)<=0.5)
                    {
                        _child1->getRealVar()[i] = c2;
                        _child2->getRealVar()[i] = c1;
                    }
                    else
                    {
                        _child1->getRealVar()[i] = c1;
                        _child2->getRealVar()[i] = c2;
                    }
                }
                else
                {
                    _child1->getRealVar()[i] = _parent1->getRealVar()[i];
                    _child2->getRealVar()[i] = _parent2->getRealVar()[i];
                }
            }
            else
            {
                _child1->getRealVar()[i] = _parent1->getRealVar()[i];
                _child2->getRealVar()[i] = _parent2->getRealVar()[i];
            }
        }
    }
    else
    {
        for (i=0; i<FRealVarSize; i++)
        {
            _child1->getRealVar()[i] = _parent1->getRealVar()[i];
            _child2->getRealVar()[i] = _parent2->getRealVar()[i];
        }
    }
    return;
}
void CSBXCrossover::setDistributionIndex(double _di) {
    FEta = _di;
}

double CSBXCrossover::getDistributionIndex(){
    return FEta;
}


}
