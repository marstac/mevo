#ifndef CCOMPUTER_H
#define CCOMPUTER_H

#include "../interfaces/ICComputer.h"

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

//! Class for single thread execution of computations
/** Provides functions for single thread execution of computation tasks
    ex. mutation, crossover, evaluation of populations.
  */
class CComputer : public ICComputer
{
public:

    /** constructor
    */
    CComputer();

    /** destructor
    */
    virtual ~CComputer();

    /** Description of selected computer
    @return brief description of chosen computer
    */
    virtual const char *getName();

    /** function for execution of a computation. computes a task for given parameters.
        @param _params the parameters necessary to perform computation
                        ex. population to mutate
      */
    virtual void compute(void* _params);

    /** function for to assign a computation task
        sets FTask field value.
        @param _c a pointer to a computation object.
      */
    virtual void setComputationTask(ICComputation* _c);

    /** function which returns assigned computation task
        @return  pointer to a computation object.
    */
    virtual ICComputation* getComputationTask();

    /** function for copying a computer object
        @return an exact copy of an computer object.
      */
    virtual ICComputer* copy();

    /** function for initializing a computer object
        checks a population size value.
        @param _opt pointer to a evolutionary algorithm.
      */
    virtual void init(ICEvolOptimizer* _opt);
protected:

    /** assigned computation task*/
    ICComputation* FTask;

};
}
#endif // CCOMPUTER_H
