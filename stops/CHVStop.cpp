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

#include "CHVStop.h"

#include <limits>

#include <iostream>
#include <math.h>

#include "../indicators/CSweepHVIndicator.h"
#include "../indicators/CMCHVindicator.h"


#include "../utils/functions.h"
namespace MEVO {
CHVStop::CHVStop()
{
    FCC=0.001;
    FFrequency =20;
    FCurrentGeneration=0;

    FLastPop=NULL;
    FIndicator=NULL;
}

CHVStop::~CHVStop() {
   if (FLastPop) {
       FLastPop->deleteIndividuals();
       delete FLastPop;
    }
    delete FIndicator;
}

const char* CHVStop::getDescription() {
    std::stringstream desc;
    desc << "Hypervolume based stop" << "\tChceck Frequency:\t"<<FFrequency<<"\tConvergence coeff.:\t"<<FCC;
    return desc.str().c_str();
}

void CHVStop::setConvergence(double _c){
    FCC = _c;
}


void  CHVStop::setFrequency(double _f) {
    FFrequency = _f;
}


double  CHVStop::getFrequency() {
    return FFrequency;
}

void  CHVStop::init(ICEvolOptimizer* _opt) {

   if (FLastPop) {
       FLastPop->deleteIndividuals();
       delete FLastPop;
       FLastPop=NULL;
   }
   if (FIndicator){delete FIndicator;}
   if (_opt->getProblem()->getObjectiveFncCnt()<5) {
       FIndicator = new CSweepHVIndicator();
   } else {
       FIndicator = new CMCSHVIndicator();
   }

   FCurrentGeneration=0;
}

bool CHVStop::checkStop(ICPopulation *_population) {
    bool result=false;
    if (FCurrentGeneration%FFrequency==0) {


        ICPopulation* p =_population->getNondominatedPopulation();
        ICPopulation* f = p->getFeasiblePopulation();
        if (f->getSize()==0){selectOne(p);}

        delete f;

        if (FLastPop) {
             if (FLastPop->getSize()==1 && p->getSize()>1){result=false;}
             else if (FLastPop->getSize()==1 && p->getSize()==1) {result=compareOnes(p);}
             else {
                 double v = FIndicator->comparePopulations(FLastPop, p);
                 if (v<FCC){result=true;}
             }
         }

        setLastPop(p);
    }
    FCurrentGeneration++;
    return result;
}

void CHVStop::setLastPop(ICPopulation *_population) {
    if (FLastPop) {
        FLastPop->deleteIndividuals();
        delete FLastPop;
    }
    FLastPop = _population->copy();
    delete _population;

}

double CHVStop::selectOne(ICPopulation *_population)
{
    ICIndividual* indb=_population->getIndividual(0);
    for(int i = 1; i<_population->getSize();i++){
        if(_population->getIndividual(i)->getConstrViolation() < indb->getConstrViolation()) {
            indb=_population->getIndividual(i);
        }
    }
    _population->clearIndividuals();
    _population->addIndividual(indb);
}

bool CHVStop::compareOnes(ICPopulation *_population) {
    double lp=FLastPop->getIndividual(0)->getConstrViolation();
    double cp = _population->getIndividual(0)->getConstrViolation();
    if(1-(cp/lp) < FCC){return true;}
    return false;
}

}
