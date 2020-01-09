#include "CComputerMultiThread.h"



#include "../core/CEvolOptimizer.h"
#include "../core/CPopulation.h"
#include "../core/CIndividual.h"
#include "../crossovers/CCrossover.h"


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

namespace MEVO {




DWORD WINAPI ComputeThread(void * _params)
{

    CThreadParams* tp = (CThreadParams*)_params;

    DWORD dwWaitResult = WaitForSingleObject(tp->FParent->ghProduction[tp->FNo],INFINITE);

    while (dwWaitResult==WAIT_OBJECT_0) {
        void* prm = tp->distribute(tp->FParams);

        tp->FComputer->compute(prm);
        ICCrossover* cr = dynamic_cast<ICCrossover*>(tp->FComputer->getComputationTask());
        if (cr) {
            SCrossoverParams *dp = (SCrossoverParams*)prm;
            delete dp->FOffspringPopulation;
            delete dp;
        } else {
            ICPopulation *population = (ICPopulation*)prm;
            delete population;
        }
        ReleaseSemaphore(tp->FParent->ghConsumption[tp->FNo],1,NULL);
        dwWaitResult = WaitForSingleObject(tp->FParent->ghProduction[tp->FNo],INFINITE);
        bool f=false;
        tp->FParent->isFinished(f);
        if (f) {break;}
    }
    return TRUE;
}

CThreadParams::CThreadParams(CComputerMultiThread* _p) {
    FParent=_p;
}

const char* CComputerMultiThread::getName() {
    std::stringstream d;
    d<<"Multithread computer, ";
    d<<"threads number: ";
    d<<FThreadNumbers;
    return d.str().c_str();
}

CComputerMultiThread::CComputerMultiThread() : CComputer()
{
    FInitiated = false;
    FthreadHandles = NULL;
    FThreadNumbers = 16;
    ghMutex = CreateMutex(
            NULL,              // default security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
    ghConsumption=NULL;
    ghProduction=NULL;
}

CComputerMultiThread::~CComputerMultiThread() {
    clear();
    CloseHandle(ghMutex);
}

void CComputerMultiThread::clear(){
    if (FInitiated) {
        if (FThreadParams.size()) {
            finishThreads();
            WaitForMultipleObjects(FThreadNumbers, FthreadHandles, TRUE, INFINITE);
        }
    }
    for(std::map<ICComputation*, std::deque<ICComputation*>* >::iterator it=FTasksMap.begin(); it!=FTasksMap.end(); ++it) {
        std::deque<ICComputation*>* dq = (std::deque<ICComputation*>*)it->second;
        for (int i = 0; i< dq->size();i++) {
            delete dq->operator [](i);
        }
        delete dq;
    }
    FTasksMap.clear();

    if(FthreadHandles) {
        for (int i=0; i< FThreadParams.size(); i++) {
            delete FThreadParams[i]->FComputer;
            delete FThreadParams[i];
            CloseHandle(FthreadHandles[i]);
            CloseHandle(ghConsumption[i]);
            CloseHandle(ghProduction[i]);
        }
    }
    FThreadParams.clear();
    if(FthreadHandles) delete [] FthreadHandles;
    if(ghConsumption) delete [] ghConsumption;
    if(ghProduction) delete [] ghProduction;
    FFinished=false;
}

void CComputerMultiThread::setThreadsNumber(unsigned int _i){
    FThreadNumbers = _i;

}

unsigned int CComputerMultiThread::getThreadsNumber()
{ return FThreadNumbers;

}

void CComputerMultiThread::compute(void* _params) {

    if (FTask) {
        std::deque<ICComputation*>* cmp = getDistributedTask(FTask);
        if (!cmp) {
            addTaskToList(FTask);
            cmp = getDistributedTask(FTask);
        }

        for (int i=0; i< FThreadNumbers;i++) {
            FThreadParams[i]->FComputer->setComputationTask(cmp->operator [](i));
            FThreadParams[i]->FParams = _params;
            ReleaseSemaphore(ghProduction[i],  1, 0);
        }
        WaitForMultipleObjects(FThreadNumbers, ghConsumption, TRUE, INFINITE);
    }

}


std::deque<ICComputation*>* CComputerMultiThread::getDistributedTask(ICComputation* _c) {
    return FTasksMap[_c];
}


void CComputerMultiThread::addTaskToList(ICComputation* _c) {
    std::deque<ICComputation*>* dt =  new std::deque<ICComputation*>;
    for (int i=0; i< FThreadParams.size();i++) {
        dt->push_back(_c->copy());
    }
    FTasksMap[_c] = dt;
}


void* CThreadParams::distribute(void* _params) {

    ICCrossover* c = dynamic_cast<ICCrossover*>(FComputer->getComputationTask());
    if (c) {return distributeCrossover(_params);}

    ICMutator* m = dynamic_cast<ICMutator*>(FComputer->getComputationTask());
    if (m) {return distributeMutator(_params);}

    ICEvaluator* e = dynamic_cast<ICEvaluator*>(FComputer->getComputationTask());
    if (e) {return distributeEvaluator(_params);}

    ICInitializer* i = dynamic_cast<ICInitializer*>(FComputer->getComputationTask());
    if(i) {return distributeInitializer(_params);}

}

ICPopulation *CThreadParams::distributePopulation(ICPopulation *population) {
    
    int b = (population->getSize()/FSize)*FNo;
    int e = (population->getSize()/FSize)*(FNo+1);
    CPopulation *thrpop = new CPopulation();
    for (int i=b; i<e; i++) {
        thrpop->addIndividual(population->getIndividual(i));
    }
    return thrpop;
}

void* CThreadParams::distributeCrossover(void* _params) {

    SCrossoverParams* p = (SCrossoverParams*)_params;

    ICPopulation* offspring = distributePopulation((ICPopulation*)p->FOffspringPopulation);

    SCrossoverParams *dp = new SCrossoverParams;
    dp->FParentPopulation = p->FParentPopulation;
    dp->FOffspringPopulation = offspring;

    return dp;
}


void* CThreadParams::distributeInitializer(void* _params) {
    return distributePopulationTask(_params);
}

void* CThreadParams::distributeMutator(void* _params) {
   return distributePopulationTask(_params);
}

void* CThreadParams::distributeEvaluator(void* _params) {
   return distributePopulationTask(_params);
}

void* CThreadParams::distributePopulationTask(void* _params) {
    ICPopulation* population = (ICPopulation*)_params;
    ICPopulation* dpopulation = distributePopulation(population);

    return dpopulation;

}
void CComputerMultiThread::init(ICEvolOptimizer* _opt) {
    prepare();
    while (_opt->getPopSize()%(FThreadNumbers)) {
        _opt->setPopSize(_opt->getPopSize()+1);
    }
}

void CComputerMultiThread::finishThreads()
{
    WaitForSingleObject(ghMutex, INFINITE);
    FFinished = true;
    for (int i=0; i< FThreadNumbers;i++) {ReleaseSemaphore(ghProduction[i],  1, NULL);Sleep(100);}
    ReleaseMutex(ghMutex);
}

void CComputerMultiThread::isFinished(bool &_finished)
{
    WaitForSingleObject(ghMutex, INFINITE);
    _finished = FFinished;
    ReleaseMutex(ghMutex);
}

void CComputerMultiThread::prepare()
{
    clear();
//    if (FThreadNumbers>MAXIMUM_PROCESSORS) {
//        FThreadNumbers=MAXIMUM_PROCESSORS;
//    }

    for (unsigned int i=0; i< FThreadNumbers; i++) {
        CComputer* st = new CComputer;
        CThreadParams* p = new CThreadParams(this);
        p->FComputer = st;
        p->FSize = FThreadNumbers;
        p->FNo=i;
        FThreadParams.push_back(p);
    }

    ghConsumption = new HANDLE[FThreadNumbers];
    ghProduction = new HANDLE[FThreadNumbers];
    for (int i=0; i< FThreadNumbers; i++) {
        ghConsumption[i] = CreateSemaphore(NULL, 0, 1, NULL);
        ghProduction[i] = CreateSemaphore(NULL, 0, 1, NULL);
    }

    FthreadHandles = new HANDLE[FThreadNumbers];
    for(unsigned int i=0; i < FThreadNumbers; i++ ) {
        FthreadHandles[i] = CreateThread(NULL, 0, ComputeThread, (void*)FThreadParams[i], 0, NULL);
    }
    FInitiated = true;
}

}


