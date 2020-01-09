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

#include "CGeneticAlgorithm.h"
#include "../computers/CComputer.h"
namespace MEVO {
CGeneticAlgorithm::CGeneticAlgorithm() : CEvolOptimizer()
{
    iCrossover = NULL;

    iIntCrossover = NULL;
    iRealCrossover = NULL;

     cComputer = new CComputer();

    setCrossover(new CDECrossover);
}

CGeneticAlgorithm::~CGeneticAlgorithm() {
    if(iCrossover) delete iCrossover;

    if (cComputer) delete cComputer;

    if(iIntCrossover) delete iIntCrossover;
    if(iRealCrossover) delete iRealCrossover;

}

void CGeneticAlgorithm::initVariables() {
   CEvolOptimizer::initVariables();

   if (iCrossover) {
       iCrossover->init(this);
   }

   if (iIntCrossover&&FProblem->getIntegerVarSize()){
       ICProblem* p = FProblem->copy();
       ICProblem* op = FProblem;
       p->setRealVarSize(0);
       FProblem=p;
       iIntCrossover->init(this);
       delete p;
       FProblem = op;
   }

   if  (iRealCrossover&&FProblem->getRealVarSize()) {
       ICProblem* p = FProblem->copy();
       ICProblem* op = FProblem;
       p->setIntegerVarSize(0);
       FProblem=p;
       iRealCrossover->init(this);
       delete p;
       FProblem = op;
   }

}

bool CGeneticAlgorithm::checkConf(){
    if (CEvolOptimizer::checkConf()) {
        return true;
    }
    return false;
}

void CGeneticAlgorithm::setCrossover(ICCrossover* _c) {
    if (_c) {
        if (iRealCrossover) {delete iRealCrossover; iRealCrossover=NULL;}
        if (iIntCrossover) {delete iIntCrossover; iIntCrossover=NULL;}
        if (iCrossover) {delete iCrossover;}
        iCrossover = _c;
    }
}

void CGeneticAlgorithm::setSeparateCrossovers(ICCrossover* _rc, ICCrossover* _ic) {
    if (_rc && _ic) {
        if (iRealCrossover) {delete iRealCrossover; iRealCrossover=NULL;}
        if (iIntCrossover) {delete iIntCrossover; iIntCrossover=NULL;}
        if (iCrossover) {delete iCrossover; iCrossover = NULL;}
        /*if (FProblem->getRealVarSize())*/ iRealCrossover = _rc;
        /*if (FProblem->getIntegerVarSize())*/ iIntCrossover = _ic;
    }
}

void CGeneticAlgorithm::crossoverPopulation(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation){
    SCrossoverParams p;
    p.FParentPopulation = _parentPopulation;
    p.FOffspringPopulation = _offspringPopulation;

    if (iRealCrossover&&FProblem->getRealVarSize()) {
        cComputer->setComputationTask(iRealCrossover);
        cComputer->compute(&p);

    }
    if (iIntCrossover&&FProblem->getIntegerVarSize()) {
        cComputer->setComputationTask(iIntCrossover);
        cComputer->compute(&p);
    }

    if(iCrossover) {
        cComputer->setComputationTask(iCrossover);
        cComputer->compute(&p);
    }
}
ICCrossover* CGeneticAlgorithm::getCrossover() {
    return iCrossover;
}


ICCrossover* CGeneticAlgorithm::getIntCrossover() {
    return iIntCrossover;
}


ICCrossover* CGeneticAlgorithm::getRealCrossover() {
    return iRealCrossover;
}


};
