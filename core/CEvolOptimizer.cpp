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

#include "CEvolOptimizer.h"


#include <stddef.h>
#include <math.h>
#include <time.h>

#include "../utils/messages.h"
#include "../random_numbers/CRandomNumbersGenerator.h"
#include "../initializers/CUniformInitializer.h"

#include "../stops/CStopOnGen.h"
#include "../stops/CHVStop.h"

#include "../computers/CComputerMultiThread.h"
#include "../notifiers/CConsoleNotifier.h"
#include "../notifiers/CFileNotifier.h"
#include "../mutators/CPolynomialMutator.h"

#include "CEvaluator.h"
#include "CPopulation.h"
#include "CIndividual.h"
namespace MEVO{
CEvolOptimizer::CEvolOptimizer() : CWithNotifiers() , CWithRNGen() {

  FProblem = NULL;

  FTerminated =false;

  FGenerations = 100;
  FPopSize = 100;

  iIntInitializer=NULL;
  iRealInitializer=NULL;
  iInitializer=NULL;
  iStop=NULL;
  FLastPop=NULL;

  iEvaluator = new CEvaluator();
  FEps=0.0;

  iMutator = NULL;

  iIntMutator = NULL;
  iRealMutator = NULL;

  iComputer = new CComputerMultiThread();
  //iComputer = new CComputer();
  mComputer = new CComputer();
  setInitializer(new CUniformInitializer());
  setMutator(new CPolynomialMutator);
  setStop(new CHVStop());

  FInitialPop = NULL;
    FCurrentGeneration=0;
  addNotifier(new CConsoleNotifier);
  //addNotifier(new CFileNotifier);


}

CEvolOptimizer::~CEvolOptimizer() {

    if (iEvaluator) delete iEvaluator;
    if (iComputer) delete iComputer;

    if (mComputer) delete mComputer;

    if (iMutator) delete iMutator;
    if (iIntMutator) delete iIntMutator;
    if (iRealMutator) delete iRealMutator;
}

void CEvolOptimizer::setEvaluator(ICEvaluator* _e) {
    delete iEvaluator;
    iEvaluator = _e;
}


void CEvolOptimizer::setComputer(ICComputer* _c) {
    if (iComputer) {delete iComputer;}
    iComputer = _c;
}

ICComputer* CEvolOptimizer::getComputer() {
    return iComputer;
}

void CEvolOptimizer::evaluatePopulation(ICPopulation *_population) {
    iComputer->setComputationTask(iEvaluator);
    iComputer->compute(_population);
}

void CEvolOptimizer::initVariables() {

    FCurrentGeneration=0;
    if(FLastPop) {
        FLastPop->deleteIndividuals();
        delete FLastPop;
        FLastPop = NULL;
    }

    iComputer->init(this);

    iEvaluator->init(this);
    iStop->init(this);
    if (iInitializer) {
        iInitializer->init(this);
    }

    if (iIntInitializer&&iRealInitializer){
        ICProblem* op = FProblem;

        ICProblem* p = FProblem->copy();
        p->setRealVarSize(0);
        FProblem=p;
        iIntInitializer->init(this);
        delete p;

        p=op->copy();
        p->setIntegerVarSize(0);
        FProblem=p;
        iRealInitializer->init(this);
        delete p;

        FProblem = op;
    }

    if (iMutator) {
        iMutator->init(this);
    }
    if (iIntMutator&&FProblem->getIntegerVarSize()){

        ICProblem* p = FProblem->copy();
        ICProblem* op = FProblem;

        p->setRealVarSize(0);
        FProblem=p;
        iIntMutator->init(this);
        delete p;
        FProblem = op;
    }

    if(iRealMutator&&FProblem->getRealVarSize()) {
        ICProblem* p = FProblem->copy();
        ICProblem* op = FProblem;
        p->setIntegerVarSize(0);
        FProblem=p;
        iRealMutator->init(this);
        delete p;
        FProblem = op;
    }
    srand(time(0));
}

void CEvolOptimizer::mutatePopulation(ICPopulation *_population) {
    if (iRealMutator&&FProblem->getRealVarSize()) {
        mComputer->setComputationTask(iRealMutator);
        mComputer->compute(_population);
    }

    if (iIntMutator&&FProblem->getIntegerVarSize()) {
        mComputer->setComputationTask(iIntMutator);
        mComputer->compute(_population);
    }

    if (iMutator){
        mComputer->setComputationTask(iMutator);
        mComputer->compute(_population);
    }
}

void CEvolOptimizer::setMutator(ICMutator* _m) {
    if (_m) {
        if (iRealMutator) {delete iRealMutator; iRealMutator=NULL;}
        if (iIntMutator) {delete iIntMutator; iIntMutator=NULL;}
        if (iMutator) {delete iMutator;}
        iMutator = _m;
    }
}


void CEvolOptimizer::setSeparateMutators(ICMutator* _rm, ICMutator* _im) {
    if (_rm && _im) {
        if (iRealMutator) {delete iRealMutator; iRealMutator=NULL;}
        if (iIntMutator) {delete iIntMutator; iIntMutator=NULL;}
        if (iMutator) {delete iMutator;iMutator=NULL;}
        /*if(FProblem->getRealVarSize())*/ iRealMutator = _rm;
        /*if(FProblem->getIntegerVarSize())*/ iIntMutator = _im;
    }
}

ICMutator* CEvolOptimizer::getMutator() {
    return iMutator;
}

ICMutator* CEvolOptimizer::getIntMutator(){
    return iIntMutator;
}

ICMutator* CEvolOptimizer::getRealMutator(){
    return iRealMutator;
}

void CEvolOptimizer::setPopSize(unsigned int _v) {
  FPopSize = _v;
}

unsigned int CEvolOptimizer::getPopSize() {
  return FPopSize;
}

unsigned int CEvolOptimizer::getCurrentGeneration() {
  return FCurrentGeneration;
}


void CEvolOptimizer::setEpsilon(double _eps) {
   FEps = _eps;
}


double CEvolOptimizer::getEpsilon() {
   return FEps;
}

void CEvolOptimizer::terminate() {
  FTerminated = true;
}

bool CEvolOptimizer::isTerminated() {
  return FTerminated;
}

void CEvolOptimizer::setInitialPopulation(ICPopulation* _pop) {
    FInitialPop = _pop;
}

ICPopulation* CEvolOptimizer::getInitialPopulation(){
    return FInitialPop;
}

void CEvolOptimizer::setLastPop(ICPopulation *_p) {
    if(FLastPop) {
        FLastPop->deleteIndividuals();
        delete FLastPop;
        FLastPop = NULL;
    }
    FLastPop = _p->copy();
}

ICPopulation* CEvolOptimizer::getLastPop() {
    return FLastPop;
}

bool CEvolOptimizer::checkConf() {
    if (FProblem->getRealVarSize()==0 && FProblem->getIntegerVarSize()==0) {
    fireOnErrorNotify(E_OPT_FBV_NS);
    return false;
  }

  if (FProblem->getObjectiveFncCnt()<2) {
    fireOnErrorNotify(E_OPT_NMO);
    return false;
  }
  return true;
}


ICInitializer* CEvolOptimizer::getInitializer() {
    return iInitializer;
}

ICInitializer* CEvolOptimizer::getIntInitializer() {
    return iIntInitializer;
}

ICInitializer* CEvolOptimizer::getRealInitializer() {
    return iRealInitializer;
}


void CEvolOptimizer::setInitializer(ICInitializer* _i) {
    if (_i) {
        if (iRealInitializer) {delete iRealInitializer; iRealInitializer=NULL;}
        if (iIntInitializer) {delete iIntInitializer; iIntInitializer=NULL;}
        if (iInitializer) {delete iInitializer;}
        iInitializer = _i;
    }
}


void CEvolOptimizer::setSeparateInitializers(ICInitializer* _ri, ICInitializer* _ii) {
    if (_ri && _ii) {
        if (iRealInitializer) {delete iRealInitializer; iRealInitializer=NULL;}
        if (iIntInitializer) {delete iIntInitializer; iIntInitializer=NULL;}
        if (iInitializer) {delete iInitializer; iInitializer = NULL;}
        iRealInitializer = _ri;
        iIntInitializer = _ii;
    }
}
void CEvolOptimizer::initializePopulation(ICPopulation* _population) {

    if (FInitialPop) {
        copyInitialPop(_population);
        return;
    }


    if (iRealInitializer && iIntInitializer) {
        if(FProblem->getRealVarSize()) {
            iComputer->setComputationTask(iRealInitializer);
            iComputer->compute(_population);
        }
        if (FProblem->getIntegerVarSize()) {
            iComputer->setComputationTask(iIntInitializer);
            iComputer->compute(_population);
        }

    } else {
        iComputer->setComputationTask(iInitializer);
        iComputer->compute(_population);
    }
}

void CEvolOptimizer::copyInitialPop(ICPopulation *_population) {
    int isize=FInitialPop->getSize();
    int psize=_population->getSize();

    int rest = psize-isize;

    if (rest>=0) {
        for (int i=0;i<isize; i++) {
            CIndividual* ind = dynamic_cast<CIndividual*>(_population->getIndividual(i));
            if(ind) ind->CIndividual::copyParams((const CIndividual*)FInitialPop->getIndividual(i));
        }
        CPopulation rpop;
        for (int i=isize; i<psize; i++) {
            rpop.addIndividual(_population->getIndividual(i));
            ICComputer* tmp = iComputer;
            CComputer* c =  new CComputer();
            c->init(this);
            iComputer = c;
            FInitialPop = NULL;
            initializePopulation(&rpop);
            iComputer = tmp;
            delete c;
        }
    } else {
        for (int i=0;i<psize; i++) {
            CIndividual* ind = dynamic_cast<CIndividual*>(_population->getIndividual(i));
            if(ind) ind->CIndividual::copyParams((const CIndividual*)FInitialPop->getIndividual(i));
        }
    }
}

void CEvolOptimizer::setStop(ICStop* _s) {
    if (iStop) {delete iStop;}
    iStop = _s;
}

ICStop* CEvolOptimizer::getStop() {
   return iStop;
}

bool CEvolOptimizer::checkStop(ICPopulation *_population) {
    return iStop->checkStop(_population);
}

ICProblem* CEvolOptimizer::getProblem() const
{
    return FProblem;
}

void CEvolOptimizer::setProblem(ICProblem *_p)
{
    FProblem = _p;

}

}
