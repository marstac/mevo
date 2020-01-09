#ifndef CCROSSOVER_H
#define CCROSSOVER_H

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
#include "../interfaces/ICEvolOptimizer.h"
#include "../interfaces/ICProblem.h"
#include "../interfaces/ICCrossover.h"
#include "../utils/CWithRNGen.h"
namespace MEVO{


struct SCrossoverParams {
    ICPopulation *FParentPopulation;
    ICPopulation *FOffspringPopulation;
};

//! Base class for crossover
/** Provides functions for implementation of different crossover types.
  */
class CCrossover : virtual public ICCrossover, virtual public CWithRNGen
{
public:
    /** Constructor*/
    CCrossover();

    /** Destructor*/
    virtual ~CCrossover();

    /** function for initializing a crossover object
        sets real and integer variables number.
        @param _opt pointer to a evolutionary algorithm.
    */
    virtual void init(ICEvolOptimizer* _opt);

    /** function for execution of a crossover
            this function calls crossover function.
            @param _params the parameters necessary to perform crossover
    */
    virtual void execute(void* _params);

    /** pure virtual function for crossover parent population
            this function is implemented in inherited classes.
            @param _parentPopulation reference to a parent population
            @param _offspringPopulation reference to a offspring population
    */
    virtual void crossover(ICPopulation *_parentPopulation, ICPopulation *_offspringPopulation)=0;

    /** pure virtual function for copying a computation object
            this function is implemented in inherited classes.
            @return an exact copy of an computation object.
    */
    virtual ICComputation* copy()=0;

    /** function sets crossover probability
        @param probability value
    */
    void setProbability(double _p);

    /** function returns crossover probability
        @return _p probability value
    */
    double  getProbability();


protected:
    /** function copies parameters from selected crossover object
        @param _c crossover object
    */
    virtual void copyParams(CCrossover* _c);

    /** number of real coded variables to be crossovered*/
    unsigned int FRealVarSize;

    /** number of integer coded variables to be crossovered*/
    unsigned int FIntVarSize;

    /** Crossover probability*/
    double FProb;

};
}
#endif // CCROSSOVER_H
