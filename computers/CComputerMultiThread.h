#ifndef CCOMPUTERMULTITHREAD_H
#define CCOMPUTERMULTITHREAD_H

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


#include <sstream>
#include <windows.h>
#include <deque>
#include <map>


#include "../interfaces/ICCrossover.h"
#include "../interfaces/ICMutator.h"
#include "../interfaces/ICEvaluator.h"

#include "CComputer.h"

namespace MEVO{

class CComputerMultiThread;



class CThreadParams {
public:
    CThreadParams(CComputerMultiThread* _p);
    ICComputer* FComputer;
    void* FParams;

    void* distribute(void* _params);
    ICPopulation* distributePopulation(ICPopulation *population);

    void* distributeCrossover(void* _params);
    void* distributeMutator(void* _params);
    void* distributeEvaluator(void* _params);
    void* distributeInitializer(void* _params);

    void* distributePopulationTask(void* _params);

    int FNo;
    int FSize;

    CComputerMultiThread* FParent;

};


class CComputerMultiThread : public CComputer
{
public:
    CComputerMultiThread();
    virtual ~CComputerMultiThread();


    virtual const char *getName();

    virtual void compute(void* _params);
    void setThreadsNumber(unsigned int _i);
    unsigned int getThreadsNumber();

    virtual void init(ICEvolOptimizer* _opt);

    HANDLE *ghConsumption;
    HANDLE *ghProduction;

    void finishThreads();
    void isFinished(bool &_finished);

private:
    void prepare();

    std::deque<CThreadParams*> FThreadParams;

    std::deque<ICComputation*>* getDistributedTask(ICComputation* _c);

    void addTaskToList(ICComputation* _c);

    std::map<ICComputation*, std::deque<ICComputation*>* > FTasksMap;

    HANDLE* FthreadHandles;
    unsigned int FThreadNumbers;
    void clear();

    bool FInitiated;
    bool FFinished;

    HANDLE ghMutex;

   };
}
#endif // CCOMPUTERMULTITHREAD_H
